#pragma once

namespace GraphX
{
	// Time of the day and Coresponding hours
	namespace DayTime
	{
		enum Type
		{
			GX_START = 0,
			GX_EARLY_MORNING = 3,
			GX_SUNRISE = 6,
			GX_MORNING = 9,
			GX_NOON = 12,
			GX_AFTERNOON = 15,
			GX_EVENING = 18,
			GX_NIGHT = 21
		};
	}

	class EngineUtil
	{
	public:
		/* Returns a byte string from the WideString */
		static std::string ToByteString(const std::wstring& WideString);

		/* Returns a wide string from the ByteString */
		static std::wstring ToWideString(const std::string& ByteString);


		/* Truncates the floating point values down to nearest int */
		template<typename RealType = float>
		static int TruncToInt(RealType val)
		{
			return (int)std::trunc(val);
		}

		/* Resets the seed back to default */
		static void ResetSeed();
		
		/* Returns a random floating point value in range [0, 1) */
		template<typename RealType = double>
		static RealType Rand()
		{
			return std::generate_canonical<RealType, 10>(s_RandNumGenerator);
		}

		/* Returns a random value between 0 and 1 for a particular x and y */
		template<typename RealType = double>
		static double Rand(unsigned int x, unsigned int y, unsigned long long SeedValue = 0)
		{
			s_RandNumGenerator.seed(x * 65655 + y * 43434 + SeedValue);
			return std::generate_canonical<RealType, 10>(s_RandNumGenerator);
		}

		/* Returns a random float value in the range [Min, Max) */
		static float RandRange(float InMin, float InMax)
		{
			return InMin + (InMax - InMin) * Rand<float>();
		}

		/* Returns a random integer value in the range [Min, Max] */
		static int RandRange(int InMin, int InMax)
		{
			const int Range = (InMax - InMin) + 1;
			return Range > 0 ? InMin + TruncToInt<float>(Range * Rand<float>()) : 0;
		}

		/* Returns the file name from the absolute file path */
		static std::string ExtractFileName(const std::string& AbsoluteFilePath);

		/* Returns the file location from the absolute file path */
		static std::string ExtractFileLocation(const std::string& AbsoluteFilePath);

	private:
		/* Random device used to generate random values */
		static std::random_device s_RandDevice;

		/* Random value generator */
		static std::mt19937_64 s_RandNumGenerator;

		/* Convertor to convert between normal and wide strings */
		static std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> s_StringConvertor;
	};
}