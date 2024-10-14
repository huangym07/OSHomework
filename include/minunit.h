#undef NDEBUG
#ifndef __minunit_h__
#define __minunit_h__

#include <stdio.h>
#include <errno.h>
#include <lcthw/dbg.h>

#define mu_start() char *message = NULL

#define mu_assert(test, message) do { \
		if(!(test)) { log_err(message); return message; } \
	} while(0) 

#define mu_test(test) do { \
		debug("------------------%s", #test); \
		message = test(); tests_run++; if(message) return message; \
	} while(0)

#define RUN_TESTS(name) int main(int argc, char *argv[]) { \
	argc = 1; \
	printf("-------------RUNNING: %s\n", argv[argc - 1]);\
	debug("--------------RUNNING: %s", argv[0]); \
	char *result = name();\
	if(result) { \
		printf("FAILED: %s\n", result); \
	} else { \
		printf("ALL TESTS PASSED\n"); \
	} \
	printf("Tests run: %d\n", tests_run); \
	return result != NULL; \
}

int tests_run;
#endif
