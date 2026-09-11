#pragma once

#include <Config/BoolSetting.h>

namespace Config
{
	namespace Settings
	{
		class RemasterStartupSurfaceClear : public BoolSetting
		{
		public:
			RemasterStartupSurfaceClear()
				: BoolSetting("RemasterStartupSurfaceClear", "off")
			{
			}
		};
	}

	extern Settings::RemasterStartupSurfaceClear remasterStartupSurfaceClear;
}
