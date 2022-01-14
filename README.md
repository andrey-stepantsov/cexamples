# C/C++ Samples

## Sample Post-Order-Traversal with no recursion

Demonstrates a non-recursive traversal of a binary tree. The traversal is implemented as exploratory unit tests in `tests/post_order_traversal_test.cc` 
and `tests/post_order_traversal_macro_test.cc`.

The second one (in `post_order_traversal_macro_test.cc`) supports macro impelementation of the stack operations that speeds up execution of the reversal. Also, the second variation is more
readable.

## Sample C code: mutlithreading with mutexes

The code demostrates a simple ring buffer implemented in C. The multithreading is supported via POSIX Threads (pthreads).

## Features

- CMake
- Doxygen
- The POSIX Threads library (pthreads)
- Google Test for unit testing

## Requirements

To build and test the code you need the following tools:

- CLang
- CMake
- Doxygen (optional)
  If you do not have these tools installed you need to install them first.

For example on a clean Debian-10 you could build it with the following steps,
Note that CMake v3.18 is minimum so we build from sources and install v3.22

```
sudo apt-get install git clang doxygen curl libssl-dev
cd ~/
mkdir tmp && cd tmp
curl -L https://github.com/Kitware/CMake/releases/download/v3.22.1/cmake-3.22.1.tar.gz | tar xz
cd cmake-3.22.1/
./bootstrap
sudo make install
```

## Comments

### build

Clone the repo locally `cd` to the root folder and:

```
mkdir build
cd build
cmake ..
make
ctest
```

### Doxygen

```
make doc_doxygen
```

To remove path from the documentation of a class (or function, type, etc.)

```
FULL_PATH_NAMES        = NO
```

Alternatively, if a relative path is desirable:

```
FULL_PATH_NAMES        = YES
STRIP_FROM_PATH        = ../ # or ../..
```

## Misc files

The `.clang` and `.clang-format` files in the root project folder are necessary to support convinient VIM configuration. They are not needed to build the code and run the tests.
