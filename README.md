# sandbox

## The descriptions of the files
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

## The approach to handle the restrict directory flag "-d" of sandbox
1. The main function will save the path of the flag "-d" into enviroment variable via "putenv("DPATH")" function<br>
2. The functions in the shared library will get the restrict directory from enviroment variable via "getenv("DPATH")" function
## Preload the shared library "sandbox.so" and execute the command by user
```C
system("LD_PRELOAD=./sandbox.so cmd arg1 arg2");
```
