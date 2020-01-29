#ifndef _WINDOW_INTERFACE_H_
#define _WINDOW_INTERFACE_H_

#pragma once
#include "DLL_Export_Window.h"
#include <memory>
#include <stdint.h>

namespace Window
{
	struct Resolution{
		uint32_t width;
		uint32_t height;
	};
	struct WindowCreationInfo{
		Resolution resolution = { 800, 640 };
		bool windowed_fullscreen = false;
		void* pNext = nullptr;
	};
	enum class Window_Type{
		SDL,
		OpenVR
	};
	class Window_Interface{
	public:
		DECLSPEC_WINDOW static std::shared_ptr<Window_Interface> create_window( Window_Type type, const WindowCreationInfo& init_info );
	};

}

#endif