#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <dirent.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/stat.h>

#define	MAXARGS	31

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

//V
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
		fprintf(stderr, "[sandbox] chmod: access to %s is not allowed\n", pathname);
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

int link(const char *oldpath, const char *newpath)
{
	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60], realname2[60];
	realpath(oldpath, realname);
	realpath(newpath, realname2);

	int (*old_link)(const char *oldpath, const char *newpath) = NULL;

	if (strncmp(realname, d_path, strlen(d_path))!=0)
	{
		fprintf(stderr, "[sandbox] link: access to %s is not allowed\n", realname);
		errno = EACCES;
		return -1;		
	}
	else if (strncmp(realname2, d_path, strlen(d_path))!=0)
	{
		fprintf(stderr, "[sandbox] link: access to %s is not allowed\n", realname2);
		errno = EACCES;
		return -1;	
	}
	else
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_link = dlsym(handle, "link");
		return old_link(oldpath, newpath);
	}
}

int mkdir(const char *pathname, mode_t mode)
{
	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60];

	realpath(pathname, realname);

	int (*old_mkdir)(const char *pathname, mode_t mode) = NULL;

	if (strncmp(realname, d_path, strlen(d_path))==0)
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_mkdir = dlsym(handle, "mkdir");
		return old_mkdir(pathname, mode);
	}
	else
	{
		fprintf(stderr, "[sandbox] mkdir: access to %s is not allowed\n", realname);
		errno = EACCES;
		return -1;
	}
}

//V
int open(const char *path, int oflag, ...)
{
	va_list ap;
	int argno = 0;
	int mode = 0;
	int var = 0;

	va_start(ap, oflag);
	while (var != 0 && argno<2)
	{
		if (argno==1)
			var = 1;
		mode = va_arg(ap, int);
		argno++;
	}
	va_end(ap);

	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60];

	realpath(path, realname);

	int (*old_open)(const char *path, int oflag, ...) = NULL;

	if (strncmp(realname, d_path, strlen(d_path))==0)
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_open = dlsym(handle, "open");
		if (var == 0)
			return old_open(path, oflag);
		else
			return old_open(path, oflag, mode);
	}
	else
	{
		fprintf(stderr, "[sandbox] open: access to %s is not allowed\n", realname);
		errno = EACCES;
		return -1;
	}
}

//V
int openat(int dirfd, const char *pathname, int flags, ...)
{
	va_list ap;
	int argno = 0;
	int mode = 0;
	int var = 0;

	va_start(ap, flags);
	while (var != 0 && argno<2)
	{
		if (argno==1)
			var = 1;
		mode = va_arg(ap, int);
		argno++;
	}
	va_end(ap);

	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60];

	realpath(pathname, realname);

	int (*old_openat)(int dirfd, const char *pathname, int flags, ...) = NULL;

	if (strncmp(realname, d_path, strlen(d_path))==0)
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_openat = dlsym(handle, "openat");
		if (var == 0)
			return old_openat(dirfd, pathname, flags);
		else
			return old_openat(dirfd, pathname, flags, mode);
	}
	else
	{
		fprintf(stderr, "[sandbox] openat: access to %s is not allowed\n", realname);
		errno = EACCES;
		return -1;
	}
}

// V
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
		fprintf(stderr, "[sandbox] opendir: access to %s is not allowed\n", realname);
		errno = EACCES;
		return NULL;
	}
}

//V
ssize_t readlink(const char *pathname, char *buf, size_t bufsiz)
{
	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60];

	realpath(pathname, realname);

	ssize_t (*old_readlink)(const char *pathname, char *buf, size_t bufsiz) = NULL;

	if (strncmp(realname, d_path, strlen(d_path))==0)
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_readlink = dlsym(handle, "readlink");
		return old_readlink(pathname, buf, bufsiz);
	}
	else
	{
		fprintf(stderr, "[sandbox] readlink: access to %s is not allowed\n", realname);
		errno = EACCES;
		return -1;
	}
}

int remove ( const char * filename )
{
	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60];

	realpath(filename, realname);

	ssize_t (*old_remove)(const char * filename) = NULL;

	if (strncmp(realname, d_path, strlen(d_path))==0)
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_remove = dlsym(handle, "remove");
		return old_remove(filename);
	}
	else
	{
		fprintf(stderr, "[sandbox] remove: access to %s is not allowed\n", realname);
		errno = EACCES;
		return -1;
	}
}

//V
int rename(const char *old, const char *new)
{
	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60], realname2[60];
	realpath(old, realname);
	realpath(new, realname2);

	int (*old_rename)(const char *old, const char *new) = NULL;

	if (strncmp(realname, d_path, strlen(d_path))!=0)
	{
		fprintf(stderr, "[sandbox] rename: access to %s is not allowed\n", realname);
		errno = EACCES;
		return -1;		
	}
	else if (strncmp(realname2, d_path, strlen(d_path))!=0)
	{
		fprintf(stderr, "[sandbox] rename: access to %s is not allowed\n", realname2);
		errno = EACCES;
		return -1;	
	}
	else
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_rename = dlsym(handle, "rename");
		return old_rename(old, new);
	}
}

//V
int rmdir(const char *path)
{
	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60];

	realpath(path, realname);

	int (*old_rmdir)(const char *path) = NULL;

	if (strncmp(realname, d_path, strlen(d_path))==0)
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_rmdir = dlsym(handle, "rmdir");
		return old_rmdir(path);
	}
	else
	{
		fprintf(stderr, "[sandbox] rmdir: access to %s is not allowed\n", realname);
		errno = EACCES;
		return -1;
	}
}

int stat(const char *pathname, struct stat *statbuf)
{
	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60];

	realpath(pathname, realname);

	int (*old_stat)(const char *pathname, struct stat *statbuf) = NULL;

	if (strncmp(realname, d_path, strlen(d_path))==0)
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_stat = dlsym(handle, "stat");
		return old_stat(pathname, statbuf);
	}
	else
	{
		fprintf(stderr, "[sandbox] stat: access to %s is not allowed\n", realname);
		errno = EACCES;
		return -1;
	}
}

//V
int symlink(const char *target, const char *linkpath)
{
	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60], realname2[60];
	realpath(target, realname);
	realpath(linkpath, realname2);

	int (*old_symlink)(const char *target, const char *linkpath) = NULL;

	if (strncmp(realname, d_path, strlen(d_path))!=0)
	{
		fprintf(stderr, "[sandbox] symlink: access to %s is not allowed\n", realname);
		errno = EACCES;
		return -1;		
	}
	else if (strncmp(realname2, d_path, strlen(d_path))!=0)
	{
		fprintf(stderr, "[sandbox] symlink: access to %s is not allowed\n", realname2);
		errno = EACCES;
		return -1;	
	}
	else
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_symlink = dlsym(handle, "symlink");
		return old_symlink(target, linkpath);
	}
}

int symlinkat(const char *oldpath, int newdirfd, const char *newpath)
{
	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60], realname2[60];
	realpath(oldpath, realname);
	realpath(newpath, realname2);

	int (*old_symlinkat)(const char *oldpath, int newdirfd, const char *newpath) = NULL;

	if (strncmp(realname, d_path, strlen(d_path))!=0)
	{
		fprintf(stderr, "[sandbox] symlinkat: access to %s is not allowed\n", realname);
		errno = EACCES;
		return -1;		
	}
	else if (strncmp(realname2, d_path, strlen(d_path))!=0)
	{
		fprintf(stderr, "[sandbox] symlinkat: access to %s is not allowed\n", realname2);
		errno = EACCES;
		return -1;	
	}
	else
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_symlinkat = dlsym(handle, "symlinkat");
		return old_symlinkat(oldpath, newdirfd, newpath);
	}
}

//V
int unlink(const char *pathname)
{
	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60];

	realpath(pathname, realname);

	int (*old_unlink)(const char *pathname) = NULL;

	if (strncmp(realname, d_path, strlen(d_path))==0)
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_unlink = dlsym(handle, "unlink");
		return old_unlink(pathname);
	}
	else
	{
		fprintf(stderr, "[sandbox] unlink: access to %s is not allowed\n", realname);
		errno = EACCES;
		return -1;
	}
}

//V
int unlinkat(int dirfd, const char *pathname, int flags)
{
	char *d_path;
	d_path = getenv("DPATH");
	
	char realname[60];

	realpath(pathname, realname);

	int (*old_unlinkat)(int dirfd, const char *pathname, int flags) = NULL;

	if (strncmp(realname, d_path, strlen(d_path))==0)
	{
		void *handle = dlopen("libc.so.6", RTLD_LAZY);
		if(handle != NULL)
			old_unlinkat = dlsym(handle, "unlinkat");
		return old_unlinkat(dirfd, pathname, flags);
	}
	else
	{
		fprintf(stderr, "[sandbox] unlinkat: access to %s is not allowed\n", realname);
		errno = EACCES;
		return -1;
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
