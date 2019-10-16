#ifndef GRAPHICS_LIBRARY_H_
#define GRAPHICS_LIBRARY_H_

#include <clib.h>
#include <types.h>
#include <pixelMap.h>

void drawRect(Vector2 start, Vector2 size, Color color);

void drawString(Vector2 pos, char * str, Color foreground, Color background);

#endif /* GRAPHICS_LIBRARY_H_ */
