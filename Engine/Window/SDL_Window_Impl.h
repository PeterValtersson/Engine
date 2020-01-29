#ifndef _WINDOW_SDL_WINDOW_H_
#define _WINDOW_SDL_WINDOW_H_

#pragma once
#include <Window_Interface.h>
#include <SDL.h>
#include <map>
#include <vector>
#include <chrono>
#include <Utilities/Time/Timer.h>
#include <Utilities/Flags.h>

namespace Window
{
	class SDL_Window_Impl : public Window_Interface{
	public:
		SDL_Window_Impl( const InitializationInfo& init_info );
		virtual ~SDL_Window_Impl();

		virtual void Frame()noexcept override;

		virtual void* GetWindowHandle()noexcept override;

		virtual void* GetWindowImplementation()noexcept override;

		virtual bool ButtonDown( ActionButton actionButton ) const noexcept override;

		virtual bool ButtonPressed( ActionButton actionButton ) const noexcept override;

		virtual bool ButtonUp( ActionButton actionButton ) const noexcept override;

		virtual bool ButtonDouble( ActionButton actionButton ) const noexcept override;

		virtual void GetMousePos( int& x, int& y ) const noexcept override;

		virtual void ToggleCursor( bool on ) noexcept override;

		virtual void SetWindowTitle( const std::string& title ) noexcept override;

		virtual Resolution GetResolution() const noexcept override;

		virtual void MapActionButton( ActionButton actionButton, KeyCode key ) override;

		virtual bool SetWindowInfo( const InitializationInfo& info ) override;

		virtual float GetDelta() const noexcept override;
	private:
		/*
			* @brief Checks which event ev is.
			*/
		void EventSwitch( SDL_Event ev )noexcept;

		/*Window related things*/
		SDL_Window* window;
		InitializationInfo init_info;
		
		/*input related things*/
		int curMouseX;
		int curMouseY;
		int relMouseX;
		int relMouseY;

		bool mouseLeftDown = false;
		bool mouseRightDown = false;

		std::map<uint32_t, std::vector<ActionButton>> keyToAction;
		/**<Maps generic keys defined in IWindow.h to key codes of SDL implementation*/
		std::map<KeyCode, uint32_t> keyMapping;

		enum class KeyState : uint32_t{
			NIL = 0,
			DOWN = 1 << 1,
			PRESSED = 3,
			UP = 4,
			DOUBLE = 1 << 4
		}; 
		ENUM_FLAGS( Window::SDL_Window_Impl::KeyState );

		/**<Maps action button to keystate (up, down, pressed)*/
		std::map<ActionButton, uint32_t> actionToKeyState;
		std::map<ActionButton, std::chrono::high_resolution_clock::time_point> actionToKeyStateLastTime;
		uint32_t GetKeyState( ActionButton actionButton ) const noexcept;

		Utilities::Time::Timer timer;
	};
	
}
#endif