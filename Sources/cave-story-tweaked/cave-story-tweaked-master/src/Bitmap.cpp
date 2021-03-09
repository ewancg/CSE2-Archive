// Released under the MIT licence.
// See LICENCE.txt for details.

#include "Bitmap.h"

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#define STBI_ONLY_BMP
#define STBI_ONLY_PNG
#define STBI_NO_LINEAR
#define STBI_NO_STDIO
#include "../external/stb_image.h"

#include "WindowsWrapper.h"

#include "Draw.h"
#include "File.h"
#include "Generic.h"

int gFilterIndex;
float gDimmingFactor = 1;

// Color filters definitions
BOOL CallbackGrayscale(unsigned char *buffer, unsigned int index){
	const unsigned char r = buffer[index];
	const unsigned char g = buffer[index+1];
	const unsigned char b = buffer[index+2];

	//compute luminance
	double r_float = (1.0*r)/255;
	double g_float = (1.0*g)/255;
	double b_float = (1.0*b)/255;

	float lum = (r_float + g_float + b_float)/3;

	lum = lum*255;

	buffer[index] = lum;
	buffer[index+1] = lum;
	buffer[index+2] = lum;
	
	return TRUE;

}

BOOL CallbackGenesis(unsigned char *buffer, unsigned int index){
	const unsigned char r = buffer[index];
	const unsigned char g = buffer[index+1];
	const unsigned char b = buffer[index+2];


	unsigned int r_gen = (r+8)*8/256;
	unsigned int g_gen = (g+8)*8/256;
	unsigned int b_gen = (b+8)*8/256;

	if(r_gen == 8){
		r_gen = 7;
	}
	if(g_gen == 8){
		g_gen = 7;
	}
	if(b_gen == 8){
		b_gen = 7;
	}

	const unsigned int colors[]={
		0,52,87,116,144,172,206,255
	};

	buffer[index] = colors[r_gen];
	buffer[index+1]= colors[g_gen];
	buffer[index+2] = colors[b_gen];

	return TRUE;
}

BOOL CallbackMSX(unsigned char *buffer, unsigned int index){
	const unsigned char r = buffer[index];
	const unsigned char g = buffer[index+1];
	const unsigned char b = buffer[index+2];

	unsigned int r_gen = r*8/256;
	unsigned int g_gen = g*8/256;
	unsigned int b_gen = b*4/256;

	buffer[index] = r_gen*256/8 + 16;
	buffer[index+1] = g_gen*256/8 + 16;
	buffer[index+2] = b_gen*256/4 + 32;

	return TRUE;
}


BOOL CallbackGameBoy(unsigned char *buffer, unsigned int index){
	const unsigned char r = buffer[index];
	const unsigned char g = buffer[index+1];
	const unsigned char b = buffer[index+2];
	//compute luminance
	double lum = 4.0*(r + g + b)/(3*256);

	int color = lum;

	unsigned int colors[] = {
		12,54,14,
		44,98,52,
		110,135,10,
		156,190,12,
	};

	buffer[index] = colors[3*color];
	buffer[index+1] = colors[3*color+1];
	buffer[index+2] = colors[3*color+2];
	
	return TRUE;
}

BOOL CallbackProtanopia(unsigned char *buffer, unsigned int index){
	const unsigned char r = buffer[index];
	const unsigned char g = buffer[index+1];
	const unsigned char b = buffer[index+2];

	const double R[] = {56.667, 43.333, 0};
    const double G[] = {55.833, 44.167, 0};
    const double B[] = {0, 24.167, 75.833};

	buffer[index] = r*R[0]/100 + g*R[1]/100 + b*R[2]/100;
	buffer[index+1] = r*G[0]/100 + g*G[1]/100 + b*G[2]/100;
	buffer[index+2] = r*B[0]/100 + g*B[1]/100 + b*B[2]/100;
	
	return TRUE;
}

BOOL CallbackProtanomaly(unsigned char *buffer, unsigned int index){
	const unsigned char r = buffer[index];
	const unsigned char g = buffer[index+1];
	const unsigned char b = buffer[index+2];

	const double R[] = {81.667, 18.333, 0};
    const double G[] = {33.333, 66.667, 0};
    const double B[] = {0, 12.5, 87.5};

	buffer[index] = r*R[0]/100 + g*R[1]/100 + b*R[2]/100;
	buffer[index+1] = r*G[0]/100 + g*G[1]/100 + b*G[2]/100;
	buffer[index+2] = r*B[0]/100 + g*B[1]/100 + b*B[2]/100;

	return TRUE;
}

BOOL CallbackDeuteranomaly(unsigned char *buffer, unsigned int index){
	const unsigned char r = buffer[index];
	const unsigned char g = buffer[index+1];
	const unsigned char b = buffer[index+2];

	const double R[] = {80, 20, 0};
    const double G[] = {25.833, 74.167, 0};
    const double B[] = {0, 14.167, 85.833};

	buffer[index] = r*R[0]/100 + g*R[1]/100 + b*R[2]/100;
	buffer[index+1] = r*G[0]/100 + g*G[1]/100 + b*G[2]/100;
	buffer[index+2] = r*B[0]/100 + g*B[1]/100 + b*B[2]/100;

	return TRUE;
}

BOOL CallbackDeuteranopia(unsigned char *buffer, unsigned int index){
	const unsigned char r = buffer[index];
	const unsigned char g = buffer[index+1];
	const unsigned char b = buffer[index+2];

	const double R[] = {62.5, 37.5, 0};
    const double G[] = {70, 30, 0};
    const double B[] = {0, 30, 70};

	buffer[index] = r*R[0]/100 + g*R[1]/100 + b*R[2]/100;
	buffer[index+1] = r*G[0]/100 + g*G[1]/100 + b*G[2]/100;
	buffer[index+2] = r*B[0]/100 + g*B[1]/100 + b*B[2]/100;

	return TRUE;
}

BOOL CallbackTritanomaly(unsigned char *buffer, unsigned int index){
	const unsigned char r = buffer[index];
	const unsigned char g = buffer[index+1];
	const unsigned char b = buffer[index+2];

	const double R[] = {96.667, 3.333, 0};
    const double G[] = {0, 73.333, 26.667};
    const double B[] = {0, 18.333, 81.667};

	buffer[index] = r*R[0]/100 + g*R[1]/100 + b*R[2]/100;
	buffer[index+1] = r*G[0]/100 + g*G[1]/100 + b*G[2]/100;
	buffer[index+2] = r*B[0]/100 + g*B[1]/100 + b*B[2]/100;

	return TRUE;
}

BOOL CallbackTritanopia(unsigned char *buffer, unsigned int index){
	const unsigned char r = buffer[index];
	const unsigned char g = buffer[index+1];
	const unsigned char b = buffer[index+2];

	const double R[] = {95, 5, 0};
    const double G[] = {0, 43.333, 56.667};
    const double B[] = {0, 47.5, 52.5};

	buffer[index] = r*R[0]/100 + g*R[1]/100 + b*R[2]/100;
	buffer[index+1] = r*G[0]/100 + g*G[1]/100 + b*G[2]/100;
	buffer[index+2] = r*B[0]/100 + g*B[1]/100 + b*B[2]/100;

	return TRUE;
}

ColorFilter gColorFilters[] = {
	{"Off", NULL},
	{"Protanomaly", CallbackProtanomaly},
	{"Protanopia", CallbackProtanopia},
	{"Deuteranomaly", CallbackDeuteranomaly},
	{"Deuteranopia", CallbackDeuteranopia},
	{"Tritanomaly", CallbackTritanomaly},
	{"Tritanopia", CallbackTritanopia},
	{"Grayscale", CallbackGrayscale},
	{"Genesis", CallbackGenesis},
	{"MSX", CallbackMSX},
	{"Game Boy", CallbackGameBoy},
};


unsigned char* DecodeBitmap(const unsigned char *in_buffer, size_t in_buffer_size, size_t *width, size_t *height, unsigned int bytes_per_pixel)
{
	int int_width, int_height;
	unsigned char *image_buffer = stbi_load_from_memory(in_buffer, in_buffer_size, &int_width, &int_height, NULL, bytes_per_pixel);

	*width = int_width;
	*height = int_height;

	// apply color filter if it exists
	if(gFilterIndex != 0){
		for (size_t i = 0; i < *width * *height; ++i){
			gColorFilters[gFilterIndex].callback(image_buffer, 4*i);
		}
	}
	
	return image_buffer;
}

unsigned char* DecodeBitmapFromFile(const char *path, size_t *width, size_t *height, unsigned int bytes_per_pixel)
{
	size_t file_size;
	unsigned char *file_buffer = LoadFileToMemory(path, &file_size);

	if (file_buffer != NULL)
	{
		unsigned char *image_buffer = DecodeBitmap(file_buffer, file_size, width, height, bytes_per_pixel);

		free(file_buffer);

		return image_buffer;
	}

	return NULL;
}

void FreeBitmap(unsigned char *buffer)
{
	stbi_image_free(buffer);
}

BOOL DimBuffer(unsigned char *buffer, float dim_factor, size_t width, size_t height){
	for (size_t i = 0; i < width*height; i++){
		unsigned char color_r = (buffer)[4*i];
		unsigned char color_g = (buffer)[4*i+1];
		unsigned char color_b = (buffer)[4*i+2];
		unsigned char color_a = (buffer)[4*i+3];

		unsigned char new_color_r = color_r*dim_factor;
		unsigned char new_color_g = color_g*dim_factor;
		unsigned char new_color_b = color_b*dim_factor;

		(buffer)[4*i] = new_color_r;
		(buffer)[4*i+1] = new_color_g;
		(buffer)[4*i+2] = new_color_b;
		(buffer)[4*i+3] = color_a;
	}
	
	return true;
}