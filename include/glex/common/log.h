#pragma once

#define DEBUG true

#if DEBUG
#include <cstdio>
#define DEBUG_PRINT(...) do { printf(__VA_ARGS__); fflush(stdout); } while (0)
#define DEBUG_PRINTLN(...) do { printf(__VA_ARGS__); puts(""); fflush(stdout); } while (0)
#else
#define DEBUG_PRINT(...)
#define DEBUG_PRINTLN(...)
#endif

#define ERROR_PRINT(...) do { fprintf(stderr, __VA_ARGS__); fflush(stderr); } while (0)
#define ERROR_PRINTLN(...) do { fprintf(stderr, __VA_ARGS__); puts(""); fflush(stderr); } while (0)