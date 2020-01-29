#include "Engine.h"

Engine::Engine::Engine( const Init_Info& init_info ) : sub_systems( init_info.sub_systems ), managers( init_info.managers )
{

	init_sub_systems();
	init_managers();
}

void Engine::Engine::init_sub_systems()
{
	if ( !sub_systems.window )
		init_window();
	if ( !sub_systems.renderer )
		init_renderer();
	if ( !sub_systems.resource_archive )
		init_resource_archive();
	if ( !sub_systems.resource_handler )
		init_resource_handler();

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
	sub_systems.renderer = Graphics::Renderer_Interface::Create_Renderer( Graphics::Renderer_Backend::DIRECTX11, {} );
}

void Engine::Engine::init_resource_archive()
{
	sub_systems.resource_archive = Resources::IResourceArchive::create_binary_archive( "data.dat", Resources::AccessMode::read );
}

void Engine::Engine::init_resource_handler()
{
	sub_systems.resource_handler = Resources::IResourceHandler::create( Resources::AccessMode::read, sub_systems.resource_archive );
	Resources::IResourceHandler::set( sub_systems.resource_handler );
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
