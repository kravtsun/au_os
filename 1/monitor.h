// monitor.h -- Defines the interface for monitor.h

#ifndef MONITOR_H
#define MONITOR_H


#include "common.h"


// Writes a single character out to the screen.
void monitor_put(char c);


// Clears the screen, by copying lots of spaces to the framebuffer.
void monitor_clear();


// Write a null-terminated ASCII string to the monitor.
void monitor_write(char *c);


#endif // MONITOR_H
