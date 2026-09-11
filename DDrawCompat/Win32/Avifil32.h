#pragma once

namespace Win32
{
	namespace Avifil32
	{
		void installHooks();
		bool isIntroVideoActive();
		LONG getIntroVideoSample();
	}
}
