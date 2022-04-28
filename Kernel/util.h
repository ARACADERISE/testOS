#ifndef MocaOS_UTIL
#define MocaOS_UTIL

typedef unsigned char		uint8;
typedef char			int8;
typedef unsigned short		uint16;
typedef short			int16;
typedef unsigned int		uint32;
typedef int			int32;
typedef unsigned long long	uint64;
typedef long long		int64;
typedef uint64			size_t;

/*	INFO STRUCTS	*/
// VesaInfoBlock:
// 	Information about the current vesa memory mode.
typedef struct VesaInfoBlock
{
	uint16		attributes;
	uint8		window_a;
	uint8		window_b;
	uint16		gran;
	uint16		window_size;
	uint16		seg_a;
	uint16		seg_b;
	uint32		win_func_ptr;
	uint16		pitch;
	uint16		width;
	uint16		height;
	uint8		w_char;
	uint8		y_char;
	uint8		planes;
	uint8		bpp;
	uint8		banks;
	uint8		memory_model;
	uint8		bank_size;
	uint8		image_pages;
	uint8		reserved1;
	uint8		red_mask;
	uint8		red_pos;
	uint8		green_mask;
	uint8		green_pos;
	uint8		blue_mask;
	uint8		blue_pos;
	uint8		reserved_mask;
	uint8		reserved_pos;
	uint8		direct_color_attributes;
	uint32		framebuffer;
	uint32		off_screen_mem_off;
	uint16		off_screen_mem_size;
	uint8		reserved2[206];
} __attribute__((packed)) Vesa_Info_Block;

// TerminalCursor
//	Information about current position.
typedef struct TerminalCursor
{
	uint16	cursor_x;
	uint16	cursor_y;
} Terminal_Cursor;

// Default values
static Terminal_Cursor tc = {
	.cursor_x = 0,
	.cursor_y = 0
};

// Smap
//  Information about memory map.
typedef struct Smap
{
	uint64	base;
	uint64	length;
	uint32	type;
	uint32	acpi;
} __attribute__((packed)) _Smap;

// Constants
#define FB	                VIB->framebuffer
#define pitch               VIB->pitch
#define MakeColor(R, G, B)  (uint32)(R*65536 + G*256 + B	)

// General Colors
#define	WHITE	MakeColor(255, 255, 255)
#define BLACK	MakeColor(  0,   0,   0)
#define RED	MakeColor(255,	 0,   0)
#define GREEN	MakeColor(  0, 255,   0)
#define BLUE	MakeColor(  0,   0, 255)

/* General functions for memory/port communication. */
void *memset(void *buf, uint8 byte, uint32 length)
{
	uint8 *val = (uint8 *)buf;
	for(uint32 i = 0; i < length; i++)
		val[i] = byte;
	return buf;
}

uint8 inb(uint16 port)
{
	uint8 rv;
	__asm__ __volatile__ ("inb %1, %0" : "=a"(rv) : "dN"(port));
	return rv;
}
void outb(uint16 port, uint8 data)
{
	__asm__ __volatile__ ("outb %1, %0" : : "dN"(port), "a"(data));
}

/* Functions for screen display(updating the screen/clearing the screen). */
void update_screen()
{
	uint32 *ss, *ss2;

	if(tc.cursor_y >= 46)
	{
		ss = (uint32 *) FB;
		ss2 = ss + (1024 * 16);

		for(uint32 p = 0; p < 0xBA000; p++)
			*ss++ = *ss2++;
		for(uint32 p = 0; p < 0x6000; p++)
			*ss++ = BLACK;
		tc.cursor_x = 0;
		tc.cursor_y--;
		return;
	}
}

void clear_screen()
{
	uint32 *fb = (uint32 *) FB;

	for(uint32 i = 0; i < WIDTH * HEIGHT; i++)
		fb[i] = BLACK;

	tc.cursor_x = 0;
	tc.cursor_y = 0;
}


#endif
