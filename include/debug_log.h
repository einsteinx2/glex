#pragma once

#define DEBUG true

#if DEBUG
#include <cstdio>
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#define DEBUG_PRINTLN(...) do { printf(__VA_ARGS__); puts(""); } while (0)
#else
#define DEBUG_PRINT(...)
#define DEBUG_PRINTLN(...)
#endif