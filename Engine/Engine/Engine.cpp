#include "Engine.h"
#include <Utilities/Profiler/Profiler.h>

ECSEngine::ECSEngine::ECSEngine( const Init_Info& init_info ) : sub_systems( init_info.sub_systems ), entity_components( init_info.entity_components ), running( false )
{

	init_sub_systems( init_info.mode );
	init_managers();
}

ECSEngine::ECSEngine::~ECSEngine()
{
	running = false;
	if ( thread.joinable() )
		thread.join();
}

void ECSEngine::ECSEngine::start( bool threaded ) noexcept
{
	PROFILE;
	sub_systems.renderer->Start();
	sub_systems.window->MapActionButton( 0, Window::KeyCode::KeyEscape );
	sub_systems.window->BindKeyPressCallback( 0, { this, &ECSEngine::quit } );
	sub_systems.window->BindOnQuitEvent( { this, &ECSEngine::quit } );

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

ECSEngine::EntityComponents ECSEngine::ECSEngine::get_entity_components()
{
	return entity_components;
}

ECSEngine::Sub_Systems ECSEngine::ECSEngine::get_sub_systems()
{
	return sub_systems;
}

const std::vector<std::weak_ptr<ECS::ComponentReflection>> ECSEngine::ECSEngine::get_component_reflections() const
{
	std::vector<std::weak_ptr<ECS::ComponentReflection>> ret;
	ret.push_back(entity_components.transform_component);
	return ret;
}

void ECSEngine::ECSEngine::frame() noexcept
{
	PROFILE;
	sub_systems.window->Frame();
}

void ECSEngine::ECSEngine::quit() noexcept
{
	running = false;
}

void ECSEngine::ECSEngine::init_sub_systems( ResourceHandler::AccessMode mode )
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

void ECSEngine::ECSEngine::init_managers()
{
	if ( !entity_components.entity_factory )
		init_entity_manager();
	if ( !entity_components.transform_component )
		init_transform_manager();
	if ( !entity_components.property_component )
		init_property_manager();
	if ( !entity_components.scene_component )
		init_scene_manager();
	if ( !entity_components.camera_component )
		init_camera_manager();

}

void ECSEngine::ECSEngine::init_window()
{
	sub_systems.window = Window::Window_Interface::create_window( Window::Window_Type::SDL, {} );
}

void ECSEngine::ECSEngine::init_renderer()
{
	Renderer::RendererInitializationInfo ii;
	auto res = sub_systems.window->GetResolution();
	ii.resolution = *( Renderer::Resolution* )&res;
	ii.windowHandle = sub_systems.window->GetWindowHandle();
	sub_systems.renderer = Renderer::Renderer_Interface::Create_Renderer( Renderer::Renderer_Backend::DIRECTX11, ii );
}

void ECSEngine::ECSEngine::init_resource_archive( ResourceHandler::AccessMode mode )
{
	sub_systems.resource_archive = ResourceHandler::IResourceArchive::create_binary_archive( "data.dat", mode );
}

void ECSEngine::ECSEngine::init_resource_handler( ResourceHandler::AccessMode mode )
{
	sub_systems.resource_handler = ResourceHandler::IResourceHandler::create( mode, sub_systems.resource_archive );
	ResourceHandler::IResourceHandler::set( sub_systems.resource_handler );
}

void ECSEngine::ECSEngine::init_entity_manager()
{
	entity_components.entity_factory = ECS::EntityFactory::create_factory();
}

void ECSEngine::ECSEngine::init_transform_manager()
{
	entity_components.transform_component = ECS::EntityComponentFactory::create_transform_component( { entity_components.entity_factory } );
}

void ECSEngine::ECSEngine::init_property_manager()
{
	entity_components.property_component = ECS::EntityComponentFactory::create_property_manager( { entity_components.entity_factory } );
}

void ECSEngine::ECSEngine::init_scene_manager()
{
	entity_components.scene_component = ECS::EntityComponentFactory::create_scene_component( { entity_components.entity_factory } );
}

void ECSEngine::ECSEngine::init_camera_manager()
{
	entity_components.camera_component = ECS::EntityComponentFactory::create_viewpoint_component( { entity_components.entity_factory, entity_components.transform_component } );
}
