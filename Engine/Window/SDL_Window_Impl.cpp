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
	return GetKeyState( actionButton ) & KeyState::DOWN;
}

bool Window::SDL_Window_Impl::ButtonPressed( ActionButton actionButton ) const noexcept
{
	return false;
}

bool Window::SDL_Window_Impl::ButtonUp( ActionButton actionButton ) const noexcept
{
	return false;
}

bool Window::SDL_Window_Impl::ButtonDouble( ActionButton actionButton ) const noexcept
{
	return false;
}

void Window::SDL_Window_Impl::GetMousePos( int& x, int& y ) const noexcept
{}

void Window::SDL_Window_Impl::ToggleCursor( bool on ) noexcept
{}

void Window::SDL_Window_Impl::SetWindowTitle( const std::string& title ) noexcept
{}

Resolution Window::SDL_Window_Impl::GetResolution() const noexcept
{
	return Resolution();
}

void Window::SDL_Window_Impl::MapActionButton( ActionButton actionButton, KeyCode key )
{}

bool Window::SDL_Window_Impl::SetWindowInfo( const InitializationInfo& info )
{
	return false;
}

float Window::SDL_Window_Impl::GetDelta() const noexcept
{
	return 0.0f;
}

void Window::SDL_Window_Impl::EventSwitch( SDL_Event ev ) noexcept
{}

uint32_t Window::SDL_Window_Impl::GetKeyState( ActionButton actionButton ) const noexcept
{
	return uint32_t();
}
