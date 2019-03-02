#ifndef __TRACE_H__
#define __TRACE_H__

#include <stdio.h>

#ifndef LOGLEVEL
	#define LOGLEVEL 0
#endif

/* stringify macros */
#define xstr(s) str(s)
#define str(s) #s

#define LOG(level, prefix, ...) do {\
	if (LOGLEVEL >= level) {\
		fprintf(stderr, "[ %s ] : ", xstr(prefix));\
		fprintf(stderr, __VA_ARGS__);\
		fprintf(stderr, "\n");\
	}\
} while (0)

/* prints only if greater or equal LOGLEVEL define */
#define ERROR(...) LOG(0, ERROR, __VA_ARGS__)
#define WARN(...) LOG(5, WARNING, __VA_ARGS__)
#define INFO(...) LOG(6, INFO, __VA_ARGS__)
#define DEBUG(...) LOG(7, DEBUG, __VA_ARGS__)

#endif /* __TRACE_H__ */