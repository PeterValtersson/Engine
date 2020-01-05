#ifndef _ENGINE_H_
#define _ENGINE_H_
#pragma once
#include "DLL_Export.h"
#include <IResourceHandler.h>

namespace Engine
{
	struct Init_Info {
		std::shared_ptr<Resources::IResourceArchive> resource_archive;
		std::shared_ptr<Resources::IResourceHandler> resource_handler;
	};

	class IEngine 
	{
	public:
		DECLSPEC_ENGINE static std::shared_ptr<IEngine> create( Init_Info init_info );
	protected:


	};

}

#endif