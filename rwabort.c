#define _GNU_SOURCE
#include <sys/stat.h>
#include <stdarg.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
#include <fcntl.h>
#include <err.h>

static void *do_dlsym (const char *name)
{
	void *ptr = dlsym (RTLD_NEXT, name);
	if (ptr == NULL)
		err (1, "dlsym(%s)", name);
	return ptr;
}

static int open_is_bad (int flags)
{
	return (flags & O_WRONLY) == O_WRONLY
		|| (flags & O_RDWR) == O_RDWR
		|| (flags & O_CREAT) == O_CREAT
		|| (flags & O_TRUNC) == O_TRUNC
		|| (flags & O_EXCL) == O_EXCL
		;
}

int openat (int fd, const char *path, int flags, ...)
{
	static int (*next_openat)(int, const char *, int, mode_t) = NULL;
	va_list ap;
	mode_t mode;

	va_start (ap, flags);
	mode = va_arg (ap, mode_t);
	va_end (ap);

	fprintf (stderr, "openat(%d, '%s', 0x%x, 0x%x);\n", fd, path, flags, (unsigned)mode);

	if (open_is_bad (flags))
		abort ();

	if (next_openat == NULL)
		next_openat = do_dlsym ("openat");

	return next_openat (fd, path, flags, mode);
}

int open (const char *path, int flags, ...)
{
	static int (*next_open)(const char *, int, mode_t);
	va_list ap;
	mode_t mode;

	va_start (ap, flags);
	mode = va_arg (ap, mode_t);
	va_end (ap);

	fprintf (stderr, "open('%s', 0x%x, 0x%x);\n", path, flags, (unsigned)mode);

	if (open_is_bad (flags))
		abort ();

	if (next_open == NULL)
		next_open = do_dlsym ("open");

	return next_open (path, flags, mode);
}

int creat (const char *path, mode_t mode)
{
	fprintf (stderr, "creat('%s', 0x%x);\n", path, (unsigned)mode);
	abort ();
	return -1;
}

int mkdir (const char *path, mode_t mode)
{
	fprintf (stderr, "mkdir('%s', 0x%x);\n", path, (unsigned)mode);
	abort ();
	return -1;
}

int mkdirat (int fd, const char *path, mode_t mode)
{
	fprintf (stderr, "mkdirat(%d, '%s', 0x%x);\n", fd, path, (unsigned)mode);
	abort ();
	return -1;
}

int mknod (const char *path, mode_t mode, dev_t dev)
{
	fprintf (stderr, "mknod('%s', 0x%x, %d);\n", path, (unsigned)mode, (int)dev);
	abort ();
	return -1;
}

int mknodat (int fd, const char *path, mode_t mode, dev_t dev)
{
	fprintf (stderr, "mknodat(%d, '%s', 0x%x, %d);\n", fd, path, (unsigned)mode, (int)dev);
	abort ();
	return -1;
}
