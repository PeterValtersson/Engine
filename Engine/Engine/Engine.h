#pragma once
#include <IEngine.h>
namespace Engine
{
	class Engine :
		public IEngine {
	public:
		Engine( Init_Info init_info );

	private:
		void init_sub_systems();
		void init_managers();

		void init_renderer();
		void init_resource_archive();
		void init_resource_handler();

	private:

		Sub_Systems sub_systems;
		Managers managers;
	};

}