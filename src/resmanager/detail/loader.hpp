#pragma once

#include <memory>
#include <utility>

namespace resmanager {
    // Generic loader
    // The resource pointer type must be some sort of smart pointer, like std::shared_ptr
    // It must have copy semantics and be default-constructible
    template<typename T, typename R>
    struct Loader {
        using ResourcePointerType = R;

        template<typename... Args>
        ResourcePointerType operator()(Args&&... args) const {
            return ResourcePointerType(std::forward<Args>(args)...);
        }
    };

    // Specialized loader for std::shared_ptr
    template<typename T>
    struct Loader<T, std::shared_ptr<T>> {
        using ResourcePointerType = std::shared_ptr<T>;

        template<typename... Args>
        ResourcePointerType operator()(Args&&... args) const {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }
    };

    // Used by default by the cache
    template<typename T>
    using DefaultLoader = Loader<T, std::shared_ptr<T>>;
}
