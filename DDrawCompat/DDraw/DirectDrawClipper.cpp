#include <vector>

#include <Common/Log.h>
#include <Common/CompatVtable.h>
#include <DDraw/DirectDrawClipper.h>
#include <DDraw/ScopedThreadLock.h>
#include <DDraw/Visitors/DirectDrawClipperVtblVisitor.h>

namespace
{
	HRESULT STDMETHODCALLTYPE SetHWnd(IDirectDrawClipper* This, DWORD flags, HWND hwnd)
	{
		HRESULT result = getOrigVtable(This).SetHWnd(This, flags, hwnd);
		if (FAILED(result))
		{
			LOG_INFO << "MW3 Remaster: DirectDraw clipper window binding failed (HRESULT="
				<< Compat::hex(result) << "); retrying in-process";
			static constexpr DWORD retryDelaysMs[] = { 100, 250, 500, 1000, 2000 };
			for (DWORD retry = 0; retry < _countof(retryDelaysMs); ++retry)
			{
				Sleep(retryDelaysMs[retry]);
				result = getOrigVtable(This).SetHWnd(This, flags, hwnd);
				if (SUCCEEDED(result))
				{
					LOG_INFO << "MW3 Remaster: DirectDraw clipper window binding recovered on retry "
						<< retry + 1;
					break;
				}
			}
			if (FAILED(result))
			{
				LOG_INFO << "MW3 Remaster: DirectDraw clipper window-binding retries exhausted (HRESULT="
					<< Compat::hex(result) << ')';
			}
		}
		return result;
	}

	template<>
	constexpr void setCompatVtable(IDirectDrawClipperVtbl& vtable)
	{
		vtable.SetHWnd = &SetHWnd;
	}
}

namespace DDraw
{
	namespace DirectDrawClipper
	{
		HRGN getClipRgn(CompatRef<IDirectDrawClipper> clipper)
		{
			std::vector<unsigned char> rgnData;
			DWORD size = 0;
			clipper->GetClipList(&clipper, nullptr, nullptr, &size);
			rgnData.resize(size);
			clipper->GetClipList(&clipper, nullptr, reinterpret_cast<RGNDATA*>(rgnData.data()), &size);
			return ExtCreateRegion(nullptr, size, reinterpret_cast<RGNDATA*>(rgnData.data()));
		}

		HRESULT setClipRgn(CompatRef<IDirectDrawClipper> clipper, HRGN rgn)
		{
			std::vector<unsigned char> rgnData;
			rgnData.resize(GetRegionData(rgn, 0, nullptr));
			GetRegionData(rgn, rgnData.size(), reinterpret_cast<RGNDATA*>(rgnData.data()));
			return clipper->SetClipList(&clipper, reinterpret_cast<RGNDATA*>(rgnData.data()), 0);
		}

		void hookVtable(const IDirectDrawClipperVtbl& vtable)
		{
			CompatVtable<IDirectDrawClipperVtbl>::hookVtable<ScopedThreadLock>(vtable);
		}
	}
}
