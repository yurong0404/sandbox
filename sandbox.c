#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <dirent.h>
#include <errno.h>

int chdir(const char *path)
{
	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60];
	realpath(path, realname);

	int (*old_chdir)(const char *path) = NULL;

	if (strncmp(path, d_path, strlen(d_path))==0)
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_chdir = dlsym(handle, "chdir");
		return old_chdir(path);
	}
	else
	{
		fprintf(stderr, "[sandbox] chdir: access to %s is not allowed", path);
		errno = EACCES;
		return -1;
	}
}

int chmod(const char *pathname, mode_t mode)
{
	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60];
	realpath(pathname, realname);

	int (*old_chmod)(const char *pathname, mode_t mode) = NULL;

	if (strncmp(pathname, d_path, strlen(d_path))==0)
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_chmod = dlsym(handle, "chmod");
		return old_chmod(pathname, mode);
	}
	else
	{
		fprintf(stderr, "[sandbox] chmod: access to %s is not allowed", pathname);
		errno = EACCES;
		return -1;
	}
}

int chown(const char *path, uid_t owner, gid_t group)
{
	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60];
	realpath(path, realname);

	int (*old_chown)(const char *path, uid_t owner, gid_t group) = NULL;

	if (strncmp(realname, d_path, strlen(d_path))==0)
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_chown = dlsym(handle, "chown");
		return old_chown(path, owner, group);
	}
	else
	{
		fprintf(stderr, "[sandbox] chown: access to %s is not allowed\n", realname);
		errno = EACCES;
		return -1;
	}
}

int creat(const char *pathname, mode_t mode)
{
	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60];
	realpath(pathname, realname);

	int (*old_creat)(const char *pathname, mode_t mode) = NULL;

	if (strncmp(realname, d_path, strlen(d_path))==0)
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_creat = dlsym(handle, "creat");
		return old_creat(pathname, mode);
	}
	else
	{
		fprintf(stderr, "[sandbox] creat: access to %s is not allowed\n", realname);
		errno = EACCES;
		return -1;
	}
}

FILE *fopen(const char *pathname, const char *mode)
{
	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60];
	realpath(pathname, realname);

	FILE* (*old_fopen)(const char *pathname, const char *mode) = NULL;

	if (strncmp(realname, d_path, strlen(d_path))==0)
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_fopen = dlsym(handle, "fopen");
		return old_fopen(pathname, mode);
	}
	else
	{
		fprintf(stderr, "[sandbox] fopen: access to %s is not allowed\n", realname);
		errno = EACCES;
		return NULL;
	}
}

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
		errno = EACCES;
		return NULL;
	}
}

int execl(const char *path, const char *arg, ...)
{
	fprintf(stderr, "[sandbox] execl(%s): not allowed\n", path);
	errno = EACCES;
	return -1;
}


int execle(const char *path, const char *arg, ...)
{
	fprintf(stderr, "[sandbox] execle(%s): not allowed\n", path);
	errno = EACCES;
	return -1;
}

int execlp(const char *file, const char *arg, ...)
{
	fprintf(stderr, "[sandbox] execlp(%s): not allowed\n", file);
	errno = EACCES;
	return -1;
}

int execv(const char *path, char *const argv[])
{
	fprintf(stderr, "[sandbox] execv(%s): not allowed\n", path);
	errno = EACCES;
	return -1;
}

int execve(const char *pathname, char *const argv[], char *const envp[])
{
	fprintf(stderr, "[sandbox] execve(%s): not allowed\n", pathname);
	errno = EACCES;
	return -1;
}

int execvp(const char *file, char *const argv[])
{
	fprintf(stderr, "[sandbox] execvp(%s): not allowed\n", file);
	errno = EACCES;
	return -1;
}

int system(const char *command)
{
	fprintf(stderr, "[sandbox] system: not allowed\n");
	errno = EACCES;
	return -1;
}
