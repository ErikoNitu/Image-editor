//312CA_NituErikoLaurentiu_Tema3
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headers.h"

void equalize_image(FILE *file, rgb_t ***image, int w, int h, char format[3])
{
	if (!(file)) {
		printf("No image loaded\n");
		return;
	}
	// checking if the image format is correct
	if (strcmp(format, "P3") == 0 || strcmp(format, "P6") == 0) {
		printf("Black and white image needed\n");
		return;
	}
	int i, j;
	int area = h * w;
	int f[256] = {0}, s_f[256];
	// determining each pixel's frequency
	for (i = 0; i < h; ++i) {
		for (j = 0; j < w; ++j) {
			f[(*image)[i][j].r]++;
		}
	}
	// we determine the sum of the frequencies of all pixels smaller than the
	// current pixel
	s_f[0] = f[0];
	for (i = 1; i < 256; ++i) {
		s_f[i] = s_f[i - 1] + f[i];
	}
	// using a temporary matrix to calculate the equalized image
	float **eq_image;
	eq_image = malloc(h * sizeof(float *));
	if (!eq_image) {
		fprintf(stderr, "Error allocation");
	}
	for (i = 0; i < h; ++i) {
		eq_image[i] = malloc(w * sizeof(float));
		if (!eq_image[i]) {
			fprintf(stderr, "Error allocation");
		}
	}
	// applying the formula on each pixel
	for (i = 0; i < h; ++i) {
		for (j = 0; j < w; ++j) {
			double buffer = (double)(255.0 * s_f[(*image)[i][j].r]) / area;
			eq_image[i][j] = buffer;
		}
	}
	// updating the initial matrix with the modified values and keeping
	// each pixel between 0 and 255
	for (i = 0; i < h; ++i) {
		for (j = 0; j < w; ++j) {
			(*image)[i][j].r = clamp(eq_image[i][j], 0, 255);
		}
	}

	// freeing the memory for the temporary matrix
	for (i = 0; i < h; ++i) {
		free(eq_image[i]);
	}
	free(eq_image);

	printf("Equalize done\n");
}
