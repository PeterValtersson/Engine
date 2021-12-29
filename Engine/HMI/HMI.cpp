#include <HMI.h>
#include "SDL_Window_Impl.h"

std::shared_ptr<ECSEngine::HMI> ECSEngine::HMI::create_window(HMIType type, const InitializationInfo& init_info)
{
	switch (type)
	{
	case ECSEngine::HMIType::SDL:
		return std::make_shared<SDL_Window_Impl>(init_info);
		break;
	case ECSEngine::HMIType::OpenVR:
		break;
	default:
		break;
	}
	return nullptr;
}
