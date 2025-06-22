//312CA_NituErikoLaurentiu_Tema3
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headers.h"

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

int angle_check(int angle)
{
	if (angle == 90 || angle == -90 ||
		angle == 180 || angle == -180 ||
		angle == 270 || angle == -270 ||
		angle == 360 || angle == -360 || angle == 0) {
		return 1;
	}
	return 0;
}

void rotate_image(char command[50], FILE *file, rgb_t ***image, int *w, int *h,
				  int *x1, int *y1, int *x2, int *y2)
{

	int angle;
	// checking if the angle was read
	if (sscanf(command, "ROTATE %d", &angle) != 1) {
		printf("Invalid command\n");
		return;
	}

	if (!(file)) {
		printf("No image loaded\n");
		return;
	}
	// checking if the angle is valid
	if (!angle_check(angle)) {
		printf("Unsupported rotation angle\n");
		return;
	}
	// determining the selection height and width
	int sh = *y2 - *y1;
	int sw = *x2 - *x1;
	// calculating how many rotations are made according to the angle
	int r = angle / 90;
	if (r < 0) {
		r = -r;
	}
	// if the current selection is the enitre image we rotate the whole image
	if ((*h == *y2  && *y1 == 0 && *w == *x2 &&  *x1 == 0) ||
	    (*h == *x2  && *x1 == 0 && *w == *y2 &&  *y1 == 0)) {
		for (int i = 0; i < r; ++i) {
			// if the angle is negative we rotate to the left and if
			// is positive we rotate to the righr
			if (angle < 0) {
				rotate_whole_image_left(&(*image), &(*w), &(*h), &(*x1),
										&(*y1), &(*x2), &(*y2));
			} else {
				rotate_whole_image_right(&(*image), &(*w), &(*h), &(*x1),
										 &(*y1), &(*x2), &(*y2));
			}
		}
	} else {
		// the selection is inside the image

		// checking if the selection is square
		if (sw != sh) {
			printf("The selection must be square\n");
			return;
		}
		for (int i = 0; i < r; ++i) {
			if (angle < 0) {
				rotate_left(&(*image), sw, sh, *x1, *y1);
			} else {
				rotate_right(&(*image), sw, sh, *x1, *y1);
			}
		}
	}
	printf("Rotated %d\n", angle);
}

// function which rotates a inner selection to the right
void rotate_right(rgb_t ***image, int sw, int sh, int x1, int y1)
{
	int i, j;

	rgb_t **rotated;
	rotated = malloc(sh * sizeof(rgb_t *));
	if (!rotated) {
		fprintf(stderr, "Error allocation");
	}
	for (i = 0; i < sh; ++i) {
		rotated[i] = malloc(sw * sizeof(rgb_t));
		if (!rotated[i]) {
			fprintf(stderr, "Error allocation");
		}
	}

	for (i = 0; i < sh; ++i) {
		for (j = 0; j < sw; ++j) {
			// rotating the selection
			rotated[j][sw - 1 - i].r = (*image)[i + y1][j + x1].r;
			rotated[j][sw - 1 - i].g = (*image)[i + y1][j + x1].g;
			rotated[j][sw - 1 - i].b = (*image)[i + y1][j + x1].b;
		}
	}
	// selection = rotated
	for (i = 0; i < sh; ++i) {
		for (j = 0; j < sw; ++j) {
			(*image)[i + y1][j + x1].r = rotated[i][j].r;
			(*image)[i + y1][j + x1].g = rotated[i][j].g;
			(*image)[i + y1][j + x1].b = rotated[i][j].b;
		}
	}

	free_mat(&rotated, sh);
}

// function which rotates a inner selection to the left
void rotate_left(rgb_t ***image, int sw, int sh, int x1, int y1)
{
	int i, j;

	rgb_t **rotated;
	rotated = malloc(sh * sizeof(rgb_t *));
	if (!rotated) {
		fprintf(stderr, "Error allocation");
	}
	for (i = 0; i < sh; ++i) {
		rotated[i] = malloc(sw * sizeof(rgb_t));
		if (!rotated[i]) {
			fprintf(stderr, "Error allocation");
		}
	}

	for (i = 0; i < sh; ++i) {
		for (j = 0; j < sw; ++j) {
			// rotating the selection
			rotated[sw - 1 - j][i].r = (*image)[i + y1][j + x1].r;
			rotated[sw - 1 - j][i].g = (*image)[i + y1][j + x1].g;
			rotated[sw - 1 - j][i].b = (*image)[i + y1][j + x1].b;
		}
	}
	// selection = rotated
	for (i = 0; i < sh; ++i) {
		for (j = 0; j < sw; ++j) {
			(*image)[i + y1][j + x1].r = rotated[i][j].r;
			(*image)[i + y1][j + x1].g = rotated[i][j].g;
			(*image)[i + y1][j + x1].b = rotated[i][j].b;
		}
	}

	free_mat(&rotated, sh);
}

// in order to rotate the whole image to the right or to the left we allocated
// a temporary matrix with the height equal to the initial width and the witdh
// equal to the initial height
void rotate_whole_image_right(rgb_t ***image, int *w, int *h, int *x1, int *y1,
							  int *x2, int *y2)
{
	int i, j;
	rgb_t **rotated;
	rotated = malloc(*w * sizeof(rgb_t *));
	if (!rotated) {
		fprintf(stderr, "Error allocation");
	}
	for (i = 0; i < *w; ++i) {
		rotated[i] = malloc(*h * sizeof(rgb_t));
		if (!rotated[i]) {
			fprintf(stderr, "Error allocation");
		}
	}

	for (i = 0; i < *h; ++i) {
		for (j = 0; j < *w; ++j) {
			// rotating the image
			rotated[j][*h - 1 - i].r = (*image)[i][j].r;
			rotated[j][*h - 1 - i].g = (*image)[i][j].g;
			rotated[j][*h - 1 - i].b = (*image)[i][j].b;
		}
	}
	free_mat(&(*image), *h);
	// swapping the height and width
	swap(&(*h), &(*w));
	//image = rotated;
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
			(*image)[i][j].r = rotated[i][j].r;
			(*image)[i][j].g = rotated[i][j].g;
			(*image)[i][j].b = rotated[i][j].b;
		}
	}

	free_mat(&rotated, *h);
	//updating the image coordinates
	*x1 = 0;
	*y1 = 0;
	swap(&(*y2), &(*x2));
}

void rotate_whole_image_left(rgb_t ***image, int *w, int *h, int *x1,
							 int *y1, int *x2, int *y2)
{
	int i, j;

	rgb_t **rotated;
	rotated = malloc(*w * sizeof(rgb_t *));
	if (!rotated) {
		fprintf(stderr, "Error allocation");
	}
	for (i = 0; i < *w; ++i) {
		rotated[i] = malloc(*h * sizeof(rgb_t));
		if (!rotated[i]) {
			fprintf(stderr, "Error allocation");
		}
	}

	for (i = 0; i < *h; ++i) {
		for (j = 0; j < *w; ++j) {
			// rotating the image
			rotated[*w - 1 - j][i].r = (*image)[i][j].r;
			rotated[*w - 1 - j][i].g = (*image)[i][j].g;
			rotated[*w - 1 - j][i].b = (*image)[i][j].b;
		}
	}
	free_mat(&(*image), *h);
	// swapping the height and width
	swap(&(*w), &(*h));
	// image = rotated
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
			(*image)[i][j].r = rotated[i][j].r;
			(*image)[i][j].g = rotated[i][j].g;
			(*image)[i][j].b = rotated[i][j].b;
		}
	}
	// updating the image coordinatess
	*x1 = 0;
	*y1 = 0;
	swap(&(*y2), &(*x2));
	free_mat(&rotated, *h);
}
