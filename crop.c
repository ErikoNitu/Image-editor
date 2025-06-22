//312CA_NituErikoLaurentiu_Tema3
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headers.h"

void crop_image(FILE *file, rgb_t ***image, int *w, int *h, int *x1, int *y1,
				int *x2, int *y2)
{
	if (!(file)) {
		printf("No image loaded\n");
		return;
	}
	// we use a image copy to save the pixel values and then we reallocate
	// our image with the cropped dimensions, then we save the pixels in the
	// cropped image
	int i, j;
	int fh = *h;
	rgb_t **all_image;
	all_image = malloc(*h * sizeof(rgb_t *));
	if (!all_image) {
		fprintf(stderr, "Error allocation");
	}
	for (i = 0; i < *h; ++i) {
		all_image[i] = malloc(*w * sizeof(rgb_t));
		if (!all_image[i]) {
			fprintf(stderr, "Error allocation");
		}
	}
	for (i = 0; i < *h; ++i) {
		for (j = 0; j < *w; ++j) {
			all_image[i][j].r = (*image)[i][j].r;
			all_image[i][j].g = (*image)[i][j].g;
			all_image[i][j].b = (*image)[i][j].b;
		}
	}

	free_mat(&(*image), *h);
	// updating the height and width with the new values
	*w = *x2 - *x1;
	*h = *y2 - *y1;
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
	for (i = 0; i < *h; ++i) {
		for (j = 0; j < *w; ++j) {
			(*image)[i][j].r = all_image[i + *y1][j + *x1].r;
			(*image)[i][j].g = all_image[i + *y1][j + *x1].g;
			(*image)[i][j].b = all_image[i + *y1][j + *x1].b;
		}
	}
	free_mat(&all_image, fh);
	// updating the selection coordinates
	*x1 = 0;
	*x2 = *w;
	*y1 = 0;
	*y2 = *h;
	printf("Image cropped\n");
}
