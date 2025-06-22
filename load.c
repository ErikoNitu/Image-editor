//312CA_NituErikoLaurentiu_Tema3
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headers.h"

void load_image(char command[50], FILE **file, rgb_t ***image, int *w, int *h,
				int *max_pix, char format[3], int *ok)
{
	int i, j;
	char file_name[30] = {0}; unsigned char pixel;
	// veryfing if the file name is valid:
	// we use "ok" in order to know if we need to free the memory
	// before another LOAD command in main.c
	// (if no image was loaded, there is no need to free the memory)
	if (sscanf(command, "LOAD %s", file_name) == 1) {
		*file = fopen(file_name, "rb");
	} else {
		printf("No image loaded\n");
		*ok = 0;
		return;
	}
	if (!(*file)) {
		printf("Failed to load %s\n", file_name);
		*ok = 0;
		return;
	}
	// the image was successfully loaded
	*ok = 1;
	// reading the file's format, width, height and max pixel britghness
	fscanf(*file, "%s", format);
	format[2] = '\0';
	fscanf(*file, "%d", w);
	fscanf(*file, "%d", h);
	fscanf(*file, "%d", max_pix);
	//memory allocation for the image matrix and reading the pixels according
	//to the format of each file (binary or ascii)
	*image = malloc(*h * sizeof(rgb_t *));
	if (!(*image)) {
		fprintf(stderr, "Error allocation");
	}
	for (i = 0; i < *h; ++i) {
		(*image)[i] = malloc(*w * sizeof(rgb_t));
		if (!(*image)[i]) {
			fprintf(stderr, "Error allocation");
		}
	}
	if (strcmp(format, "P2") == 0) {
		for (i = 0; i < *h; ++i) {
			for (j = 0; j < *w; ++j) {
				fscanf(*file, "%d", &(*image)[i][j].r);
				(*image)[i][j].g = 0;
				(*image)[i][j].b = 0;
			}
		}
	} else if (strcmp(format, "P3") == 0) {
		for (i = 0; i < *h; ++i) {
			for (j = 0; j < *w; ++j) {
				fscanf(*file, "%d%d%d", &(*image)[i][j].r, &(*image)[i][j].g,
					   &(*image)[i][j].b);
			}
		}
	} else if (strcmp(format, "P5") == 0) {
		fread(&pixel, 1, 1, *file);
		for (i = 0; i < *h; ++i) {
			for (j = 0; j < *w; ++j) {
				fread(&pixel, 1, 1, *file);
				(*image)[i][j].r = pixel;
				(*image)[i][j].g = 0;
				(*image)[i][j].b = 0;
			}
		}
	} else if (strcmp(format, "P6") == 0) {
		fread(&pixel, 1, 1, *file);
		for (i = 0; i < *h; ++i) {
			for (j = 0; j < *w; ++j) {
				fread(&pixel, 1, 1, *file);
				(*image)[i][j].r = pixel;
				fread(&pixel, 1, 1, *file);
				(*image)[i][j].g = pixel;
				fread(&pixel, 1, 1, *file);
				(*image)[i][j].b = pixel;
			}
		}
	}
	printf("Loaded %s\n", file_name);
}
