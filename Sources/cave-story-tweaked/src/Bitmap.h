// Released under the MIT licence.
// See LICENCE.txt for details.

#pragma once

#include <stddef.h>

#include "WindowsWrapper.h"

typedef struct ColorFilter{
	const char* name;
	BOOL (*callback)(unsigned char *buffer, unsigned int index);
} ColorFilter;

extern ColorFilter gColorFilters[];
extern int gFilterIndex;
extern float gDimmingFactor;

unsigned char* DecodeBitmapWithAlpha(const unsigned char *in_buffer, size_t in_buffer_size, unsigned int *width, unsigned int *height, BOOL colour_key);
unsigned char* DecodeBitmapWithAlphaFromFile(const char *path, unsigned int *width, unsigned int *height, BOOL colour_key);
unsigned char* DecodeBitmap(const unsigned char *in_buffer, size_t in_buffer_size, size_t *width, size_t *height, unsigned int bytes_per_pixel);
unsigned char* DecodeBitmapFromFile(const char *path, size_t *width, size_t *height, unsigned int bytes_per_pixel);
void FreeBitmap(unsigned char *buffer);
BOOL DimBuffer(unsigned char *buffer, float dim_factor, size_t width, size_t height);