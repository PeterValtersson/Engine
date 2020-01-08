#include "Engine.h"

Engine::Engine::Engine( Init_Info init_info ) : sub_systems( init_info.sub_systems ), managers( init_info.managers )
{

	init_sub_systems();
	init_managers();
}

void Engine::Engine::init_sub_systems()
{
	if ( !sub_systems.resource_archive )
		init_resource_archive();
	if ( !sub_systems.resource_handler )
		init_resource_handler();

}

void Engine::Engine::init_managers()
{}

void Engine::Engine::init_resource_archive()
{
	sub_systems.resource_archive = Resources::IResourceArchive::create_binary_archive( "data.dat", Resources::AccessMode::read );
}

void Engine::Engine::init_resource_handler()
{
	sub_systems.resource_handler = Resources::IResourceHandler::create( Resources::AccessMode::read, sub_systems.resource_archive );
	Resources::IResourceHandler::set( sub_systems.resource_handler );
}
