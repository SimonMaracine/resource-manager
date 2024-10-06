#pragma once

#include <cstdint>
#include <cstddef>
#include <string>  // std::hash

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
            static constexpr std::uint64_t FNV_OFFSET_BASIS {14695981039346656037u};
            static constexpr std::uint64_t FNV_PRIME {1099511628211u};
        };

        template<typename T>
        class HashedStr {
        public:
            using Type = typename T::Type;

            constexpr HashedStr() noexcept
                : hash(0) {}

            explicit constexpr HashedStr(const char* const string) noexcept
                : hash(fnv1a(string)) {}  // TODO maybe should be consteval: soon

            explicit HashedStr(const std::string& string) noexcept
                : hash(fnv1a(string.c_str())) {}

            constexpr operator Type() const noexcept { return hash; }
            constexpr bool operator==(const HashedStr other) const noexcept { return hash == other.hash; }
        private:
            static constexpr Type fnv1a(const char* const string) noexcept {
                Type hash {T::FNV_OFFSET_BASIS};

                for (std::size_t i {0}; string[i] != '\0'; i++) {
                    hash ^= static_cast<Type>(string[i]);
                    hash *= T::FNV_PRIME;
                }

                return hash;
            }

            Type hash;
        };
    }

    // 32-bit hash number
    using HashedStr32 = internal::HashedStr<internal::Variant32>;

    // 64-bit hash number
    using HashedStr64 = internal::HashedStr<internal::Variant64>;

    // Used to more easily convert string literals to hashes
    namespace literals {
        constexpr HashedStr32 operator""_h(const char* const string, std::size_t) noexcept {
            return HashedStr32(string);
        }

        constexpr HashedStr64 operator""_H(const char* const string, std::size_t) noexcept {
            return HashedStr64(string);
        }
    }

    // Pass-through hash functor, used by default by the cache
    // As the keys of the cache are already hashes, they don't need to use the default hash implementation of
    // unsigned int or unsigned long
    template<typename T>
    struct Hash {
        constexpr std::size_t operator()(const T hashed_string) const noexcept {
            static_assert(sizeof(T) <= sizeof(std::size_t));

            if constexpr (sizeof(T) < sizeof(std::size_t)) {  // Using 32-bit hashes in a 64-bit environment
                return std::hash<typename T::Type>()(hashed_string);
            } else {
                return static_cast<std::size_t>(hashed_string);  // Using 64-bit hashes
            }
        }
    };
}
