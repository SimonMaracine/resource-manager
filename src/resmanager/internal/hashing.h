#pragma once

#include <cstdint>
#include <cstddef>
#include <string>

// Implements the Fowler-Noll-Vo-1a hash algorithm

namespace resmanager {
    class hashed_str {
    public:
        explicit constexpr hashed_str(const char* string);
        explicit hashed_str(const std::string& string);

        constexpr operator uint32_t() const { return hash; }
    private:
        constexpr uint32_t fnv1a(const char* string);

        static constexpr uint32_t FNV_OFFSET_BASIS = 2166136261;
        static constexpr uint32_t FNV_PRIME = 16777619;

        const uint32_t hash;
    };

    constexpr hashed_str::hashed_str(const char* string)
        : hash(fnv1a(string)) {}

    inline hashed_str::hashed_str(const std::string& string)
        : hash(fnv1a(string.c_str())) {}

    constexpr uint32_t hashed_str::fnv1a(const char* string) {
        uint32_t hash = FNV_OFFSET_BASIS;

        for (size_t i = 0; string[i] != '\0'; i++) {
            hash ^= static_cast<uint8_t>(string[i]);
            hash *= FNV_PRIME;
        }

        return hash;
    }

    namespace literals {
        constexpr hashed_str operator""_h(const char* string, size_t) {
            return hashed_str {string};
        }
    }
}
