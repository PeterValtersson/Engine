#ifndef _WINDOW_SDL_WINDOW_H_
#define _WINDOW_SDL_WINDOW_H_

#pragma once
#include <HMI.h>
#include <SDL.h>
#include <map>
#include <vector>
#include <chrono>
#include <Utilities/Time/Timer.h>
#include <Utilities/Flags.h>
#include <Utilities/Event.h>

namespace ECSEngine
{
	enum class KeyState : uint32_t {
		NIL = 0,
		DOWN = 1 << 1,
		PRESSED = 3,
		UP = 4,
		DOUBLE = 1 << 4
	};
	ENUM_FLAGS( ECSEngine::KeyState );
	 
	class SDL_Window_Impl : public HMI {
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

		virtual Rectangle GetRectangle() const noexcept override;

		virtual void MapActionButton( ActionButton actionButton, KeyCode key ) override;

		virtual void SetWindowInfo( const InitializationInfo& info ) override;

		virtual float GetDelta() const noexcept override;


		virtual void BindMouseClickCallback( ActionButton actionButton, const MouseClickCallback& callback )noexcept override;
		virtual void UnbindMouseClickCallback( ActionButton actionButton, const MouseClickCallback& callback )noexcept override;

		virtual void BindMouseMotionCallback( const MouseMotionCallback& callback )noexcept override;
		virtual void UnbindMouseMotionCallback( const MouseMotionCallback& callback )noexcept override;

		virtual void BindKeyPressCallback( ActionButton actionButton, const KeyCallback& callback )noexcept override;
		virtual void UnbindKeyPressCallback( ActionButton actionButton, const KeyCallback& callback )noexcept override;

		virtual void BindKeyDownCallback( ActionButton actionButton, const KeyCallback& callback )noexcept override;
		virtual void UnbindKeyDownCallback( ActionButton actionButton, const KeyCallback& callback )noexcept override;

		virtual void BindKeyUpCallback( ActionButton actionButton, const KeyCallback& callback )noexcept override;
		virtual void UnbindKeyUpCallback( ActionButton actionButton, const KeyCallback& callback )noexcept override;

		virtual void UnbindAllCallbacks()noexcept override;

		virtual void BindOnQuitEvent( const QuitCallback& callback )noexcept override;
		virtual void UnbindOnQuitEvent( const QuitCallback& callback )noexcept override;
	private:
		/*
			* @brief Checks which event ev is.
			*/
		void EventSwitch( SDL_Event ev )noexcept;

		/*ECSEngine related things*/
		SDL_Window* hmi;
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

		

		/**<Maps action button to keystate (up, down, pressed)*/
		std::map<ActionButton, KeyState> actionToKeyState;
		std::map<ActionButton, std::chrono::high_resolution_clock::time_point> actionToKeyStateLastTime;
		KeyState GetKeyState( ActionButton actionButton ) const noexcept;

		Utilities::Time::Timer timer;
	

		/* Callbacks */
		std::map<ActionButton, Utilities::Event<KeyCallback::Callback_Signature>> actionToKeyPressCallback;
		std::map<ActionButton, Utilities::Event<KeyCallback::Callback_Signature>> actionToKeyDownCallback;
		std::map<ActionButton, Utilities::Event<KeyCallback::Callback_Signature>> actionToKeyUpCallback;
		std::map<ActionButton, Utilities::Event<MouseClickCallback::Callback_Signature>> actionToMouseClickCallback;
		Utilities::Event<MouseMotionCallback::Callback_Signature> mouseMotionCallbacks;
		Utilities::Event<QuitCallback::Callback_Signature> quitEvent;
	};
	
}
#endif