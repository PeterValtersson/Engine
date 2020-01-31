#ifndef _ENGINE_H_
#define _ENGINE_H_
#pragma once
#include "DLL_Export.h"
#include <IResourceHandler.h>
#include <Graphics/Renderer_Interface.h>
#include <Window_Interface.h>
#include <Managers/TransformManager_Interface.h>
#include <Managers/PropertyManager_Interface.h>
#include <Managers/SceneManager_Interface.h>
#include <Managers/CameraManager_Interface.h>

namespace Engine
{
	struct Sub_Systems {
		std::shared_ptr<Window::Window_Interface>		window;
		std::shared_ptr<Renderer::Renderer_Interface>	renderer;
		std::shared_ptr<ResourceHandler::IResourceArchive>	resource_archive;
		std::shared_ptr<ResourceHandler::IResourceHandler>	resource_handler;
	};
	struct Managers {
		std::shared_ptr<ECS::EntityManager_Interface>		entity_manager;
		std::shared_ptr<ECS::TransformManager_Interface>	transform_manager;
		std::shared_ptr<ECS::PropertyManager_Interface>		property_manager;
		std::shared_ptr<ECS::SceneManager_Interface>		scene_manager;
		std::shared_ptr<ECS::CameraManager_Interface>		camera_manager;

	};
	struct Init_Info {
		ResourceHandler::AccessMode mode = ResourceHandler::AccessMode::read;
		Sub_Systems sub_systems;
		Managers managers;
	};

	class IEngine 
	{
	public:
		DECLSPEC_ENGINE static std::unique_ptr<IEngine> create(const Init_Info& init_info );
		virtual ~IEngine()
		{}

		virtual void start(bool threaded = false)noexcept = 0;
	
		virtual Managers get_managers() = 0;
		virtual Sub_Systems get_sub_systems() = 0;
	protected:


	};

}

#endif