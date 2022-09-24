#pragma once

#include <memory>
#include <unordered_map>
#include <cstdint>
#include <cstddef>
#include <utility>

#include "loader.h"

namespace resmanager {
    template<typename T, typename L = Loader<T>>
    class Cache {
    public:
        Cache() = default;
        ~Cache() = default;
        Cache(const Cache&) = delete;
        Cache& operator=(const Cache&) = delete;

        Cache(Cache&& other);
        Cache& operator=(Cache&& other);

        template<typename... Args>
        std::shared_ptr<T> load(const uint32_t id, Args&&... args);

        template<typename... Args>
        std::shared_ptr<T> force_load(const uint32_t id, Args&&... args);

        std::shared_ptr<T> operator[](const uint32_t id) const;
        bool contains(const uint32_t id) const;
        std::shared_ptr<T> release(const uint32_t id);

        void merge(Cache&& other);
        void clear();

        size_t size() const { return cache.size(); }
        bool empty() const { return cache.empty(); }
    private:
        L loader;
        std::unordered_map<uint32_t, std::shared_ptr<T>> cache;
    };

    template<typename T, typename L>
    Cache<T, L>::Cache(Cache&& other) {
        cache = std::move(other.cache);
        loader = other.loader;
    }

    template<typename T, typename L>
    Cache<T, L>& Cache<T, L>::operator=(Cache&& other) {
        cache = std::move(other.cache);
        loader = other.loader;

        return *this;
    }

    template<typename T, typename L>
    template<typename... Args>
    std::shared_ptr<T> Cache<T, L>::load(const uint32_t id, Args&&... args) {
        if (auto iter = cache.find(id); iter != cache.end()) {
            return iter->second;
        } else {
            std::shared_ptr<T> resource = loader.load(std::forward<Args>(args)...);
            cache[id] = resource;
            return resource; 
        }
    }

    template<typename T, typename L>
    template<typename... Args>
    std::shared_ptr<T> Cache<T, L>::force_load(const uint32_t id, Args&&... args) {
        std::shared_ptr<T> resource = loader.load(std::forward<Args>(args)...);
        cache[id] = resource;
        return resource; 
    }

    template<typename T, typename L>
    std::shared_ptr<T> Cache<T, L>::operator[](const uint32_t id) const {
        return cache[id];
    }

    template<typename T, typename L>
    bool Cache<T, L>::contains(const uint32_t id) const {
        return cache.count(id) == 1;
    }

    template<typename T, typename L>
    std::shared_ptr<T> Cache<T, L>::release(const uint32_t id) {
        if (auto iter = cache.find(id); iter != cache.end()) { 
            std::shared_ptr<T> resource = std::move(iter->second);
            cache.erase(id);
            return resource;
        }

        return nullptr;
    }

    template<typename T, typename L>
    void Cache<T, L>::merge(Cache&& other) {
        cache.merge(std::move(other));
    }

    template<typename T, typename L>
    void Cache<T, L>::clear() {
        cache.clear();
    }
}
