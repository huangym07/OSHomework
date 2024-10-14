#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NDEBUG
#define debug(M, ...) 
#else
#define debug(M, ...) fprintf(stderr, "debug %s:%d" M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clear_errno() (errno == 0 ? "NONE" : strerror(errno))
#define log_err(M, ...) fprintf(stderr, "[ERROR] %s:%d (error: %s)" M "\n", __FILE__, __LINE__, clear_errno(), ##__VA_ARGS__)
#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno = 0; goto error; }
#define check_mem(A) check(A, "Out of memory")

#endif
