#ifndef _ERRNO_H_
#define _ERRNO_H_

static int errno = 0;
#define strerror( i_err ) "error message not available"

/* Error Codes */
#define EPERM           1
#define ENOENT          2
#define ESRCH           3
#define EINTR           4
#define EIO             5
#define ENXIO           6
#define E2BIG           7
#define ENOEXEC         8
#define EBADF           9
#define ECHILD          10
#define EAGAIN          11
#define ENOMEM          12
#define EACCES          13
#define EFAULT          14
#define EBUSY           16
#define EEXIST          17
#define EXDEV           18
#define ENODEV          19
#define ENOTDIR         20
#define EISDIR          21
#define EINVAL          22
#define ENFILE          23
#define EMFILE          24
#define ENOTTY          25
#define EFBIG           27
#define ENOSPC          28
#define ESPIPE          29
#define EROFS           30
#define EMLINK          31
#define EPIPE           32
#define EDOM            33
#define ERANGE          34
#define EDEADLK         36
#if 0 /* defined differently in winsock.h on WinCE */
#define ENAMETOOLONG    38
#endif
#define ENOLCK          39
#define ENOSYS          40
#if 0 /* defined differently in winsock.h on WinCE */
#define ENOTEMPTY       41
#endif

#endif  /* _ERRNO_H_ */
