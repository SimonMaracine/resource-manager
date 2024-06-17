# resmanager

## A tiny resource manager library written for myself

It is heavily inspired by [EnTT](https://github.com/skypjack/entt)'s resource management,
which sadly doesn't fully suit my needs. That's why I wrote my own library. I tested it on `GCC 13.2`
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

Check out `example/main.cpp` for some examples.

Development takes place on the `main` branch. `stable` is for actual use.
