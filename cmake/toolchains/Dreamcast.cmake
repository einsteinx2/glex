#
## MacOS Specific Build Settings
#

include(${CMAKE_CURRENT_LIST_DIR}/Common.cmake)

# Let the main script know we're building for Dreamcast
set(DREAMCAST_BUILD TRUE)

# Use the GLdc OpenGL library
set(USE_GLDC TRUE)

# Set up cross compiling
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_C_COMPILER "kos-cc")
set(CMAKE_CXX_COMPILER "kos-c++")
set(CMAKE_AR "kos-ar" CACHE FILEPATH "Archiver")
set(CMAKE_RANLIB "kos-ranlib" CACHE FILEPATH "Ranlib")
set(CMAKE_ASM_COMPILER "kos-as")
set(CMAKE_LINKER "kos-c++")
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_SYSTEM_INCLUDE_PATH "$ENV{KOS_BASE}/include")
set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")
set(THREADS_PREFER_PTHREAD_FLAG OFF)
set(OpenGL_GL_PREFERENCE "LEGACY")

# Add compiler flags shared by all languages
add_definitions(
    -DDREAMCAST
    -D__DREAMCAST__
    -D_arch_dreamcast
    -D__arch_dreamcast
    -D_arch_sub_pristine
)

# Add language specific compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -Wextra -Wno-unused-parameter" CACHE STRING "" FORCE)
# We force disable debugging information in release builds - this is so we override settings in the
# kallistios gnu-wrappers which sometimes pick up -g through the default env vars
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -s -fomit-frame-pointer -g0" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -s -fomit-frame-pointer -g0" CACHE STRING "" FORCE)



# Dreamcast compiler flag notes:
#
# Ian older GCC version compiler flags:
#
# ml -m4-single-only -O3  -fexpensive-optimizations -finline-functions -ffreestanding 
# -mnomacsave -fno-rtti -fno-exceptions -mdalign -fargument-noalias-global -fwritable-strings
# -funroll-all-loops  -fno-gcse  -ffast-math -fno-omit-frame-pointer  -fno-optimize-sibling-calls
# -freduce-all-givs    -falign-functions=32 -falign-labels=32 -falign-loops=32 -falign-jumps=32
#
#
# Moop GCC 4 and 9 shared compiler flags:
#
# -ml -m4-single-only -Wall -Wextra -Wno-unused -ffreestanding -fno-zero-initialized-in-bss 
# -fomit-frame-pointer -fno-strict-aliasing -fno-unwind-tables -fno-asynchronous-unwind-tables 
# -fno-exceptions -fno-delete-null-pointer-checks -fno-stack-protector -fno-stack-check 
# -fmerge-constants -fmerge-all-constants -std=gnu11 $(INCLUDE) -Wa,-adghlmns=$*.asm -o $@ -c $<
#
#
# Moop GCC 9 compiler flags:
# 
# sh4-elf-gcc -ml -m4-single-only -maccumulate-outgoing-args -mfsca -mfsrra -ffp-contract=fast 
# -ffinite-math-only -funsafe-math-optimizations -Og -ffreestanding -fomit-frame-pointer 
# -fno-delete-null-pointer-checks -fno-common -fno-zero-initialized-in-bss -fno-exceptions 
# -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-stack-protector -fno-stack-check
# -fno-strict-aliasing -fno-merge-all-constants -fno-merge-constants --std=gnu11 $HFILES -g3
# -Wall -Wextra -Wdouble-promotion -Wpedantic -fmessage-length=0 -c -MMD -MP -Wa,-adghlmns="${f%.*}.out" 
# -MF"${f%.*}.d" -MT"${f%.*}.o" -o "${f%.*}.o" "${f%.*}.c"
#
# Moop GCC 9 linker flags:
# 
# "$GCC_FOLDER_NAME/bin/sh4-elf-gcc" -T$LinkerScript -static -nostdlib -nostartfiles -s 
# -Wl,-Ttext=0x8c010000 -Wl,--warn-common -Wl,--no-undefined -Wl,-z,text -Wl,-z,norelro -Wl,
# -z,now -Wl,-Map=output.map -o "program.elf" @"objects.list"
#


# Dreamcast SH4 performance notes:
#
# @Moopthehedgehog
#   I think for you the biggest thing is that you align stuff
#   that's verrrrry important on SH4, 4 byte align all variables
#
#   btw -mdalign does nothing for m4-single-only:
#   ```
#   -mdalign
#   Align doubles at 64-bit boundaries. Note that this changes the calling conventions, 
#   and thus some functions from the standard C library do not work unless you recompile 
#   it first with -mdalign.
#   ``` 
#
# @mrneo240
#   Other than size waste is there any issue aligning everything not variables to 32 
#   and aligning all variables to 4?
#
# @Moopthehedgehog
#   no, it's fast
# 
# @ian micheal
#   On ports you notice it a lot goes from aslow to useable
# 
# @Moopthehedgehog
#   In many cases putting nops in the right place are critical for performance, I was reading
#
# @ian micheal
#   Yes even delay in places helps one of my secrets
#
# @Moopthehedgehog
#   I do stuff like this:
#   ```
#   bf/s go_main
#   enable_exceptions:
#       mov.l r2,@r3 ! set dcload_present C variable, 0 = none, 1 = present with console, 2 = present without console
#       mov.l    init_sr_exceptions,r1
#       ldc    r1,sr
#   ```
#   find the delay :wink:
#
#   If you look in sh4_math.h I got some more example of double-use delay slots
#   Basically makes the jump a one-cycle instruction in every possible path, highly recommended
# 
#   This is how one hits maximum SH4 data throughput:
#   ```
#   zero_out_bss:
#       mov.l r2,@r0
#       cmp/hi r0,r1
#       bt/s zero_out_bss
#       add #4,r0
#   ```
#
#   https://devblogs.microsoft.com/oldnewthing/20190805-00/?p=102749
#   That's a good reference, the blog
#
# @Ben Baron
#   @Moopthehedgehog btw what is that bss section and why does zeroing it out improve performance?
#
# @Moopthehedgehog
#   @Ben Baron Oh I was talking about the loop executing in 4 instructions
#
#   BSS is the part of a binary that is supposed to hold uninitialized data 
#   and is supposed to be zeroed out at binary load time
#
#   when you make a variable like this:
#   `int a;`
#   it goes into bss
#