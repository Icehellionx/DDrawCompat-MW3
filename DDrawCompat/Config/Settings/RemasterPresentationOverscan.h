#pragma once

#include <Config/IntSetting.h>

namespace Config
{
	namespace Settings
	{
		class RemasterPresentationOverscan : public IntSetting
		{
		public:
			RemasterPresentationOverscan()
				: IntSetting("PresentationEdgeRepair", "0", 0, 12)
			{
			}
		};
	}

	extern Settings::RemasterPresentationOverscan remasterPresentationOverscan;
}
