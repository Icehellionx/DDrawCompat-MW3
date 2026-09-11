#pragma once

#include <Windows.h>

namespace Gdi
{
	namespace PresentationWindow
	{
		HWND create(HWND owner, bool dpiAware = false);
		void setClickToActivate(HWND presentationWindow, bool enable);

		void installHooks();
	}
}
