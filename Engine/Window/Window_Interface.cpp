#include <Window_Interface.h>
#include "SDL_Window_Impl.h"

std::shared_ptr<Window::Window_Interface> Window::Window_Interface::create_window( Window_Type type, const InitializationInfo& init_info )
{
	switch ( type )
	{
	case Window::Window_Type::SDL:
		return std::make_shared<SDL_Window_Impl>( init_info );
		break;
	case Window::Window_Type::OpenVR:
		break;
	default:
		break;
	}
	return nullptr;
}
