/*
 * Name: Ahmed Affaan
 * Title: kernel.c
 * Folder: minimalOS
 * Date: 26/05/2022
 * Country: Republic of Maldives
 * Description: Minimal OS, to show how to boot an x86 machine.
 * Student Number(UWE): 19045165
 * Module: Operating Systems
 * Module Code: UFCFWK-15-2
 * Year: 02
 * Student ID:(VC): 1802035
 * Note: Uncomment codes to execute and comment them when not in use.
 */

// Program start.

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
enum vga_color 
{
  // Colour codes for terminal writestrings and terminal colours.
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

// This function will choose colours for the terminal screen. 
uint8_t make_color(enum vga_color fg, enum vga_color bg) 
{
  // Sets red colour as background and foreground.
  return fg | bg << COLOR_RED;
}

// This function will choose colours for the terminal's buffers.
uint16_t make_vgaentry(char c, uint8_t color) 
{
  uint16_t c16 = c;
  uint16_t color16 = color;
  return c16 | color16 << COLOR_DARK_GREY;
}

// This function will validate strings. 
size_t strlen(const char* str) 
{
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

//static const size_t calculate_screen;

// Refers to termninal row.
size_t terminal_row;
// Refers to terminal column.
size_t terminal_column;
// Refers to terminal's colour.
uint8_t terminal_color;
// Refers to terminal's buffer.
uint16_t* terminal_buffer;
 
// This function will refresh the programs terminal screen.
void terminal_initialize() 
{
  // Variables declared referring to VGA characteristics.
  size_t screen_width = VGA_WIDTH;
  size_t screen_height = VGA_HEIGHT;

  terminal_row = 0;
  terminal_column = 0;
  terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
  terminal_buffer = (uint16_t*) 0xB8000;
  for (size_t y = 0; y < screen_height; y++) {
    for (size_t x = 0; x < screen_width; x++) {
      const size_t index = y * screen_width + x;
      terminal_buffer[index] = make_vgaentry(' ', terminal_color);
    }
  }
}
 
// This function will set the terminal's colours.
void terminal_setcolor(uint8_t color) 
{
 // Sets the termninal's colour.
  terminal_color = color;
}
 
// This function will display various characters on the terminal screen.
void terminal_putentryat(char f, uint8_t color, size_t x, size_t y) 
{
  size_t screen_width = VGA_WIDTH;
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = make_vgaentry(f, color);
}

// This function will display the current position of the cursor on the terminal screen.
void terminal_putchar(char f) 
{
  // Variables declared referring to VGA characteristics.
  size_t screen_width = VGA_WIDTH;
  size_t screen_height = VGA_HEIGHT;

  // This will check for a new line character on the terminal screen and calls the scroll function to be implemented.
  // "char f" refers to characters on the terminal screen.
  // Do not use " ("") " to check for "char f" as a parameter.
  if (f == '\n')
  {
    // Setting terminal column as zero.
    terminal_column = 0;
    //terminal_row++;
    // Calling the terminal scroll function if the terminal's row is equal to "VGA_HEIGHT".
    if (++terminal_row == screen_height)
    {
      // Calling the terminal scroll function.
      terminal_screen_vertical_scroll();
    }
  }
  else
  {
      // Calling function to display characters on the screen.
      terminal_putentryat(f, terminal_color, terminal_column, terminal_row);
  }

  // This checks for the width and height of the terminal.
  if (++terminal_column == screen_width) 
  {
    terminal_column = 0;
    if (++terminal_row == screen_height) 
    {
      // Calling the terminal scroll function function.
      terminal_screen_vertical_scroll();
    if (++terminal_row == screen_height)
    {
      terminal_row = 0;
    }
    }
  }
}
 
// This function will help print characters on the terminal screen.
void terminal_writestring(const char* data) 
{
  size_t datalen = strlen(data);
  for (size_t t = 0; t < datalen; t++)
    terminal_putchar(data[t]);
}

/*
void terminal_writestring_repeat()
{
  // Variable for the FOR Loop below.
  size_t g;

  // Prints 30 lines of the writestring.
  for(g = 0; g < 30; g++)
  { 
    // Calling color function.
    terminal_writestring_color_multiple();
    // Prints the message.
    terminal_writestring("Hello, kernel World!\n");
  }
}
*/

/*
void terminal_writestring_color_multiple()
{

}
*/


// This function will handle the scrolling in the terminal.
void terminal_screen_vertical_scroll()
{
  // This variable will calculate the width and height of the terminal screen to get the screen size. 
  size_t terminal_screen_size_calculate = VGA_WIDTH * VGA_HEIGHT;
  // This will check for the width and height of the terminal screen and add it to the buffer.
  size_t screen_width = VGA_WIDTH;
  // Variable declared for FOR loop below.
  size_t t;

  for (t = 0; t < terminal_screen_size_calculate; t++)
  {
    // Variable declared to calculate terminal buffer.
    size_t terminal_buffer_calculate = t + screen_width;
    terminal_buffer[t] = terminal_buffer[terminal_buffer_calculate];
  }
  // Decreases one row from the terminal screen.
  // "terminal_row -= 1;" and "terminal_row--;"" does the same thing.
  terminal_row--;
}

// Programs main function.
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main() 
{
  /* Initialize terminal interface */
  terminal_initialize();
  
  /* Since there is no support for newlines in terminal_putchar
   * yet, '\n' will produce some VGA specific character instead.
   * This is normal.
   */
  
  // Using for example "COLOR_WHITE" or colour code "15" to call for colours both work.
  // This will print writestring in the colour red once.
  //terminal_writestring_repeat();

  // Variables declared for the FOR loops below.
  size_t q, w, e, r;

  for(q = 0; q < 1; q++)
  { 
    // Selects the colour red.
    terminal_setcolor(4);
    // Prints the message.
    terminal_writestring("Hello, kernel World!\n");
  }

  // This will print writestring in the colour white six times.
  for (w = 0; w < 6; w++)
  {
    // Selects the colour white.
    terminal_setcolor(15);
    // Prints the message.
    terminal_writestring("Hello, kernel World!\n");
  }

  // This will print writestring in the colour blue eighteen times.
  for (e = 0; e < 18; e++)
  {
    // Selects the colour blue.
    terminal_setcolor(1);
    // Prints the message.
    terminal_writestring("Hello, kernel World!\n");
  }

  // This will print writestring in the colour green five times.
  for (r = 0; r < 5; r++)
  {
    // Selects the colour green.
    terminal_setcolor(2);
    // Prints the message.
    terminal_writestring("Hello, kernel World!\n");
  }
}

// Program end.
