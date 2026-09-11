#pragma once

#include <Config/BoolSetting.h>

namespace Config
{
	namespace Settings
	{
		class RemasterIntroWidescreen : public BoolSetting
		{
		public:
			RemasterIntroWidescreen()
				: BoolSetting("RemasterIntroWidescreen", "off")
			{
			}
		};
	}

	extern Settings::RemasterIntroWidescreen remasterIntroWidescreen;
}
