#pragma once

#include <cstdint>
#include <cstddef>
#include <string>

// Implements the Fowler-Noll-Vo-1a hash algorithm

namespace resmanager {
    namespace internal {
        struct Hash32 {
            static constexpr uint32_t FNV_OFFSET_BASIS = 2166136261u;
            static constexpr uint32_t FNV_PRIME = 16777619u;
        };

        struct Hash64 {
            static constexpr uint64_t FNV_OFFSET_BASIS = 14695981039346656037u;
            static constexpr uint64_t FNV_PRIME = 1099511628211u;
        };

        template<typename T, typename H>
        static constexpr T fnv1a(const char* string) {
            T hash = H::FNV_OFFSET_BASIS;

            for (size_t i = 0; string[i] != '\0'; i++) {
                hash ^= static_cast<uint8_t>(string[i]);
                hash *= H::FNV_PRIME;
            }

            return hash;
        }
    }

    class HashedStr32 {
    public:
        constexpr HashedStr32();
        explicit constexpr HashedStr32(const char* string);  // TODO should use consteval, but were not at C++20 yet
        explicit HashedStr32(const std::string& string);

        constexpr operator uint32_t() const { return hash; }
    private:
        const uint32_t hash;
    };

    constexpr HashedStr32::HashedStr32()
        : hash(0) {}

    constexpr HashedStr32::HashedStr32(const char* string)
        : hash(internal::fnv1a<uint32_t, internal::Hash32>(string)) {}

    inline HashedStr32::HashedStr32(const std::string& string)
        : hash(internal::fnv1a<uint32_t, internal::Hash32>(string.c_str())) {}

    class HashedStr64 {
    public:
        constexpr HashedStr64();
        explicit constexpr HashedStr64(const char* string);
        explicit HashedStr64(const std::string& string);

        constexpr operator uint64_t() const { return hash; }
    private:
        const uint64_t hash;
    };

    constexpr HashedStr64::HashedStr64()
        : hash(0) {}

    constexpr HashedStr64::HashedStr64(const char* string)
        : hash(internal::fnv1a<uint64_t, internal::Hash64>(string)) {}

    inline HashedStr64::HashedStr64(const std::string& string)
        : hash(internal::fnv1a<uint64_t, internal::Hash64>(string.c_str())) {}

    namespace literals {
        constexpr HashedStr32 operator""_h(const char* string, size_t) {
            return HashedStr32 {string};
        }

        constexpr HashedStr64 operator""_H(const char* string, size_t) {
            return HashedStr64 {string};
        }
    }
}
