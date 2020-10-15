#ifndef PLAT_ARM_ATAGS_H
#define PLAT_ARM_ATAGS_H

/**
 * ARMTag boot information data structures and types
 */

#include <config.h>
#include <stddef.h>
#include <stdint.h>

/**
 * Tags: Name,      Value,      Size, Description
 * ATAG_NONE,       0x00000000, 2,	Empty tag used to end list
 * ATAG_CORE,	      0x54410001, 5 (2 if empty),	First tag used to start list
 * ATAG_MEM,        0x54410002,	4,	Describes a physical area of memory
 * ATAG_VIDEOTEXT,	0x54410003,	5,	Describes a VGA text display
 * ATAG_RAMDISK,    0x54410004,	5,	Describes how the ramdisk will be used in kernel
 * ATAG_INITRD2,	  0x54420005,	4,	Describes where the compressed ramdisk image is placed in memory
 * ATAG_SERIAL,	    0x54410006,	4,	64-bit board serial number
 * ATAG_REVISION	  0x54410007,	3,	32 bit board revision number
 * ATAG_VIDEOLFB	  0x54410008,	8,	Initial values for vesafb-type framebuffers
 * ATAG_CMDLINE	    0x54410009,	2 + ((length_of_cmdline + 3) / 4),	Command line to pass to kernel 
 */
#define ATAG_NONE	      (0x00000000)
#define ATAG_CORE	      (0x54410001)
#define ATAG_MEM	      (0x54410002)
#define ATAG_VIDEOTEXT	(0x54410003)
#define ATAG_RAMDISK	  (0x54410004)
#define ATAG_INITRD2	  (0x54420005)
#define ATAG_SERIAL	    (0x54410006)
#define ATAG_REVISION	  (0x54410007)
#define ATAG_VIDEOLFB 	(0x54410008)
#define ATAG_CMDLINE	  (0x54410009)


typedef struct {
  uint32_t size; /* legth of tag in words including this header */
  uint32_t tag;  /* tag value */
} atag_header_t;

typedef struct {
  uint32_t flags;     /* bit 0 = read-only */
  uint32_t pagesize;  /* systems page size (usually 4k) */
  uint32_t rootdev;   /* root device number */
} atag_core_t;

typedef struct {
  uint32_t size;   /* size of the area */
  uint32_t start;  /* physical start address */
} atag_mem_t;

typedef struct {
  uint8_t       x;           /* width of display */
  uint8_t       y;           /* height of display */
  uint16_t      video_page;
  uint8_t       video_mode;
  uint8_t       video_cols;
  uint16_t      video_ega_bx;
  uint8_t       video_lines;
  uint8_t       video_isvga;
  uint16_t      video_points;
} atag_videotext_t;

typedef struct {
  uint32_t flags;      /* bit 0 = load, bit 1 = prompt */
  uint32_t size;       /* decompressed ramdisk size in _kilo_ bytes */
  uint32_t start;      /* starting block of floppy-based RAM disk image */
} atag_ramdisk_t;

typedef struct {
  uint32_t start;      /* physical start address */
  uint32_t size;       /* size of compressed ramdisk image in bytes */
} atag_initrd2_t;

typedef struct {
  uint32_t low;
  uint32_t high;
} atag_serialnr_t;

typedef struct {
  uint32_t rev;
} atag_revision_t;

typedef struct {
  uint16_t      lfb_width;
  uint16_t      lfb_height;
  uint16_t      lfb_depth;
  uint16_t      lfb_linelength;
  uint32_t       lfb_base;
  uint32_t       lfb_size;
  uint8_t       red_size;
  uint8_t       red_pos;
  uint8_t       green_size;
  uint8_t       green_pos;
  uint8_t       blue_size;
  uint8_t       blue_pos;
  uint8_t       rsvd_size;
  uint8_t       rsvd_pos;
} atag_videolfb_t;

typedef struct {
  // The command line must be NULL terminated.
  // The length_of_cmdline variable should include the terminator.
  char    cmdline[1]; /* this is the minimum size */
} atag_cmdline_t;

typedef struct  {
  atag_header_t hdr;
  union {
    atag_core_t       core;
    atag_mem_t        mem;
    atag_videotext_t  videotext;
    atag_ramdisk_t    ramdisk;
    atag_initrd2_t    initrd2;
    atag_serialnr_t   serialnr;
    atag_revision_t   revision;
    atag_videolfb_t   videolfb;
    atag_cmdline_t    cmdline;
  } u;
} atag_t;

#define atag_next(t)    ((atag_t *)((uint32_t *)(t) + (t)->hdr.size))
#define atag_size(type) ((sizeof(atag_header_t) + sizeof(struct type)) >> 2)

#endif
