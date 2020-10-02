
/*
 * floopy driver
 *
 * http://www.brokenthorn.com/Resources/OSDev20.html
 */

#include <block.h>
#include <interrupt.h>
#include <peripheral.h>

volatile bool ReceivedIRQ = false;
extern void outb(unsigned short port,unsigned short value);
extern unsigned char inb(unsigned short port);
extern unsigned short inw(unsigned short port);

static void fdc_handler(pt_regs *r)
{
	ReceivedIRQ = true;
}


void reset_floppy(int device)
{
	int i;
	char devs[] = {0x1C, 0x2D, 0x4E, 0x8F};

	register_interrupt_handler(IRQ6, fdc_handler);

	outb(0x3f7, 0x00);
	outb(0x3f2, 0x0c);

	ReceivedIRQ = false; // This will prevent the FDC from being faster than us!
 
	// Enter, then exit reset mode.
	outb(FLPYDSK_CTRL, 0x00);
	outb(FLPYDSK_DOR, 0x0C);

	while(!ReceivedIRQ); // Wait for the IRQ handler to run

#if 0
	// sense interrupt -- 4 of them typically required after a reset
	for (i = 4 ; i > 0 ; --i); 
	{
		flpydsk_send_command(SENSE_INTERRUPT);
		flpydsk_read_data();
		flpydsk_read_data();
	}
 
	outb(FLPYDSK_CTRL, 0x00); // 500Kbps -- for 1.44M floppy

	// configure the drive
	flpydsk_send_command(SPECIFY);
	outb(DATA_FIFO, steprate_headunload);
	outb(DATA_FIFO, headload_ndma);
#endif
}

/*
 * floppy init
 */
void floppy_init()
{
	int device;

	reset_floppy(device);
	// fdc_flag
	while(ReceivedIRQ);
	return;
}

uint8_t flpydsk_read_status()
{
	//! just return main status register
	return inb (FLPYDSK_MSR);
}

void flpydsk_send_command(uint8_t cmd)
{
	//! wait until data register is ready. We send commands to the data register
	for (int i = 0; i < 500; i++ )
		if ( flpydsk_read_status () & FLPYDSK_MSR_MASK_DATAREG )
			return outb (DATA_FIFO, cmd);
}

uint8_t flpydsk_read_data() 
{
	//! same as above function but returns data register for reading
	for (int i = 0; i < 500; i++ )
		if ( flpydsk_read_status () & FLPYDSK_MSR_MASK_DATAREG )
			return inb (DATA_FIFO);
}

//! initialize DMA to use phys addr 1k-64k
void flpydsk_initialize_dma()
{
	outb(0x0a,0x06);	//mask dma channel 2
	outb(0xd8,0xff);	//reset master flip-flop
	outb(0x04, 0);     //address=0x1000 
	outb(0x04, 0x10);
	outb(0xd8, 0xff);  //reset master flip-flop
	outb(0x05, 0xff);  //count to 0x23ff (number of bytes in a 3.5" floppy disk track)
	outb(0x05, 0x23);
	outb(0x80, 0);     //external page register = 0
	outb(0x0a, 0x02);  //unmask dma channel 2
}
 
//! prepare the DMA for read transfer
void flpydsk_dma_read()
{
 
	outb(0x0a, 0x06); //mask dma channel 2
	outb(0x0b, 0x56); //single transfer, address increment, autoinit, read, channel 2
	outb(0x0a, 0x02); //unmask dma channel 2
}
 
//! prepare the DMA for write transfer
void flpydsk_dma_write()
{
	outb(0x0a, 0x06); //mask dma channel 2
	outb(0x0b, 0x5a); //single transfer, address increment, autoinit, write, channel 2
	outb(0x0a, 0x02); //unmask dma channel 2
}

