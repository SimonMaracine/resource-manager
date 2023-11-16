#pragma once

#include <cstdint>
#include <cstddef>
#include <string>

// Implements the Fowler-Noll-Vo hash algorithm, or more exactly FNV-1a

namespace resmanager {
    namespace internal {
        struct Variant32 {
            static constexpr std::uint32_t FNV_OFFSET_BASIS = 2166136261u;
            static constexpr std::uint32_t FNV_PRIME = 16777619u;
        };

        struct Variant64 {
            static constexpr std::uint64_t FNV_OFFSET_BASIS = 14695981039346656037u;
            static constexpr std::uint64_t FNV_PRIME = 1099511628211u;
        };

        template<typename T, typename V>
        class HashedStr {
        public:
            constexpr HashedStr() noexcept;
            explicit constexpr HashedStr(const char* string) noexcept;  // TODO should be consteval, but we don't have C++20 yet
            explicit HashedStr(const std::string& string) noexcept;

            constexpr operator T() const noexcept { return hash; }
            constexpr bool operator==(const HashedStr other) const noexcept;
        private:
            static constexpr T fnv1a(const char* string) noexcept;

            T hash;
        };

        template<typename T, typename V>
        constexpr HashedStr<T, V>::HashedStr() noexcept
            : hash(0u) {}

        template<typename T, typename V>
        constexpr HashedStr<T, V>::HashedStr(const char* string) noexcept
            : hash(fnv1a(string)) {}

        template<typename T, typename V>
        inline HashedStr<T, V>::HashedStr(const std::string& string) noexcept
            : hash(fnv1a(string.c_str())) {}

        template<typename T, typename V>
        constexpr bool HashedStr<T, V>::operator==(const HashedStr other) const noexcept {
            return hash == other.hash;
        }

        template<typename T, typename V>
        constexpr T HashedStr<T, V>::fnv1a(const char* string) noexcept {
            T hash = V::FNV_OFFSET_BASIS;

            for (std::size_t i = 0; string[i] != '\0'; i++) {
                hash ^= static_cast<T>(string[i]);
                hash *= V::FNV_PRIME;
            }

            return hash;
        }
    }

    using HashedStr32 = internal::HashedStr<std::uint32_t, internal::Variant32>;
    using HashedStr64 = internal::HashedStr<std::uint64_t, internal::Variant64>;

    namespace literals {
        constexpr HashedStr32 operator""_h(const char* string, std::size_t) noexcept {
            return HashedStr32(string);
        }

        constexpr HashedStr64 operator""_H(const char* string, std::size_t) noexcept {
            return HashedStr64(string);
        }
    }

    /* FIXME
        might be wrong casting uint32_t to std::size_t (64 bits)
        IT IS WRONG casting uint64_t when std::size_t is 32 bits in size
    */
    template<typename V>
    struct Hash {
        constexpr std::size_t operator()(V hashed_string) const noexcept {
            static_assert(sizeof(V) <= sizeof(std::size_t));

            return static_cast<std::size_t>(hashed_string);
        }
    };
}
