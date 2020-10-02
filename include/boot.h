// running in real mode may require:
__asm__(".code16gcc\n");

#ifndef __BOOT_H__
#define __BOOT_H__

#include <ctypes.h>

#define E820_MAX_ENTRIES_ZEROPAGE 128

typedef struct SMAP_entry {
	uint32_t BaseL; // base address uint64_t
	uint32_t BaseH;
	uint32_t LengthL; // length uint64_t
	uint32_t LengthH;
	uint32_t Type; // entry Type
	uint32_t ACPI; // extended
 
}__attribute__((packed)) SMAP_entry_t;

struct boot_e820_entry {
    u64 addr;    /* start of memory segment */
    u64 size;    /* size of memory segment */
    u32 type;    /* type of memory segment */
} __attribute__((packed));


/* The so-called "zeropage" */
struct boot_params {
#if 0
	struct screen_info screen_info;			/* 0x000 */
	struct apm_bios_info apm_bios_info;		/* 0x040 */
#endif
	u8  _pad2[4];					/* 0x054 */
	u64  tboot_addr;				/* 0x058 */
#if 0
	struct ist_info ist_info;			/* 0x060 */
#endif
	u8  _pad3[16];				/* 0x070 */
	u8  hd0_info[16];	/* obsolete! */		/* 0x080 */
	u8  hd1_info[16];	/* obsolete! */		/* 0x090 */
#if 0
	struct sys_desc_table sys_desc_table; /* obsolete! */	/* 0x0a0 */
	struct olpc_ofw_header olpc_ofw_header;		/* 0x0b0 */
#endif
	u32 ext_ramdisk_image;			/* 0x0c0 */
	u32 ext_ramdisk_size;				/* 0x0c4 */
	u32 ext_cmd_line_ptr;				/* 0x0c8 */
	u8  _pad4[116];				/* 0x0cc */
#if 0
	struct edid_info edid_info;			/* 0x140 */
	struct efi_info efi_info;			/* 0x1c0 */
#endif
	u32 alt_mem_k;				/* 0x1e0 */
	u32 scratch;		/* Scratch field! */	/* 0x1e4 */
	u8  e820_entries;				/* 0x1e8 */
	u8  eddbuf_entries;				/* 0x1e9 */
	u8  edd_mbr_sig_buf_entries;			/* 0x1ea */
	u8  kbd_status;				/* 0x1eb */
	u8  secure_boot;				/* 0x1ec */
	u8  _pad5[2];					/* 0x1ed */
	/*
	 * The sentinel is set to a nonzero value (0xff) in header.S.
	 *
	 * A bootloader is supposed to only take setup_header and put
	 * it into a clean boot_params buffer. If it turns out that
	 * it is clumsy or too generous with the buffer, it most
	 * probably will pick up the sentinel variable too. The fact
	 * that this variable then is still 0xff will let kernel
	 * know that some variables in boot_params are invalid and
	 * kernel should zero out certain portions of boot_params.
	 */
	u8  sentinel;					/* 0x1ef */
	u8  _pad6[1];					/* 0x1f0 */
#if 0
	struct setup_header hdr;    /* setup header */	/* 0x1f1 */
	__u8  _pad7[0x290-0x1f1-sizeof(struct setup_header)];
	__u32 edd_mbr_sig_buffer[EDD_MBR_SIG_MAX];	/* 0x290 */
#endif
#if 0
	struct boot_e820_entry e820_map[E820_MAX_ENTRIES_ZEROPAGE]; /* 0x2d0 */
#else
	SMAP_entry_t e820_map[E820_MAX_ENTRIES_ZEROPAGE]; /* 0x2d0 */
#endif
	u8  _pad8[48];				/* 0xcd0 */
#if 0
	struct edd_info eddbuf[EDDMAXNR];		/* 0xd00 */
#endif
	u8  _pad9[276];				/* 0xeec */
} __attribute__((packed));

/*
 * This variable is used to store some parameters during boot time.
 */
extern struct boot_params boot_params;

#endif

