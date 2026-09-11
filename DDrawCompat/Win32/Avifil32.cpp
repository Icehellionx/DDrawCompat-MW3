#include <mutex>
#include <unordered_map>

#include <Windows.h>
#include <vfw.h>

#include <Common/Hook.h>
#include <Common/Log.h>
#include <Config/Settings/RemasterIntroChromaCleanup.h>
#include <Config/Settings/RemasterIntroWidescreen.h>
#include <Win32/Avifil32.h>

namespace
{
	using AviStreamOpenFromFileAProc = HRESULT(WINAPI*)(
		PAVISTREAM*, LPCSTR, DWORD, LONG, UINT, CLSID*);
	using AviStreamReleaseProc = ULONG(WINAPI*)(PAVISTREAM);
	using AviStreamReadProc = HRESULT(WINAPI*)(PAVISTREAM, LONG, LONG, LPVOID, LONG, LONG*, LONG*);

	AviStreamOpenFromFileAProc g_origAviStreamOpenFromFileA = nullptr;
	AviStreamReleaseProc g_origAviStreamRelease = nullptr;
	AviStreamReadProc g_origAviStreamRead = nullptr;
	std::mutex g_introStreamMutex;
	std::unordered_map<PAVISTREAM, DWORD> g_introStreams;
	LONG g_introVideoSample = 0;

	bool isIntroPath(LPCSTR path)
	{
		if (!path)
		{
			return false;
		}
		const char* fileName = path;
		for (const char* p = path; *p; ++p)
		{
			if ('\\' == *p || '/' == *p)
			{
				fileName = p + 1;
			}
		}
		return 0 == _stricmp(fileName, "INTRO.AVI");
	}

	HRESULT WINAPI aviStreamOpenFromFileA(PAVISTREAM* stream, LPCSTR fileName,
		DWORD streamType, LONG handler, UINT mode, CLSID* handlerClass)
	{
		const HRESULT result = g_origAviStreamOpenFromFileA(
			stream, fileName, streamType, handler, mode, handlerClass);
		if (SUCCEEDED(result) && stream && *stream && isIntroPath(fileName))
		{
			std::lock_guard<std::mutex> lock(g_introStreamMutex);
			if (g_introStreams.emplace(*stream, streamType).second)
			{
				LOG_INFO << "MW3 Remaster: identified INTRO.AVI for 16:9 center crop";
			}
		}
		return result;
	}

	ULONG WINAPI aviStreamRelease(PAVISTREAM stream)
	{
		const ULONG result = g_origAviStreamRelease(stream);
		if (0 == result && stream)
		{
			std::lock_guard<std::mutex> lock(g_introStreamMutex);
			g_introStreams.erase(stream);
			if (g_introStreams.empty())
			{
				g_introVideoSample = 0;
			}
		}
		return result;
	}

	HRESULT WINAPI aviStreamRead(PAVISTREAM stream, LONG start, LONG samples,
		LPVOID buffer, LONG bufferSize, LONG* bytesRead, LONG* samplesRead)
	{
		const HRESULT result = g_origAviStreamRead(
			stream, start, samples, buffer, bufferSize, bytesRead, samplesRead);
		if (SUCCEEDED(result) && stream && 0 <= start)
		{
			std::lock_guard<std::mutex> lock(g_introStreamMutex);
			const auto it = g_introStreams.find(stream);
			if (it != g_introStreams.end() &&
				(streamtypeVIDEO == it->second || (0 == it->second && 1 == samples)))
			{
				g_introVideoSample = start;
			}
		}
		return result;
	}
}

namespace Win32
{
	namespace Avifil32
	{
		void installHooks()
		{
			if (!Config::remasterIntroWidescreen.get() && !Config::remasterIntroChromaCleanup.get())
			{
				return;
			}
			Compat::hookFunction("avifil32", "AVIStreamOpenFromFileA",
				reinterpret_cast<void*&>(g_origAviStreamOpenFromFileA),
				reinterpret_cast<void*>(&aviStreamOpenFromFileA));
			Compat::hookFunction("avifil32", "AVIStreamRelease",
				reinterpret_cast<void*&>(g_origAviStreamRelease),
				reinterpret_cast<void*>(&aviStreamRelease));
			Compat::hookFunction("avifil32", "AVIStreamRead",
				reinterpret_cast<void*&>(g_origAviStreamRead),
				reinterpret_cast<void*>(&aviStreamRead));
		}

		bool isIntroVideoActive()
		{
			std::lock_guard<std::mutex> lock(g_introStreamMutex);
			return !g_introStreams.empty();
		}

		LONG getIntroVideoSample()
		{
			std::lock_guard<std::mutex> lock(g_introStreamMutex);
			return g_introVideoSample;
		}
	}
}
