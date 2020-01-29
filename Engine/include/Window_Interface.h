#ifndef _WINDOW_INTERFACE_H_
#define _WINDOW_INTERFACE_H_

#pragma once
#include "DLL_Export_Window.h"
#include <memory>
#include <stdint.h>
#include <string>

namespace Window
{
	struct Resolution{
		uint32_t width;
		uint32_t height;
	};
	enum class Window_Type{
		SDL,
		OpenVR
	};

	struct InitializationInfo{
		std::string windowTitle = "";
		bool fullScreen = false;
		Resolution resolution = { 1280, 720 };
		void* pNext = nullptr;
	};
	typedef uint32_t ActionButton; // Defined by the user.

	enum KeyCode{
		KeyEscape,
		KeyF1,
		KeyF2,
		KeyF3,
		KeyF4,
		KeyF5,
		KeyF6,
		KeyF7,
		KeyF8,
		KeyF9,
		KeyF10,
		KeyF11,
		KeyF12,
		KeyF13,
		KeyF14,
		KeyF15,
		KeyF16,
		KeyF17,
		KeyF18,
		KeyF19,
		KeyPrint,
		KeyScrollLock,
		KeyBreak,

		KeySpace = 0x0020,
/*
			KeyApostrophe = 0x0027,
			KeyComma = 0x002c,
			KeyMinus = 0x002d,
			KeyPeriod = 0x002e,
			KeySlash = 0x002f,
			*/
			Key0 = 0x0030,
			Key1 = 0x0031,
			Key2 = 0x0032,
			Key3 = 0x0033,
			Key4 = 0x0034,
			Key5 = 0x0035,
			Key6 = 0x0036,
			Key7 = 0x0037,
			Key8 = 0x0038,
			Key9 = 0x0039,
/*
			KeySemicolon = 0x003b,
			KeyLess = 0x003c,
			KeyEqual = 0x003d,
			*/
			KeyA = 0x0041,
			KeyB = 0x0042,
			KeyC = 0x0043,
			KeyD = 0x0044,
			KeyE = 0x0045,
			KeyF = 0x0046,
			KeyG = 0x0047,
			KeyH = 0x0048,
			KeyI = 0x0049,
			KeyJ = 0x004a,
			KeyK = 0x004b,
			KeyL = 0x004c,
			KeyM = 0x004d,
			KeyN = 0x004e,
			KeyO = 0x004f,
			KeyP = 0x0050,
			KeyQ = 0x0051,
			KeyR = 0x0052,
			KeyS = 0x0053,
			KeyT = 0x0054,
			KeyU = 0x0055,
			KeyV = 0x0056,
			KeyW = 0x0057,
			KeyX = 0x0058,
			KeyY = 0x0059,
			KeyZ = 0x005a,
/*
			KeyBracketLeft = 0x005b,
			KeyBackslash = 0x005c,
			KeyBracketRight = 0x005d,
			KeyGrave = 0x0060,
			*/
			KeyLeft,
			KeyRight,
			KeyUp,
			KeyDown,
			/*
			KeyInsert,
			KeyHome,
			KeyDelete,
			KeyEnd,
			KeyPageUp,
			KeyPageDown,

			KeyNumLock,
			KeyKpEqual,
			KeyKpDivide,
			KeyKpMultiply,
			KeyKpSubtract,
			KeyKpAdd,
			KeyKpEnter,
			KeyKpInsert, // 0
			KeyKpEnd, // 1
			KeyKpDown, // 2
			KeyKpPageDown, // 3
			KeyKpLeft, // 4
			KeyKpBegin, // 5
			KeyKpRight, // 6
			KeyKpHome, // 7
			KeyKpUp, // 8
			KeyKpPageUp, // 9
			KeyKpDelete, // ,
			*/
			KeyBackSpace,
			KeyTab,
			KeyReturn,
			KeyCapsLock,
			KeyShiftL,
			KeyCtrlL,
			//KeySuperL,
			KeyAltL,
			KeyAltR,
		//	KeySuperR,
		//	KeyMenu,
		KeyCtrlR,
		KeyShiftR,
/*
			KeyBack,
			KeySoftLeft,
			KeySoftRight,
			KeyCall,
			KeyEndcall,
			KeyStar,
			KeyPound,
			KeyDpadCenter,
			KeyVolumeUp,
			KeyVolumeDown,
			KeyPower,
			KeyCamera,
			KeyClear,
			KeySymbol,
			KeyExplorer,
			KeyEnvelope,
			KeyEquals,
			KeyAt,
			KeyHeadsethook,
			KeyFocus,
			KeyPlus,
			KeyNotification,
			KeySearch,
			KeyMediaPlayPause,
			KeyMediaStop,
			KeyMediaNext,
			KeyMediaPrevious,
			KeyMediaRewind,
			KeyMediaFastForward,
			KeyMute,
			KeyPictsymbols,
			KeySwitchCharset,
			KeyForward,
			KeyExtra1,
			KeyExtra2,
			KeyExtra3,
			KeyExtra4,
			KeyExtra5,
			KeyExtra6,
			KeyFn,
			KeyCircumflex,
			KeySsharp,
			KeyAcute,
			KeyAltGr,
			KeyNumbersign,
			KeyUdiaeresis,
			KeyAdiaeresis,
			KeyOdiaeresis,
			KeySection,
			KeyAring,
			KeyDiaeresis,
			KeyTwosuperior,
			KeyRightParenthesis,
			KeyDollar,
			KeyUgrave,
			KeyAsterisk,
			KeyColon,
			KeyExclam,
			KeyBraceLeft,
			KeyBraceRight,
			KeySysRq,
			*/
			MouseLeft,
			MouseRight,
			KeyCount_
	};

	class Window_Interface{
	public:
		DECLSPEC_WINDOW static std::shared_ptr<Window_Interface> create_window( Window_Type type, const InitializationInfo& init_info );

		/**
			* @brief Polls for input
			*/
		virtual void Frame()noexcept = 0;

		/**
		* @brief Returns a pointer to the window handle
		* @retval ptr Valid pointer on success
		* @retval nullptr Nullptr if no such handle.
		*/
		virtual void* GetWindowHandle()noexcept = 0;

		/**
		* @brief Returns a pointer to the implementation specific window. In the case of SDL, it returns a SDL_Window*.
		* @param[in] implementation The implementation to get the window pointer from.
		* @retval ptr Valid pointer on success
		* @retval nullptr Nullptr if no such window exists.
		*/
		virtual void* GetWindowImplementation( )noexcept = 0;

		/**
			* @brief Checks whether or no the bound action button is down or not. An action button must be bound with MapActionButton before this method is called
			* @retval true Returns true if the button is down
			* @retval false Returns false if the button is not down.
			* @sa MapActionButton
			*/
		virtual bool ButtonDown( ActionButton actionButton ) const noexcept = 0;
		/**
		* @brief Checks whether or no the bound action button was pressed during this frame or not, i.e. if the button is down this frame but was not down last frame.
		* @retval true Returns true if the button was pressed.
		* @retval false Returns false if the button was not pressed.
		* @sa MapActionButton
		*/
		virtual bool ButtonPressed( ActionButton actionButton ) const noexcept = 0;
		/**
		* @brief Checks whether or no the bound action button was lifted this frame
		* @retval true Returns true if the button was lifted
		* @retval false Returns false if the button was not lifted
		* @sa MapActionButton
		*/
		virtual bool ButtonUp( ActionButton actionButton ) const noexcept = 0;
		/**
		* @brief Checks whether or not the bound action button was double pressed
		* @retval true Returns true if the button was double pressed
		* @retval false Returns false if the button was not double pressed
		* @sa MapActionButton
		*/
		virtual bool ButtonDouble( ActionButton actionButton ) const noexcept = 0;

		/**
		* @brief Retrieves the mouse pos on the window.
		* @parameter[out] x The x position
		* @parameter[out] y The y position
		*/
		virtual void GetMousePos( int& x, int& y ) const noexcept = 0;

		/**
			* @brief Toggles the cursor on/off
			*/
		virtual void ToggleCursor( bool on ) noexcept = 0;

		/*
		 * @brief Sets the title of the window
		 */
		virtual void SetWindowTitle( const std::string& title ) noexcept = 0;

		/*
		 * @brief Returns the resolution of the window.
		 */
		virtual Resolution GetResolution() const noexcept = 0;
	

		/*@TODO Add method to unregister a single callback. Needs operator== for delegates for that. (OR returning handles to the callback but that's cumbersome)*/

		/**
		* @brief Maps an action button to a certain key. An action button can be bound to several keys, but one key can only be bound to one action button. An action button is any user defined key represented as an unsigned integer.
		* * Example code:
		* @code
		*   enum{
		*       JUMP,
		*       SHOOT
		*   };
		*
		*   window.MapActionButton(JUMP, KeySpace);
		*   window.Frame();
		*   if(window.ButtonDown(JUMP))
		*        character.jump();
		* @endcode
		*/
		virtual void MapActionButton( ActionButton actionButton, KeyCode key ) = 0;
		/**
		* @brief Sets the window data
		*
		* @retval true Returns true if changes has been done to size
		* @retval false Returns false if no changed to size
		*/
		virtual bool SetWindowInfo( const InitializationInfo& info ) = 0;

		/**
		* @brief Gets delta time from window
		*
		* @retval float Returns frame delta time in seconds
		*/
		virtual float GetDelta() const noexcept= 0;
	};

}

#endif