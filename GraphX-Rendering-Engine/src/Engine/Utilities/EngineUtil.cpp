#include "pch.h"
#include "EngineUtil.h"

namespace GraphX
{
	std::random_device EngineUtil::s_RandDevice;

	std::mt19937_64 EngineUtil::s_RandNumGenerator(s_RandDevice());

	std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> EngineUtil::s_StringConvertor;

	std::string EngineUtil::ToByteString(const std::wstring& WideString)
	{
		return s_StringConvertor.to_bytes(WideString);
	}

	std::wstring EngineUtil::ToWideString(const std::string& ByteString)
	{
		return s_StringConvertor.from_bytes(ByteString);
	}

	void EngineUtil::ResetSeed()
	{
		s_RandNumGenerator.seed(s_RandNumGenerator.default_seed);
	}

	std::string EngineUtil::ExtractFileName(const std::string& AbsoluteFilePath)
	{
		int LastSlash = AbsoluteFilePath.find_last_of("/\\");
		LastSlash = LastSlash == std::string::npos ? 0 : LastSlash + 1;

		int LastDot = AbsoluteFilePath.rfind(".");
		int count = LastDot == std::string::npos ? AbsoluteFilePath.size() - LastSlash : LastDot - LastSlash;

		return AbsoluteFilePath.substr(LastSlash, count);
	}

	std::string EngineUtil::ExtractFileLocation(const std::string& AbsoluteFilePath)
	{
		int LastSlash = AbsoluteFilePath.find_last_of("/\\");
		if (LastSlash == std::string::npos)
		{
			return std::string();
		}
		else
		{
			return AbsoluteFilePath.substr(0, LastSlash);
		}
	}
}