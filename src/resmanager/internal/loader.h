#pragma once

#include <memory>
#include <utility>

namespace resmanager {
    template<typename T>
    struct Loader {
        template<typename... Args>
        std::shared_ptr<T> load(Args&&... args) const {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }
    };
}
