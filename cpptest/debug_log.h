#pragma once

#define DEBUG true

#if DEBUG
#define DEBUG_WRITE(...) printf(__VA_ARGS__)
#define DEBUG_WRITELN(...) do { DEBUG_WRITE(__VA_ARGS__); puts(""); } while (0)
#else
#define DEBUG_WRITE(...)
#define DEBUG_WRITELN(...)
#endif