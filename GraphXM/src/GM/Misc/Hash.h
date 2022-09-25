#pragma once

namespace GM
{
    /*
    *   Calculates the hash of a given value
    */
    template<typename T>
    std::size_t Hash(T const & v)
    {
        return std::hash<T>{}(v);
    }

    /*
        Combines the hash of a given value with the given seed
        Reference: boost::hash_combine
    */
    template<typename T>
    std::size_t Hash_Combine(std::size_t& seed, const T& v)
    {
        std::size_t hV = Hash<T>(v);
		return Hash_Combine(seed, hV);
    }

	template<>
	inline std::size_t Hash_Combine<std::size_t>(std::size_t& seed, const std::size_t& value)
	{
		const uint64_t m = (uint64_t(0xc6a4a793) << 32) + 0x5bd1e995;
		const int r = 47;

		std::size_t hV = value;
		hV *= m;
		hV ^= hV >> r;
		hV *= m;

		seed ^= hV;
		seed *= m;

		// Completely arbitrary number, to prevent 0's
		// from hashing to 0.
		seed += 0xe6546b64;

		return seed;
	}
}

// std::hash specializations for vectors
namespace std
{
	template<>
	struct hash<GM::Vector2>
	{
		std::size_t operator()(const GM::Vector2& v) const noexcept
		{
			std::size_t seed = 0;
			GM::Hash_Combine(seed, v.x);
			GM::Hash_Combine(seed, v.y);
			return seed;
		}
	};

	template<>
	struct hash<GM::Vector3>
	{
		std::size_t operator()(const GM::Vector3& v) const noexcept
		{
			std::size_t seed = 0;
			GM::Hash_Combine(seed, v.x);
			GM::Hash_Combine(seed, v.y);
			GM::Hash_Combine(seed, v.z);
			return seed;
		}
	};

	template<>
	struct hash<GM::Vector4>
	{
		std::size_t operator()(const GM::Vector4& v) const noexcept
		{
			std::size_t seed = 0;
			GM::Hash_Combine(seed, v.x);
			GM::Hash_Combine(seed, v.y);
			GM::Hash_Combine(seed, v.z);
			GM::Hash_Combine(seed, v.w);
			return seed;
		}
	};

	template<>
	struct hash<GM::IntVector3>
	{
		std::size_t operator()(const GM::IntVector3& v) const noexcept
		{
			std::size_t seed = 0;
			GM::Hash_Combine(seed, v.x);
			GM::Hash_Combine(seed, v.y);
			GM::Hash_Combine(seed, v.z);
			return seed;
		}
	};


	template<>
	struct hash<GM::IntVector2>
	{
		std::size_t operator()(const GM::IntVector2& v) const noexcept
		{
			std::size_t seed = 0;
			GM::Hash_Combine(seed, v.x);
			GM::Hash_Combine(seed, v.y);
			return seed;
		}
	};
}