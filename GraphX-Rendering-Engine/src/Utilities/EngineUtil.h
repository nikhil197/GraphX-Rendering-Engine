#pragma once

namespace engine
{
	class EngineUtil
	{
		/* Convertor to convert between normal and wide strings */
		static std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> s_StringConvertor;

	public:
		/* Returns a byte string from the WideString */
		static std::string ToByteString(const std::wstring& WideString);

		/* Returns a wide string from the ByteString */
		static std::wstring ToWideString(const std::string& ByteString);
	};
}