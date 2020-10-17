/**
 * Flat Device Tree
 * 
 * @see https://www.kernel.org/doc/Documentation/devicetree/booting-without-of.txt
 */
#ifndef BOOT_FDT_H
#define BOOT_FDT_H
#include <config.h>
#include <stdint.h>

typedef struct {
  uint32_t  magic;                  /* magic word OF_DT_HEADER */
  uint32_t  totalsize;              /* total size of DT block */
  uint32_t  off_dt_struct;          /* offset to structure */
  uint32_t  off_dt_strings;         /* offset to strings */
  uint32_t  off_mem_rsvmap;         /* offset to memory reserve map */
  uint32_t  version;                /* format version */
  uint32_t  last_comp_version;      /* last compatible version */
  /* version 2 fields below */
  uint32_t  boot_cpuid_phys;        /* Which physical CPU id we're booting on */
  /* version 3 fields below */
  uint32_t  size_dt_strings;        /* size of the strings block */
  /* version 17 fields below */
  uint32_t  size_dt_struct;		/* size of the DT structure block */
} fdt_header_t; // All values in this header are in big endian format!

#define ben2len_u32(x)  ((x & 0xFF)<<24) | ((x & 0xFF00) << 8) | \
                          ((x & 0xFF0000) >> 8) | ((x & 0xFF000000) >> 24) 
/* Definitions used by the flattened device tree */
#define OF_DT_HEADER      0xd00dfeed    /* 4: version, 4: total size */
#define OF_DT_BEGIN_NODE  0x1           /* Start node: full name*/
#define OF_DT_END_NODE    0x2           /* End node */
#define OF_DT_PROP        0x3           /* Property: name off, size, content */
#define OF_DT_END         0x9


#endif