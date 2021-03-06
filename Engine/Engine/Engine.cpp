#include "Engine.h"
#include <Utilities/Profiler/Profiler.h>

Engine::Engine::Engine( const Init_Info& init_info ) : sub_systems( init_info.sub_systems ), managers( init_info.managers ), running( false )
{

	init_sub_systems( init_info.mode );
	init_managers();
}

Engine::Engine::~Engine()
{
	running = false;
	if ( thread.joinable() )
		thread.join();
}

void Engine::Engine::start( bool threaded ) noexcept
{
	PROFILE;
	sub_systems.renderer->Start();
	sub_systems.window->MapActionButton( 0, Window::KeyCode::KeyEscape );
	sub_systems.window->BindKeyPressCallback( 0, { this, &Engine::quit } );
	sub_systems.window->BindOnQuitEvent( { this, &Engine::quit } );

	running = true;
	if ( threaded )
	{
		thread = std::thread( [this]
		{
			while ( running )
				frame();
		} );
	}
	else
		while ( running )
			frame();
}

Engine::Managers Engine::Engine::get_managers()
{
	return managers;
}

Engine::Sub_Systems Engine::Engine::get_sub_systems()
{
	return sub_systems;
}

void Engine::Engine::frame() noexcept
{
	PROFILE;
	sub_systems.window->Frame();
}

void Engine::Engine::quit() noexcept
{
	running = false;
}

void Engine::Engine::init_sub_systems( ResourceHandler::AccessMode mode )
{
	if ( !sub_systems.window )
		init_window();
	if ( !sub_systems.renderer )
		init_renderer();
	if ( !sub_systems.resource_archive )
		init_resource_archive( mode );
	if ( !sub_systems.resource_handler )
		init_resource_handler( mode );

}

void Engine::Engine::init_managers()
{
	if ( !managers.entity_manager )
		init_entity_manager();
	if ( !managers.transform_manager )
		init_transform_manager();
	if ( !managers.property_manager )
		init_property_manager();
	if ( !managers.scene_manager )
		init_scene_manager();
	if ( !managers.camera_manager )
		init_camera_manager();

}

void Engine::Engine::init_window()
{
	sub_systems.window = Window::Window_Interface::create_window( Window::Window_Type::SDL, {} );
}

void Engine::Engine::init_renderer()
{
	Renderer::RendererInitializationInfo ii;
	ii.resolution = *( Renderer::Resolution* ) & sub_systems.window->GetResolution();
	ii.windowHandle = sub_systems.window->GetWindowHandle();
	sub_systems.renderer = Renderer::Renderer_Interface::Create_Renderer( Renderer::Renderer_Backend::DIRECTX11, ii );
}

void Engine::Engine::init_resource_archive( ResourceHandler::AccessMode mode )
{
	sub_systems.resource_archive = ResourceHandler::IResourceArchive::create_binary_archive( "data.dat", mode );
}

void Engine::Engine::init_resource_handler( ResourceHandler::AccessMode mode )
{
	sub_systems.resource_handler = ResourceHandler::IResourceHandler::create( mode, sub_systems.resource_archive );
	ResourceHandler::IResourceHandler::set( sub_systems.resource_handler );
}

void Engine::Engine::init_entity_manager()
{
	managers.entity_manager = ECS::EntityManager_Interface::create_manager();
}

void Engine::Engine::init_transform_manager()
{
	managers.transform_manager = ECS::TransformManager_Interface::create_manager( { managers.entity_manager } );
}

void Engine::Engine::init_property_manager()
{
	managers.property_manager = ECS::PropertyManager_Interface::create_manager( { managers.entity_manager } );
}

void Engine::Engine::init_scene_manager()
{
	managers.scene_manager = ECS::SceneManager_Interface::create_manager( { managers.entity_manager } );
}

void Engine::Engine::init_camera_manager()
{
	managers.camera_manager = ECS::CameraManager_Interface::create_manager( { managers.entity_manager, managers.transform_manager } );
}
