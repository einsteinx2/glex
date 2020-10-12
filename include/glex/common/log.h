#pragma once

#ifndef DEBUG_LOGGING
    #define DEBUG_LOGGING true
#endif

#if DEBUG_LOGGING
    #include <cstdio>
    #define DEBUG_PRINT(...) printf(__VA_ARGS__)
    #define DEBUG_PRINTLN(...) do { printf(__VA_ARGS__); puts(""); } while (0)
#else
    #define DEBUG_PRINT(...)
    #define DEBUG_PRINTLN(...)
#endif

#define ERROR_PRINT(...) fprintf(stderr, __VA_ARGS__)
#define ERROR_PRINTLN(...) do { fprintf(stderr, __VA_ARGS__); puts(""); } while (0)