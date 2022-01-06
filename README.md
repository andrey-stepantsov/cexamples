# Sample C code: mutlithreading with mutexes

The code demostrates a simple ring buffer implemented in C. The multithreading is supported via POSIX Threads (pthreads).

## Features
 - CMake
 - Doxygen
 - The POSIX Threads library (pthreads)
 - Google Test for unit testing

## Requirements

 - CLang
 - CMake
 - Doxygen (optional)

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

## Vim related notes
### my '~/.vimrc'
```
" Plugins will be downloaded under the specified directory.
call plug#begin(has('nvim') ? stdpath('data') . '/plugged' : '~/.vim/plugged')

" Declare the list of plugins.
Plug 'justmao945/vim-clang'
Plug 'rhysd/vim-clang-format'
Plug 'prabirshrestha/vim-lsp'
Plug 'mattn/vim-lsp-settings'

" List ends here. Plugins become visible to Vim after this call.
call plug#end()
let g:clang_c_completeopt = 'longest,menuone,preview'
:set exrc
```
### my .vimrc
```
set ts=4 sw=4
let g:clang_compilation_database = './build'
```
### generating 'compile commands' to support `vim-clang`
```
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B build
```

### Vim session
a vim session is started with:
```
:mksession! .session.vim
```
a vim session can be loaded with:
```
vim -S .session.vim
```
