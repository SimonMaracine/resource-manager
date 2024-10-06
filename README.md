# resmanager

## A tiny resource manager library written for myself

It is heavily inspired by [EnTT](https://github.com/skypjack/entt)'s resource management,
which sadly doesn't fully suit my needs. That's why I wrote my own library. I tested it on `GCC 14.1`
and `MSVC 19.39`. It requires at least `C++17`.

## Usage

This library is header-only. But you can still use CMake to cleanly integrate it into your CMake
project. Type this into the command line to add the repository:

```text
git submodule add -b stable -- https://github.com/SimonMaracine/resource-manager <path/to/submodule>
```

Then in `CMakeLists.txt` write:

```cmake
add_subdirectory(<path/to/submodule>)
target_link_libraries(<target> PRIVATE resmanager)
```

And to build the example program:

```cmake
set(RESMANAGER_BUILD_EXAMPLE ON)
```

Check out `example/main.cpp` for some examples. Read the source files for some documentation.

Development takes place on the `main` branch. `stable` is for actual use.

## Example

```c++
#include <memory>
#include <string>
#include <resmanager/resmanager.hpp>

// Some resource type
class Image {
public:
    explicit Image(const std::string& file_path) {
        // ...
    }
private:
    // ...
};

// Some function that needs to be called at image creation time
void some_image_initialization_function(std::shared_ptr<Image> image) {
    // ...
}

class Loader {
public:
    // Example 1
    std::shared_ptr<Image> load_image1(resmanager::HashedStr64 id, const std::string& file_path) {
        // Load the resource if it's not in the cache
        // If it's in the cache, return it
        // Thus, this function can be called multiple times
        return images.load(id, file_path);
    }

    // Example 2
    std::shared_ptr<Image> load_image2(resmanager::HashedStr64 id, const std::string& file_path) {
        // Same as load(), but also return if it was accessed from the cache or loaded
        const auto [image, present] {images.load_check(id, file_path)};

        if (!present) {
            // This needs to be called only once
            some_image_initialization_function(image);
        }

        return image;
    }

    // Example 3
    std::shared_ptr<Image> load_image3(resmanager::HashedStr64 id, const std::string& file_path) {
        // Check if already present in the cache
        // If so, access it directly
        if (images.contains(id)) {
            return images.get(id);
        }

        // Because we already checked if it's in the cache, we load it directly
        const auto image {images.force_load(id, file_path)};

        // This is still called only once
        some_image_initialization_function(image);

        return image;
    }
private:
    resmanager::Cache<Image> images;

    // Caches for other resource types...
};
```
