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
## Example
```console
$ ./sandbox -h
./sandbox: invalid option -- 'h'
Usage: ./sandbox [-p sopath] [-d basedir] [--] cmd [cmd args ...]
	-p: set the path to sandbox.so, default = ./sandbox.so
	-d: restrict directory, default = .
	--: seperate the arguments for sandbox and for the executed command

$ ./sandbox ls
lib.c  lib.h  main.c  makefile	README.md  sandbox  sandbox.c  sandbox.so

# use "--" flag to split the argumemt between sandbox and 'ls'
$ ./sandbox -- ls /
[sandbox] opendir: access to / is not allowed
ls: cannot open directory '/': Permission denied

$ ./sandbox -d / -- ls /
bin    dev   initrd.img      lib32	 media	proc  sbin  sys  var
boot   etc   initrd.img.old  lib64	 mnt	root  snap  tmp  vmlinuz
cdrom  home  lib	     lost+found  opt	run   srv   usr  vmlinuz.old

$ ./sandbox -- sh -c 'ls'
[sandbox] execve(/bin/ls): not allowed
[sandbox] execve(/usr/games/ls): not allowed
[sandbox] execve(/usr/local/games/ls): not allowed
[sandbox] execve(/snap/bin/ls): not allowed
[sandbox] execve(/usr/local/cuda-8.0/bin//ls): not allowed
sh: 1: ls: Permission denied
```
