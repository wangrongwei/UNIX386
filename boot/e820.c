// running in real mode may require:
__asm__(".code16gcc\n");
 
// SMAP entry structure
#include <ctypes.h>
#include <unistd.h>
#include <boot.h>

struct boot_params boot_params __attribute__((aligned(16)));

/*
 * struct boot_params
 */
// load memory map to buffer - note: regparm(3) avoids stack issues with gcc in real mode
int __attribute__((noinline)) __attribute__((regparm(3)))
detect_memory_e820(SMAP_entry_t *buffer, int maxentries)
{
	uint32_t contID = 0;
	int entries = 0, signature, bytes;

	do {
		__asm__ __volatile__ ("int 0x15" 
				: "=a"(signature), "=c"(bytes), "=b"(contID)
				: "a"(0xE820), "b"(contID), "c"(24), "d"(0x534D4150), "D"(buffer));
		if (signature != 0x534D4150) 
			return -1; // error
		if (bytes > 20 && (buffer->ACPI & 0x0001) == 0) {
			// ignore this entry
		} else {
			buffer++;
			entries++;
		}
	} while (contID != 0 && entries < maxentries);

	return entries;
}

/*
 * This function will be invoked by boot.asm, then detecting memory layout.
 */
void detect_memory(void)
{
	SMAP_entry_t *desc = NULL;

	desc = boot_params.e820_map;
	boot_params.e820_entries = detect_memory_e820(desc, 128);
	return;
}

