# NNWCLI

This is a tiny static library (potentially to be converted into a header-only library) for parsing command inputs and dispatching them regardless of the way it is done.
It is inspired from my own Python project admin-console-python, but unline that, it doesn't implement reading the line from the POSIX-based terminal, since there are plenty of libraries that already do that, such as GNU Readline and libedit.
Many stuff is not yet documented, as the code is released quickly.
For an example of usage, see src/main.cpp

# BUILDING

This is a CMake project that has no dependencies, and only uses standard C++ library.
It is known to work with GCC, but can potentially be used well with other compilers.

We recommend following these commands when using GNU/Linux or Cygwin while being in the root directory of the project:

```
cmake -DCMAKE_BUILD_TYPE=RELEASE -Bbuild
cmake --build build --target nnwcli
```

This will create an out-of-source build of a small static library.
The release builds have an -O3 compiler flag.

Or otherwise when using it in another CMake project as a git submodule, simply adding this project as a subdirectory.

```cmake
add_subdirectory(path/to/this/root)

target_include_directories(your_executable_or_library PUBLIC path/to/this/root/include)
target_link_libraries(your_executable_or_library PUBLIC nnwcli)
```

will make the library and its headers available with just a few lines.

# SUPPORTED OS

Any operating system that supports C++ compilers can be used to build this library together with other projects.

# LICENSE

The project is open-source and is licensed under [The MIT License](https://opensource.org/license/mit).
