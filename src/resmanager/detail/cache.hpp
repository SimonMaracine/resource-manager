#pragma once

#include <unordered_map>
#include <cstddef>
#include <utility>

#include "loader.hpp"
#include "hashing.hpp"

namespace resmanager {
    template<typename T, typename L = DefaultLoader<T>, typename K = HashedStr64, typename H = Hash<K>>
    class Cache {
    public:
        using ResourceType = typename L::ResourceType;

        // If already present, return the resource directly, otherwise load and then return it
        template<typename... Args>
        ResourceType load(const K id, Args&&... args) {
            if (auto iter = cache.find(id); iter != cache.end()) {
                return iter->second;
            } else {
                const L loader {};
                ResourceType resource {loader(std::forward<Args>(args)...)};
                cache[id] = resource;

                return resource;
            }
        }

        // Load the resource and replace the old one, if already present
        template<typename... Args>
        ResourceType force_load(const K id, Args&&... args) {
            const L loader {};
            ResourceType resource {loader(std::forward<Args>(args)...)};
            cache[id] = resource;

            return resource;
        }

        // Get the resource; throws std::out_of_range, if resource is not found
        ResourceType operator[](const K id) const {
            return cache.at(id);  // TODO maybe handle exception here and return null
        }

        // Check if the resource is present
        bool contains(const K id) const {
            return cache.find(id) != cache.cend();
        }

        // Release and return the resource
        ResourceType release(const K id) {
            if (auto iter = cache.find(id); iter != cache.end()) {
                ResourceType resource {std::move(iter->second)};
                cache.erase(id);

                return resource;
            }

            return nullptr;
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
        std::unordered_map<K, ResourceType, H> cache;
    };
}
