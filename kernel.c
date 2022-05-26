/*
 * Name: Ahmed Affaan
 * Title: kernel.c
 * Folder: minimalOS
 * Date: 26/05/2022
 * Country: Republic of Maldives
 * Description: Minimal OS, to show how to boot an x86 machine. UFCFWK-15-2
 * Student Number(UWE): 19045165
 * Student ID:(VC): 1802035
 * Note: Uncomment codes to execute and comment them when not in use.
 */

// Program start.

/*
 * Desc: Minimal OS, to show how to boot an x86 machine. UFCFWK-15-2
 */
#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
 
/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 
/* Hardware text mode color constants. */
enum vga_color {
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};

// This function will choose colours for the kernel. 
uint8_t make_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}

// This function will creates the characters colours needed for the kernel.
uint16_t make_vgaentry(char c, uint8_t color) {
  uint16_t c16 = c;
  uint16_t color16 = color;
  return c16 | color16 << 8;
}

// This function will validate strings. 
size_t strlen(const char* str) {
  size_t ret = 0;
  while ( str[ret] != 0 )
    ret++;
  return ret;
}

// Variable Declarations.
// Width of the terminal screen.
static const size_t VGA_WIDTH = 80;
// Height of the terminal screen.
static const size_t VGA_HEIGHT = 25;

// Refers to termninal row.
size_t terminal_row;
// Refers to terminal column.
size_t terminal_column;
// Refers to terminal's colour.
uint8_t terminal_color;
// Refers to terminal's buffer.
uint16_t* terminal_buffer;
 
// This function will refresh the programs terminal screen.
void terminal_initialize() {
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
  terminal_buffer = (uint16_t*) 0xB8000;
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = make_vgaentry(' ', terminal_color);
    }
  }
}
 
// This function will set the terminal's colours.
void terminal_setcolor(uint8_t color) {
  terminal_color = color;
}
 
// This function will display various characters on the terminal screen.
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = make_vgaentry(c, color);
}

// This function will display the current position of the cursor on the terminal screen.
void terminal_putchar(char c) {
  terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
  if (++terminal_column == VGA_WIDTH) {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT) {
      terminal_row = 0;
    }
  }
}
 
// This function will help print characters on the terminal screen.
void terminal_writestring(const char* data) {
  size_t datalen = strlen(data);
  for (size_t i = 0; i < datalen; i++)
    terminal_putchar(data[i]);
}

// Programs main function.
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main() {
  /* Initialize terminal interface */
  terminal_initialize();
  
  /* Since there is no support for newlines in terminal_putchar
   * yet, '\n' will produce some VGA specific character instead.
   * This is normal.
   */
  //terminal_writestring("Hello, kernel World!\n");

  // This will print writestring in the colour red once.
  for (size_t q = 0; q < 1; q++) 
  {
    // Selects the colour red.
    terminal_setcolor(COLOR_RED);
    // Prints the message.
    terminal_writestring("Hello, kernel World!");
  }

  // This will print writestring in the colour white six times.
  for (size_t w = 0; w < 6; w++)
  {
    // Selects the colour white.
    terminal_setcolor(COLOR_WHITE);
    // Prints the message.
    terminal_writestring("Hello, kernel World!");
  }

  // This will print writestring in the colour blue eighteen times.
  for (size_t e = 0; e < 18; e++)
  {
    // Selects the colour blue.
    terminal_setcolor(COLOR_BLUE);
    // Prints the message.
    terminal_writestring("Hello, kernel World!");
  }


}

// Program end.

