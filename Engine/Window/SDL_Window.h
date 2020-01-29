#ifndef _WINDOW_SDL_WINDOW_H_
#define _WINDOW_SDL_WINDOW_H_

#pragma once
#include <Window_Interface.h>
namespace Window
{
	class SDL_Window : public Window_Interface{
	public:
		SDL_Window( const WindowCreationInfo& init_info );
	};
}
#endif