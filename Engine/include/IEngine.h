#ifndef _ENGINE_H_
#define _ENGINE_H_
#pragma once
#include "DLL_Export.h"
#include <IResourceHandler.h>
#include <Graphics/Renderer_Interface.h>

namespace Engine
{
	struct Sub_Systems {
		std::shared_ptr<Graphics::Renderer_Interface>	renderer;
		std::shared_ptr<Resources::IResourceArchive>	resource_archive;
		std::shared_ptr<Resources::IResourceHandler>	resource_handler;
	};
	struct Managers {

	};
	struct Init_Info {
		Sub_Systems sub_systems;
		Managers managers;
	};

	class IEngine 
	{
	public:
		DECLSPEC_ENGINE static std::unique_ptr<IEngine> create( Init_Info init_info );
	protected:


	};

}

#endif