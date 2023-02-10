#pragma once

#include <memory>
#include <unordered_map>
#include <cstdint>
#include <cstddef>
#include <utility>

#include "loader.h"
#include "hashing.h"

namespace resmanager {
    template<typename T, typename L = DefaultLoader<T>, typename K = HashedStr64, typename H = Hash<K>>
    class Cache {
    public:
        Cache() = default;
        ~Cache() = default;
        Cache(const Cache&) = default;
        Cache& operator=(const Cache&) = default;

        Cache(Cache&& other) noexcept;
        Cache& operator=(Cache&& other) noexcept;

        template<typename... Args>
        typename L::ResourceType load(const K id, Args&&... args);

        template<typename... Args>
        typename L::ResourceType force_load(const K id, Args&&... args);

        typename L::ResourceType operator[](const K id) const;
        bool contains(const K id) const;
        typename L::ResourceType release(const K id);

        void merge(Cache&& other);
        void merge_replace(Cache&& other);
        void clear();

        size_t size() const { return cache.size(); }
        bool empty() const { return cache.empty(); }
    private:
        L loader;
        std::unordered_map<K, typename L::ResourceType, H> cache;
    };

    template<typename T, typename L, typename K, typename H>
    Cache<T, L, K, H>::Cache(Cache&& other) noexcept {
        cache = std::move(other.cache);
        loader = other.loader;
    }

    template<typename T, typename L, typename K, typename H>
    Cache<T, L, K, H>& Cache<T, L, K, H>::operator=(Cache&& other) noexcept {
        cache = std::move(other.cache);
        loader = other.loader;

        return *this;
    }

    template<typename T, typename L, typename K, typename H>
    template<typename... Args>
    typename L::ResourceType Cache<T, L, K, H>::load(const K id, Args&&... args) {
        if (auto iter = cache.find(id); iter != cache.end()) {
            return iter->second;
        } else {
            typename L::ResourceType resource = loader.load(std::forward<Args>(args)...);
            cache[id] = resource;

            return resource;
        }
    }

    template<typename T, typename L, typename K, typename H>
    template<typename... Args>
    typename L::ResourceType Cache<T, L, K, H>::force_load(const K id, Args&&... args) {
        typename L::ResourceType resource = loader.load(std::forward<Args>(args)...);
        cache[id] = resource;

        return resource;
    }

    template<typename T, typename L, typename K, typename H>
    typename L::ResourceType Cache<T, L, K, H>::operator[](const K id) const {
        return cache.at(id);
    }

    template<typename T, typename L, typename K, typename H>
    bool Cache<T, L, K, H>::contains(const K id) const {
        return cache.count(id) == 1;
    }

    template<typename T, typename L, typename K, typename H>
    typename L::ResourceType Cache<T, L, K, H>::release(const K id) {
        if (auto iter = cache.find(id); iter != cache.end()) {
            typename L::ResourceType resource = std::move(iter->second);
            cache.erase(id);

            return resource;
        }

        return nullptr;
    }

    template<typename T, typename L, typename K, typename H>
    void Cache<T, L, K, H>::merge(Cache&& other) {
        cache.merge(std::move(other.cache));
    }

    template<typename T, typename L, typename K, typename H>
    void Cache<T, L, K, H>::merge_replace(Cache&& other) {
        for (auto& [other_id, _] : other.cache) {
            if (cache.find(other_id) != cache.end()) {
                cache[other_id] = std::move(other.cache[other_id]);
            } else {
                cache[other_id] = other.cache[other_id];
            }
        }
    }

    template<typename T, typename L, typename K, typename H>
    void Cache<T, L, K, H>::clear() {
        cache.clear();
    }
}
