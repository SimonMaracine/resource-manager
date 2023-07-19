# resmanager

## A tiny resource manager library written for myself

It is heavily inspired by [EnTT](https://github.com/SimonMaracine/entt)'s resource management,
which sadly doesn't fully suit my needs. That's why I wrote my own library.

This library is header-only. But you can still use CMake to cleanly integrate it into your CMake
project. Type this into the command line to add the repository:

```text
git submodule add https://github.com/SimonMaracine/resource-manager <path/to/the/submodule>
```

Then, in `CMakeLists.txt` write:

```cmake
add_subdirectory(<path/to/the/submodule>)
target_link_libraries(<target> PRIVATE resmanager)
```

And to build the example program:

```cmake
set(RESMANAGER_BUILD_EXAMPLE ON)
```

Check out `example/main.cpp` for some examples.
