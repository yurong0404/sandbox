#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <dirent.h>

/*extern int chdir(const char *directory)*/
/*{*/
/*	static int (*old_chdir)(char *directory) = NULL;*/

/*	if (strncmp(directory, d_path, strlen(d_path))==0)*/
/*	{*/
/*		void *handle = dlopen("libc.so.6", RTLD_LAZY);*/
/*		if(handle != NULL)*/
/*			old_chdir = dlsym(handle, "chdir");*/
/*		return 0;*/
/*	}*/
/*	else*/
/*	{*/
/*		fprintf(stderr, "[sandbox] chdir: access to %s is not allowed", directory);*/
/*		return -1;*/
/*	}*/
/*}*/

DIR *opendir(const char *name)
{
	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60];
	realpath(name, realname);

	DIR* (*old_opendir)(const char *name) = NULL;

	if (strncmp(realname, d_path, strlen(d_path))==0)
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_opendir = dlsym(handle, "opendir");
		return old_opendir(name);
	}
	else
	{
		fprintf(stderr, "[sandbox] chdir: access to %s is not allowed\n", realname);
		return NULL;
	}
}
