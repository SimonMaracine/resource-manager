#pragma once

#include <memory>
#include <utility>

namespace resmananger {
    template<typename T>
    struct Loader {
        template<typename... Args>
        std::shared_ptr<T> operator()(Args&&... args) const {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }
    };
}
