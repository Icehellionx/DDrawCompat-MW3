#pragma once

#include <Config/BoolSetting.h>

namespace Config
{
	namespace Settings
	{
		class RemasterIntroChromaCleanup : public BoolSetting
		{
		public:
			RemasterIntroChromaCleanup()
				: BoolSetting("RemasterIntroChromaCleanup", "off")
			{
			}
		};
	}

	extern Settings::RemasterIntroChromaCleanup remasterIntroChromaCleanup;
}
