#ifndef _ENGINE_H_
#define _ENGINE_H_
#pragma once
#include "DLL_Export.h"
#include <IResourceHandler.h>
#include <Graphics/Renderer_Interface.h>
#include <Window_Interface.h>
#include <ECS/Components/EntityComponentFactory.h>

namespace Engine
{
	struct Sub_Systems {
		std::shared_ptr<Window::Window_Interface>		window;
		std::shared_ptr<Renderer::Renderer_Interface>	renderer;
		std::shared_ptr<ResourceHandler::IResourceArchive>	resource_archive;
		std::shared_ptr<ResourceHandler::IResourceHandler>	resource_handler;
	};
	struct EntityComponents {
		std::shared_ptr<ECS::EntityFactory>		entity_factory;
		std::shared_ptr<ECS::TransformComponent>	transform_component;
		std::shared_ptr<ECS::PropertyComponent>		property_component;
		std::shared_ptr<ECS::SceneComponent>		scene_component;
		std::shared_ptr<ECS::ViewpointComponent>	camera_component;

	};
	struct Init_Info {
		ResourceHandler::AccessMode mode = ResourceHandler::AccessMode::read;
		Sub_Systems sub_systems;
		EntityComponents entity_components;
	};

	class IEngine 
	{
	public:
		DECLSPEC_ENGINE static std::unique_ptr<IEngine> create(const Init_Info& init_info );
		virtual ~IEngine()
		{}

		virtual void start(bool threaded = false)noexcept = 0;
	
		virtual EntityComponents get_entity_components() = 0;
		virtual Sub_Systems get_sub_systems() = 0;
		virtual const std::vector<std::weak_ptr<ECS::ComponentReflection>> get_component_reflections()const = 0;
	protected:


	};

}

#endif