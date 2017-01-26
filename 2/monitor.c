// monitor.c -- Functions writing to the monitor.

#include "monitor.h"

// The VGA framebuffer starts at 0xB8000.
static uint16_t *const video_memory = (uint16_t *)0xB8000;
// Stores the cursor position.
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static const int MONITOR_ROWS = 25;
static const int MONITOR_COLS = 80;


// Updates the hardware cursor.
static void move_cursor() {
    // The screen is 80 characters wide...
    uint16_t location = cursor_y * MONITOR_COLS + cursor_x;
    outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, location >> 8); // Send the high cursor byte.
    outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, location);      // Send the low cursor byte.
}


// Scrolls the text on the screen up by one line.
static void scroll() {
    // Get a space character with the default colour attributes.
    uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

    // Row 25 is the end, this means we need to scroll up
    if(cursor_y >= MONITOR_ROWS) {
        // Move the current text chunk that makes up the screen
        // back in the buffer by a line
        int i;
        for (i = 0*MONITOR_COLS; i < (MONITOR_ROWS-1)*MONITOR_COLS; i++) {
            video_memory[i] = video_memory[i+MONITOR_COLS];
        }

        // The last line should now be blank. Do this by writing
        // 80 spaces to it.
        for (i = (MONITOR_ROWS-1)*MONITOR_COLS; i < MONITOR_ROWS*MONITOR_COLS; i++) {
            video_memory[i] = blank;
        }
        // The cursor should now be on the last line.
        cursor_y = MONITOR_ROWS-1;
    }
}


void monitor_put(char c) {
    // The background colour is black (0), the foreground is white (15).
    uint8_t backColour = 0;
    uint8_t foreColour = 15;

    // The attribute byte is made up of two nibbles - the lower being the
    // foreground colour, and the upper the background colour.
    uint8_t  attributeByte = (backColour << 4) | (foreColour & 0x0F);
    // The attribute byte is the top 8 bits of the word we have to send to the
    // VGA board.
    uint16_t attribute = attributeByte << 8;
    uint16_t *location;

    const int interrupts_enabled = are_interrupts_enabled();
    switch (c) {

    // Handle a backspace, by moving the cursor back one space
    case 0x08:
        if (cursor_x) {
            cursor_x--;
        }
        break;

    // Handle a tab by increasing the cursor's X,
    // but only to a point where it is divisible by 8.
    case 0x09:
        cursor_x = (cursor_x+8) & ~(8-1);
        break;

    // Handle carriage return
    case '\r':
        cursor_x = 0;
        break;

    // Handle newline by moving cursor back to left and increasing the row
    case '\n':
        cursor_x = 0;
        cursor_y++;
        break;

    // Handle any other printable character.
    default:
        location = video_memory + (cursor_y*MONITOR_COLS + cursor_x);

        if (interrupts_enabled) {
            asm volatile("cli");
        }
        *location = c | attribute;
        if (interrupts_enabled) {
            asm volatile("sti");
        }
        cursor_x++;
        break;
    }

    // Check if we need to insert a new line
    // because we have reached the end of the screen.
    if (cursor_x >= MONITOR_COLS) {
        cursor_x = 0;
        cursor_y++;
    }


    if (interrupts_enabled) {
        asm volatile("cli");
    }
    // Scroll the screen if needed.
    scroll();
    // Move the hardware cursor.
    move_cursor();
    if (interrupts_enabled) {
        asm volatile("sti");
    }
}


void monitor_clear() {
    // Make an attribute byte for the default colours
    uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

    int i;
    const int interrupts_enabled = are_interrupts_enabled();
    if (interrupts_enabled) {
        asm volatile("cli");
    }
    for (i = 0; i < MONITOR_COLS*MONITOR_ROWS; i++) {
        video_memory[i] = blank;
    }
    if (interrupts_enabled) {
        asm volatile("sti");
    }

    // Move the hardware cursor back to the start.
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}


void monitor_write(const char *c) {
    // asm volatile("cli");
    while (*c) monitor_put(*c++);
    // asm volatile("sti");
}


static int align_number = -1;
static char monitor_buf[100];
void monitor_write_common_number(uint32_t num, uint32_t base) {
    int cnt = 0;

    while (num) {
        uint32_t rem = num % base;
        if (rem > 9) {
            monitor_buf[cnt++] = 'a' + (rem - 10);
        }
        else {
            monitor_buf[cnt++] = '0' + rem;
        }
        num /= base;
    }
    if (cnt == 0) {
        monitor_buf[cnt++] = '0';
    }
    while (align_number != -1 && base == 16 && cnt < align_number) {
        monitor_buf[cnt++] = '0';
    }
    int i = 0;
    for (i = 0; i < cnt/2; ++i) {
        char tmp = monitor_buf[i];
        monitor_buf[i] = monitor_buf[cnt-i-1];
        monitor_buf[cnt-i-1] = tmp;
    }
    monitor_buf[cnt++] = 0;
    monitor_write(monitor_buf);
}


void monitor_debug(const char *str, uint32_t num) {
    monitor_write(str);
    monitor_write_number(num);
    monitor_write("\n");
}

void monitor_write_big_number(uint64_t num) {
    const uint64_t num_low = num & 0xFFFFFFFF;
    const uint64_t num_high = num >> 32;
    if (num_high) {
        monitor_write_number(num_high);
        align_number = 8;
        monitor_write_number(num_low);
        align_number = -1;
    }
    else {
        monitor_write_number(num_low);
    }
}
