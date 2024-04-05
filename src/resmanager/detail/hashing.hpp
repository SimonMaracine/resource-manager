#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <functional>  // For std::hash

// Implements the Fowler-Noll-Vo hash algorithm, or more exactly FNV-1a

namespace resmanager {
    namespace internal {
        struct Variant32 {
            using Type = std::uint32_t;
            static constexpr std::uint32_t FNV_OFFSET_BASIS {2166136261u};
            static constexpr std::uint32_t FNV_PRIME {16777619u};
        };

        struct Variant64 {
            using Type = std::uint64_t;
            static constexpr std::uint64_t FNV_OFFSET_BASIS {14695981039346656037ul};
            static constexpr std::uint64_t FNV_PRIME {1099511628211ul};
        };

        template<typename V>
        class HashedStr {
        public:
            using Type = typename V::Type;

            constexpr HashedStr() noexcept
                : hash(0u) {}

            explicit constexpr HashedStr(const char* const string) noexcept
                : hash(fnv1a(string)) {}  // TODO maybe should be consteval: soon

            explicit HashedStr(const std::string& string) noexcept
                : hash(fnv1a(string.c_str())) {}

            constexpr operator Type() const noexcept { return hash; }
            constexpr bool operator==(const HashedStr other) const noexcept { return hash == other.hash; }
        private:
            static constexpr Type fnv1a(const char* const string) noexcept {
                Type hash {V::FNV_OFFSET_BASIS};

                for (std::size_t i {0u}; string[i] != '\0'; i++) {
                    hash ^= static_cast<Type>(string[i]);
                    hash *= V::FNV_PRIME;
                }

                return hash;
            }

            Type hash;
        };
    }

    using HashedStr32 = internal::HashedStr<internal::Variant32>;
    using HashedStr64 = internal::HashedStr<internal::Variant64>;

    namespace literals {
        constexpr HashedStr32 operator""_h(const char* const string, std::size_t) noexcept {
            return HashedStr32(string);
        }

        constexpr HashedStr64 operator""_H(const char* const string, std::size_t) noexcept {
            return HashedStr64(string);
        }
    }

    template<typename V>
    struct Hash {
        constexpr std::size_t operator()(const V hashed_string) const noexcept {
            static_assert(sizeof(V) <= sizeof(std::size_t));

            if constexpr (sizeof(V) < sizeof(std::size_t)) {  // Using 32-bit hashes in a 64-bit environment
                return std::hash<typename V::Type>()(hashed_string);
            } else {
                return static_cast<std::size_t>(hashed_string);  // Using 64-bit hashes
            }
        }
    };
}
