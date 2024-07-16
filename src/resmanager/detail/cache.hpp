#pragma once

#include <unordered_map>
#include <cstddef>
#include <utility>
#include <type_traits>

#include "loader.hpp"
#include "hashing.hpp"

namespace resmanager {
    template<typename T, typename L = DefaultLoader<T>, typename K = HashedStr64, typename H = Hash<K>>
    class Cache {
    public:
        using ResourcePointerType = typename L::ResourcePointerType;

        static_assert(std::is_default_constructible_v<ResourcePointerType>);
        static_assert(std::is_copy_constructible_v<ResourcePointerType>);
        static_assert(std::is_copy_assignable_v<ResourcePointerType>);

        // If already present, return the resource directly, otherwise load and then return it
        template<typename... Args>
        ResourcePointerType load(const K id, Args&&... args) {
            return load_check(id, std::forward<Args>(args)...).first;
        }

        // If already present, return the resource directly, otherwise load and then return it
        // Also return true if the resource was already in the cache, false otherwise
        template<typename... Args>
        std::pair<ResourcePointerType, bool> load_check(const K id, Args&&... args) {
            if (auto iter = cache.find(id); iter != cache.end()) {
                return std::make_pair(iter->second, true);
            } else {
                return std::make_pair(force_load(id, std::forward<Args>(args)...), false);
            }
        }

        // Just load the resource, replacing the old one, if already present
        template<typename... Args>
        ResourcePointerType force_load(const K id, Args&&... args) {
            const L loader {};
            ResourcePointerType resource {loader(std::forward<Args>(args)...)};
            cache[id] = resource;

            return resource;
        }

        // Get the resource; throws std::out_of_range, if the resource is not in the cache
        ResourcePointerType get(const K id) const {
            return cache.at(id);
        }

        // Check if the resource is present
        bool contains(const K id) const {
            return cache.find(id) != cache.cend();
        }

        // Release and return the resource; throws std::out_of_range, if the resource is not in the cache
        ResourcePointerType release(const K id) {
            ResourcePointerType resource {cache.at(id)};
            cache.erase(id);

            return resource;
        }

        // Merge the other cache into this cache
        void merge(Cache& other) {
            cache.merge(other.cache);
        }

        // Merge the other cache into this cache
        // If the two caches contain the same resource, then the other will replace this
        void merge_replace(const Cache& other) {
            for (const auto& [other_id, _] : other.cache) {
                cache[other_id] = other.cache.at(other_id);
            }
        }

        // Clear the cache
        void clear() noexcept {
            cache.clear();
        }

        // Get the size of the cache
        std::size_t size() const noexcept {
            return cache.size();
        }

        // Check if the cache is empty
        bool empty() const noexcept {
            return cache.empty();
        }
    private:
        std::unordered_map<K, ResourcePointerType, H> cache;
    };
}
