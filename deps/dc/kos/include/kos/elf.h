/* KallistiOS ##version##

   kos/elf.h
   Copyright (C)2000,2001,2003 Dan Potter

*/

/** \file   kos/elf.h
    \brief  ELF binary loading support.

    This file contains the support functionality for loading ELF binaries in
    KOS. This includes the various header structures and whatnot that are used
    in ELF files to store code/data/relocations/etc. This isn't necessarily
    meant for running multiple processes, but more for loadable library support
    within KOS.

    \author Dan Potter
*/

#ifndef __KOS_ELF_H
#define __KOS_ELF_H

#include <sys/cdefs.h>
__BEGIN_DECLS

#include <arch/types.h>
#include <sys/queue.h>

/** \brief  ELF file header.

    This header is at the beginning of any valid ELF binary and serves to
    identify the architecture of the binary and various data about it.

    \headerfile kos/elf.h
*/
struct elf_hdr_t {
    uint8 ident[16];    /**< \brief ELF identifier */
    uint16 type;        /**< \brief ELF file type */
    uint16 machine;     /**< \brief ELF file architecture */
    uint32 version;     /**< \brief Object file version */
    uint32 entry;       /**< \brief Entry point */
    uint32 phoff;       /**< \brief Program header offset */
    uint32 shoff;       /**< \brief Section header offset */
    uint32 flags;       /**< \brief Processor flags */
    uint16 ehsize;      /**< \brief ELF header size in bytes */
    uint16 phentsize;   /**< \brief Program header entry size */
    uint16 phnum;       /**< \brief Program header entry count */
    uint16 shentsize;   /**< \brief Section header entry size */
    uint16 shnum;       /**< \brief Section header entry count */
    uint16 shstrndx;    /**< \brief String table section index */
};

/** \defgroup elf_archs                 ELF architecture types

    These are the various architectures that we might care about for ELF files.

    @{
*/
#define EM_386  3   /**< \brief x86 (IA32) */
#define EM_ARM  40  /**< \brief ARM */
#define EM_SH   42  /**< \brief SuperH */
/** @} */

/** \defgroup elf_sections              Section header types

    These are the various types of section headers that can exist in an ELF
    file.

    @{
*/
#define SHT_NULL        0       /**< \brief Inactive section */
#define SHT_PROGBITS    1       /**< \brief Program code/data */
#define SHT_SYMTAB      2       /**< \brief Full symbol table */
#define SHT_STRTAB      3       /**< \brief String table */
#define SHT_RELA        4       /**< \brief Relocation table, with addends */
#define SHT_HASH        5       /**< \brief Symbol hash table */
#define SHT_DYNAMIC     6       /**< \brief Dynamic linking info */
#define SHT_NOTE        7       /**< \brief Notes section */
#define SHT_NOBITS      8       /**< \brief A section that occupies no space in
the file */
#define SHT_REL         9       /**< \brief Relocation table, no addends */
#define SHT_SHLIB       10      /**< \brief Reserved */
#define SHT_DYNSYM      11      /**< \brief Dynamic-only sym tab */
#define SHT_LOPROC  0x70000000  /**< \brief Start of processor specific types */
#define SHT_HIPROC  0x7fffffff  /**< \brief End of processor specific types */
#define SHT_LOUSER  0x80000000  /**< \brief Start of program specific types */
#define SHT_HIUSER  0xffffffff  /**< \brief End of program specific types */
/** @} */

/** \defgroup elf_hdrflags              Section header flags

    These are the flags that can be set on a section header. These are related
    to whether the section should reside in memory and permissions on it.

    @{
*/
#define SHF_WRITE       1           /**< \brief Writable data */
#define SHF_ALLOC       2           /**< \brief Resident */
#define SHF_EXECINSTR   4           /**< \brief Executable instructions */
#define SHF_MASKPROC    0xf0000000  /**< \brief Processor specific mask */
/** @} */

/** \defgroup elf_specsec               Special section indeces

    These are the indices to be used in special situations in the section array.

    @{
*/
#define SHN_UNDEF   0       /**< \brief Undefined, missing, irrelevant */
#define SHN_ABS     0xfff1  /**< \brief Absolute values */
/** @} */

/** \brief  ELF Section header.

    This structure represents the header on each ELF section.

    \headerfile kos/elf.h
*/
struct elf_shdr_t {
    uint32 name;        /**< \brief Index into string table */
    uint32 type;        /**< \brief Section type \see elf_sections */
    uint32 flags;       /**< \brief Section flags \see elf_hdrflags */
    uint32 addr;        /**< \brief In-memory offset */
    uint32 offset;      /**< \brief On-disk offset */
    uint32 size;        /**< \brief Size (if SHT_NOBITS, amount of 0s needed) */
    uint32 link;        /**< \brief Section header table index link */
    uint32 info;        /**< \brief Section header extra info */
    uint32 addralign;   /**< \brief Alignment constraints */
    uint32 entsize;     /**< \brief Fixed-size table entry sizes */
};
/* Link and info fields:

switch (sh_type) {
    case SHT_DYNAMIC:
        link = section header index of the string table used by
            the entries in this section
        info = 0
    case SHT_HASH:
        ilnk = section header index of the string table to which
            this info applies
        info = 0
    case SHT_REL, SHT_RELA:
        link = section header index of associated symbol table
        info = section header index of section to which reloc applies
    case SHT_SYMTAB, SHT_DYNSYM:
        link = section header index of associated string table
        info = one greater than the symbol table index of the last
            local symbol (binding STB_LOCAL)
}

*/

/** \defgroup elf_binding               Symbol binding types.

    These are the values that can be set to say how a symbol is bound in an ELF
    binary. This is stored in the upper 4 bits of the info field in elf_sym_t.

    @{
*/
#define STB_LOCAL   0       /**< \brief Local (non-exported) symbol */
#define STB_GLOBAL  1       /**< \brief Global (exported) symbol */
#define STB_WEAK    2       /**< \brief Weak-linked symbol */
/** @} */

/** \defgroup elf_symtype               Symbol types.

    These are the values that can be set to say what kind of symbol a given
    symbol in an ELF file is. This is stored in the lower 4 bits of the info
    field in elf_sym_t.

    @{
*/
#define STT_NOTYPE  0       /**< \brief Symbol has no type */
#define STT_OBJECT  1       /**< \brief Symbol is an object */
#define STT_FUNC    2       /**< \brief Symbol is a function */
#define STT_SECTION 3       /**< \brief Symbol is a section */
#define STT_FILE    4       /**< \brief Symbol is a file name */
/** @} */

/** \brief  Symbol table entry

    This structure represents a single entry in a symbol table in an ELF file.

    \headerfile kos/elf.h
*/
struct elf_sym_t {
    uint32 name;        /**< \brief Index into file's string table */
    uint32 value;       /**< \brief Value of the symbol */
    uint32 size;        /**< \brief Size of the symbol */
    uint8 info;         /**< \brief Symbol type and binding */
    uint8 other;        /**< \brief 0. Holds no meaning. */
    uint16 shndx;       /**< \brief Section index */
};

/** \brief  Retrieve the binding type for a symbol.
    \param  info            The info field of an elf_sym_t.
    \return                 The binding type of the symbol.
    \see                    elf_binding
*/
#define ELF32_ST_BIND(info) ((info) >> 4)

/** \brief  Retrieve the symbol type for a symbol.
    \param  info            The info field of an elf_sym_t.
    \return                 The symbol type of the symbol.
    \see                    elf_symtype
*/
#define ELF32_ST_TYPE(info) ((info) & 0xf)

/** \brief  ELF Relocation entry (with explicit addend).

    This structure represents an ELF relocation entry with an explicit addend.
    This structure is used on some architectures, whereas others use the
    elf_rel_t structure instead.

    \headerfile kos/elf.h
*/
struct elf_rela_t {
    uint32 offset;      /**< \brief Offset within section */
    uint32 info;        /**< \brief Symbol and type */
    int32 addend;       /**< \brief Constant addend for the symbol */
};

/** \brief  ELF Relocation entry (without explicit addend).

    This structure represents an ELF relocation entry without an explicit
    addend. This structure is used on some architectures, whereas others use the
    elf_rela_t structure instead.

    \headerfile kos/elf.h
*/
struct elf_rel_t {
    uint32      offset;     /**< \brief Offset within section */
    uint32      info;       /**< \brief Symbol and type */
};

/** \defgroup elf_reltypes              ELF relocation types

    These define the types of operations that can be done to calculate
    relocations within ELF files.

    @{
*/
#define R_SH_DIR32  1       /**< \brief SuperH: Rel = Symbol + Addend */
#define R_386_32    1       /**< \brief x86: Rel = Symbol + Addend */
#define R_386_PC32  2       /**< \brief x86: Rel = Symbol + Addend - Value */
/** @} */

/** \brief  Retrieve the symbol index from a relocation entry.
    \param  i               The info field of an elf_rel_t or elf_rela_t.
    \return                 The symbol table index from that relocation entry.
*/
#define ELF32_R_SYM(i) ((i) >> 8)

/** \brief  Retrieve the relocation type from a relocation entry.
    \param  i               The info field of an elf_rel_t or an elf_rela_t.
    \return                 The relocation type of that relocation.
    \see                    elf_reltypes
*/
#define ELF32_R_TYPE(i) ((uint8)(i))

struct klibrary;

/** \brief  Kernel-specific definition of a loaded ELF binary.

    This structure represents the internal representation of a loaded ELF binary
    in KallistiOS (specifically as a dynamically loaded library).

    \headerfile kos/elf.h
*/
typedef struct elf_prog {
    void *data;             /**< \brief Pointer to program in memory */
    uint32 size;            /**< \brief Memory image size (rounded up to page size) */

    /* Library exports */
    ptr_t lib_get_name;     /**< \brief Pointer to get_name() function */
    ptr_t lib_get_version;  /**< \brief Pointer to get_version() function */
    ptr_t lib_open;         /**< \brief Pointer to library's open function */
    ptr_t lib_close;        /**< \brief Pointer to library's close function */

    char fn[256];           /**< \brief Filename of library */
} elf_prog_t;

/** \brief  Load an ELF binary.

    This function loads an ELF binary from the VFS and fills in an elf_prog_t
    for it.

    \param  fn              The filename of the binary on the VFS.
    \param  shell           Unused?
    \param  out             Storage for the binary that will be loaded.
    \return                 0 on success, <0 on failure.
*/
int elf_load(const char *fn, struct klibrary * shell, elf_prog_t * out);

/** \brief  Free a loaded ELF program.

    This function cleans up an ELF binary that was loaded with elf_load().

    \param  prog            The loaded binary to clean up.
*/
void elf_free(elf_prog_t *prog);

__END_DECLS

#endif  /* __OS_ELF_H */

