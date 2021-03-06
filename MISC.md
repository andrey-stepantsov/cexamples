# Misc. notes

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