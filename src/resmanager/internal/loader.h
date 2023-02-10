#pragma once

#include <memory>
#include <utility>

namespace resmanager {
    // Generic loader
    template<typename T, typename R>
    struct Loader {
        using ResourceType = R;

        template<typename... Args>
        ResourceType load(Args&&... args) const {
            return R {std::forward<Args>(args)...};
        }
    };

    // Specialized loader for std::shared_ptr
    template<typename T>
    struct Loader<T, std::shared_ptr<T>> {
        using ResourceType = std::shared_ptr<T>;

        template<typename... Args>
        ResourceType load(Args&&... args) const {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }
    };

    template<typename T>
    using DefaultLoader = Loader<T, std::shared_ptr<T>>;
}
