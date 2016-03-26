/* A few utility functions. Debug/error handling macros are adapted from
 * LCTHW.
 */
#ifndef _util_h
#define _util_h

#include <stdio.h>

#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); exit(1); }

#define check_mem(A) check(A, "Out of memory")

#endif
