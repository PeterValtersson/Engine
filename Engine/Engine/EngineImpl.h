#pragma once
#include <Engine.h>
#include <thread>

namespace ECSEngine
{
	class EngineImpl :
		public Engine {
	public:
		EngineImpl( const Init_Info& init_info );
		virtual ~EngineImpl();

		virtual void start( bool threaded = false )noexcept override;
		
		virtual EntityComponents get_entity_components() override;
		virtual Sub_Systems get_sub_systems() override;
		virtual const std::vector<std::weak_ptr<ECS::ComponentReflection>> get_component_reflections()const override;
	private:
		void frame()noexcept;
		void quit()noexcept;


		void init_sub_systems( ResourceHandler::AccessMode mode );
		void init_managers();

		void init_window();
		void init_renderer();
		void init_resource_archive( ResourceHandler::AccessMode mode );
		void init_resource_handler( ResourceHandler::AccessMode mode );

		void init_entity_manager();
		void init_transform_manager();
		void init_property_manager();
		void init_scene_manager();
		void init_camera_manager();
	private:

		Sub_Systems sub_systems;
		EntityComponents entity_components;


		bool running;
		std::thread thread;
	};

}