# sandbox
There are 6 files in this directory
1. makefile
2. lib.h (header file of lib.c)
3. lib.c (It contains some useful functions called by main.c)
4. main.c (It contains main functino)
5. sandbox.c (It contains the functions to preload)
6. README.md

After executing "make" command, 2 files will show up.
1. sandbox (It's executable file and contains main function)
2. sandbox.so (It's shared library and it will be preloaded in the main function)
