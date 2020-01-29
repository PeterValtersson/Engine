#include "SDL_Window_Impl.h"
#include <SDL_syswm.h>
#include <ctime>
#include <Utilities/Profiler/Profiler.h>

Window::SDL_Window_Impl::SDL_Window_Impl( const InitializationInfo& init_info ) : curMouseX( 0 ), curMouseY( 0 ), relMouseX( 0 ), relMouseY( 0 ), init_info( init_info ), window( nullptr )
{
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
		throw std::exception( "Failed to initialize SDL subsystem" );
	uint32_t createFlags = SDL_WINDOW_SHOWN | ( init_info.fullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0 );
	window = SDL_CreateWindow( init_info.windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, init_info.resolution.width, init_info.resolution.height, createFlags );
	if ( window == nullptr )
		throw std::exception( "Failed to create window." );

	/* Annoying to type abstraction of SDL... */
	keyMapping = {
		{ KeyCode::Key0, SDLK_0},
		{ KeyCode::Key1, SDLK_1 },
		{ KeyCode::Key2, SDLK_2 },
		{ KeyCode::Key3, SDLK_3 },
		{ KeyCode::Key4, SDLK_4 },
		{ KeyCode::Key5, SDLK_5 },
		{ KeyCode::Key6, SDLK_6 },
		{ KeyCode::Key7, SDLK_7 },
		{ KeyCode::Key8, SDLK_8 },
		{ KeyCode::Key9, SDLK_9 },
		{ KeyCode::KeyA, SDLK_a },
		{ KeyCode::KeyB, SDLK_b },
		{ KeyCode::KeyC, SDLK_c },
		{ KeyCode::KeyD, SDLK_d },
		{ KeyCode::KeyE, SDLK_e },
		{ KeyCode::KeyF, SDLK_f },
		{ KeyCode::KeyG, SDLK_g },
		{ KeyCode::KeyH, SDLK_h },
		{ KeyCode::KeyI, SDLK_i },
		{ KeyCode::KeyJ, SDLK_j },
		{ KeyCode::KeyK, SDLK_k },
		{ KeyCode::KeyL, SDLK_l },
		{ KeyCode::KeyM, SDLK_m },
		{ KeyCode::KeyN, SDLK_n },
		{ KeyCode::KeyO, SDLK_o },
		{ KeyCode::KeyP, SDLK_p },
		{ KeyCode::KeyQ, SDLK_q },
		{ KeyCode::KeyR, SDLK_r },
		{ KeyCode::KeyS, SDLK_s },
		{ KeyCode::KeyT, SDLK_t },
		{ KeyCode::KeyU, SDLK_u },
		{ KeyCode::KeyV, SDLK_v },
		{ KeyCode::KeyW, SDLK_w },
		{ KeyCode::KeyX, SDLK_x },
		{ KeyCode::KeyY, SDLK_y },
		{ KeyCode::KeyEscape, SDLK_ESCAPE},
		{ KeyCode::KeyF1, SDLK_F1 },
		{ KeyCode::KeyF2, SDLK_F2 },
		{ KeyCode::KeyF3, SDLK_F3 },
		{ KeyCode::KeyF4, SDLK_F4 },
		{ KeyCode::KeyF5, SDLK_F5 },
		{ KeyCode::KeyF6, SDLK_F6 },
		{ KeyCode::KeyF7, SDLK_F7},
		{ KeyCode::KeyF8, SDLK_F8 },
		{ KeyCode::KeyF9, SDLK_F9 },
		{ KeyCode::KeyF10, SDLK_F10 },
		{ KeyCode::KeyF11, SDLK_F11 },
		{ KeyCode::KeyF12, SDLK_F12 },
		{ KeyCode::KeySpace, SDLK_SPACE },
		{ KeyCode::KeyLeft, SDLK_LEFT},
		{ KeyCode::KeyRight, SDLK_RIGHT },
		{ KeyCode::KeyUp, SDLK_UP },
		{ KeyCode::KeyDown, SDLK_DOWN },
		{ KeyCode::KeyBackSpace, SDLK_BACKSPACE},
		{ KeyCode::KeyTab ,SDLK_TAB},
		{ KeyCode::KeyReturn, SDLK_RETURN},
		{ KeyCode::KeyCapsLock, SDLK_CAPSLOCK},
		{ KeyCode::KeyShiftL,   SDLK_LSHIFT},
		{ KeyCode::KeyCtrlL,	   SDLK_LCTRL},
		{ KeyCode::KeyAltL,	   SDLK_LALT},
		{ KeyCode::KeyAltR,	   SDLK_RALT},
		{ KeyCode::KeyCtrlR,	   SDLK_RCTRL},
		{ KeyCode::KeyShiftR,   SDLK_RSHIFT},
		{ KeyCode::MouseLeft, SDL_BUTTON_LEFT},
		{ KeyCode::MouseRight, SDL_BUTTON_RIGHT}
	};


	std::srand( std::time( nullptr ) );
	timer.Tick();
}

Window::SDL_Window_Impl::~SDL_Window_Impl()
{
	SDL_Quit();
}

void Window::SDL_Window_Impl::Frame() noexcept
{
	PROFILE;
	timer.Tick();
	for ( auto& ks : actionToKeyState )
	{
		ks.second = ( ks.second & KeyState::DOWN );
	}
	SDL_Event ev;
	while ( SDL_PollEvent( &ev ) )
	{
		/*for ( auto& onEvent : onEventCallbacks )
			onEvent( &ev, SE::Window::WindowImplementation::WINDOW_IMPLEMENTATION_SDL );*/
		EventSwitch( ev );
	}
}

void* Window::SDL_Window_Impl::GetWindowHandle() noexcept
{
	PROFILE;
	SDL_SysWMinfo sdlInfo;
	SDL_VERSION( &sdlInfo.version );
	SDL_GetWindowWMInfo( window, &sdlInfo );
	return sdlInfo.info.win.window;
}

void* Window::SDL_Window_Impl::GetWindowImplementation() noexcept
{
	return window;
}

bool Window::SDL_Window_Impl::ButtonDown( ActionButton actionButton ) const noexcept
{
	return flag_has( GetKeyState( actionButton ), KeyState::DOWN );
}

bool Window::SDL_Window_Impl::ButtonPressed( ActionButton actionButton ) const noexcept
{
	auto state = GetKeyState( actionButton );
	return !( state ^ KeyState::PRESSED ) || *( state & KeyState::DOUBLE );
}

bool Window::SDL_Window_Impl::ButtonUp( ActionButton actionButton ) const noexcept
{
	return *( GetKeyState( actionButton ) & KeyState::UP );
}

bool Window::SDL_Window_Impl::ButtonDouble( ActionButton actionButton ) const noexcept
{
	return *( GetKeyState( actionButton ) & KeyState::DOUBLE );
}

void Window::SDL_Window_Impl::GetMousePos( int& x, int& y ) const noexcept
{
	x = curMouseX;
	y = curMouseY;
}

void Window::SDL_Window_Impl::ToggleCursor( bool on ) noexcept
{
	if ( on )
		while ( ShowCursor( on ) < 0 ) {}
	else
		while ( ShowCursor( on ) >= 0 ) {}
}

void Window::SDL_Window_Impl::SetWindowTitle( const std::string& title ) noexcept
{
	init_info.windowTitle = title;
	SDL_SetWindowTitle( window, title.c_str() );
}

Window::Resolution Window::SDL_Window_Impl::GetResolution() const noexcept
{
	return init_info.resolution;
}

void Window::SDL_Window_Impl::MapActionButton( ActionButton actionButton, KeyCode key )
{
	keyToAction[keyMapping[key]].push_back( actionButton );
}

void Window::SDL_Window_Impl::SetWindowInfo( const InitializationInfo& info )
{
	bool changed = false;
	if ( init_info.resolution.height != info.resolution.height )
	{
		init_info.resolution.height = info.resolution.height;
		changed = true;
	}
	if ( init_info.resolution.width != info.resolution.width )
	{
		init_info.resolution.width = info.resolution.width;
		changed = true;
	}
	if ( init_info.fullScreen != info.fullScreen )
	{
		init_info.fullScreen = info.fullScreen;
		uint32_t createFlags = SDL_WINDOW_SHOWN | ( init_info.fullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0 );
		SDL_SetWindowFullscreen( window, createFlags );
	}

	if ( changed == true && init_info.fullScreen == false )
	{
		SDL_SetWindowSize( window, init_info.resolution.width, init_info.resolution.height );
	}
}

float Window::SDL_Window_Impl::GetDelta() const noexcept
{
	return 0.0f;
}

void Window::SDL_Window_Impl::EventSwitch( SDL_Event ev ) noexcept
{
	switch ( ev.type )
	{
	case SDL_KEYUP:	// if type is KeyUp
	{
		const auto state = keyToAction.find( ev.key.keysym.sym );
		if ( state != keyToAction.end() )	// if key is bound sets its state
		{
			for ( auto& k : state->second )
			{
				actionToKeyState[k] = KeyState::UP;
				actionToKeyStateLastTime[k] = std::chrono::high_resolution_clock::now();
			}

		}

		break;
	}
	case SDL_KEYDOWN:	// if type is KeyDown
	{
		auto nt = std::chrono::high_resolution_clock::now();

		const auto state = keyToAction.find( ev.key.keysym.sym );
		if ( state != keyToAction.end() )	// if key is bound sets its state
		{
			for ( auto& k : state->second )
			{
				if ( !( actionToKeyState[k] & KeyState::DOWN ) )
				{
					actionToKeyState[k] = KeyState::PRESSED;

					auto const find = actionToKeyStateLastTime.find( k );
					if ( find != actionToKeyStateLastTime.end() )
					{
						auto diff = std::chrono::duration<float, std::milli>( nt - actionToKeyStateLastTime[k] ).count();
						if ( diff < 300 )
						{
							actionToKeyState[k] |= KeyState::DOUBLE;
						}
					}

					//auto pressCallbacks = actionToKeyPressCallback.find( k );
					//if ( pressCallbacks != actionToKeyPressCallback.end() )
					//{
					//	for ( auto& cb : pressCallbacks->second )
					//		cb();
					//}
				}
				/*auto downCallbacks = actionToKeyDownCallback.find( k );
				if ( downCallbacks != actionToKeyDownCallback.end() )
				{
					for ( auto& cb : downCallbacks->second )
						cb();
				}*/
			}

		}
		break;
	}
	case SDL_MOUSEMOTION:	// if type is MouseMotion
	{
		curMouseX = ev.motion.x;
		curMouseY = ev.motion.y;
		relMouseX = ev.motion.xrel;
		relMouseY = ev.motion.yrel;
	/*	for ( auto& cb : mouseMotionCallbacks )
			cb( relMouseX, relMouseY, curMouseX, curMouseY );*/
		break;
	}
	case SDL_MOUSEBUTTONDOWN:	// if type is MouseButtonDown
	{
		if ( ev.button.button == SDL_BUTTON_LEFT )
		{

			mouseLeftDown = true;
		}
		else if ( ev.button.button == SDL_BUTTON_RIGHT )
		{
			mouseRightDown = true;
		}
		const auto state = keyToAction.find( ev.button.button );
		if ( state != keyToAction.end() )
		{
			for ( auto& k : state->second )
				if ( !( actionToKeyState[k] & KeyState::DOWN ) )
				{
					actionToKeyState[k] = KeyState::PRESSED;
				}
		}
		break;
	}
	case SDL_MOUSEBUTTONUP:	// if type is MouseButtonUP
	{
		const auto state = keyToAction.find( ev.button.button );
		if ( state != keyToAction.end() )
		{
			for ( auto& k : state->second )
			{
				/*auto mouseClickCallbacks = actionToMouseClickCallback.find( k );
				if ( mouseClickCallbacks != actionToMouseClickCallback.end() )
				{
					for ( auto& cb : mouseClickCallbacks->second )
						cb( curMouseX, curMouseY );
				}*/
				actionToKeyState[k] = KeyState::UP;
			}



		}
		if ( ev.button.button == SDL_BUTTON_LEFT )
		{
			mouseLeftDown = false;
		}
		else if ( ev.button.button == SDL_BUTTON_RIGHT )
		{
			mouseRightDown = false;
		}
		break;
	}
	default:
	{
		break;
	}
	}
}

Window::KeyState Window::SDL_Window_Impl::GetKeyState( ActionButton actionButton ) const noexcept
{
	//Find which KeyCode actionbutton is mapped to
	if ( const auto k = actionToKeyState.find( actionButton ); k != actionToKeyState.end() )
		return k->second;

	return KeyState::NIL;

}
