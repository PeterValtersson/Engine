#pragma once
#include <IEngine.h>
namespace Engine
{
	class Engine :
		public IEngine {
	public:
		Engine( const Init_Info& init_info );

		virtual void start()noexcept override;
		
		virtual Managers get_managers() override;
		virtual Sub_Systems get_sub_systems() override;
	private:
		void frame()noexcept;
		void quit()noexcept;


		void init_sub_systems();
		void init_managers();

		void init_window();
		void init_renderer();
		void init_resource_archive();
		void init_resource_handler();

		void init_entity_manager();
		void init_transform_manager();
		void init_property_manager();
		void init_scene_manager();
		void init_camera_manager();
	private:

		Sub_Systems sub_systems;
		Managers managers;


		bool running;
	};

}