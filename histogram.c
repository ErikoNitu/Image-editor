//312CA_NituErikoLaurentiu_Tema3
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headers.h"

void histogram(char command[50], FILE *file, rgb_t **image, int w, int h,
			   char format[3])
{
	int i, j;
	int x, y;
	int poz = 0;
	int f[256] = {0};

	if (!(file)) {
		printf("No image loaded\n");
		return;
	}
	// veryfing if exactly two numbers were read after the command HISTOGRAM
	if (sscanf(command, "HISTOGRAM %d %d %n", &x, &y, &poz) == 2) {
		if (command[poz] != '\0') {
			printf("Invalid command\n");
			return;
		}
		// veryfing if the image format is correct
		if (strcmp(format, "P3") == 0 || strcmp(format, "P6") == 0) {
			printf("Black and white image needed\n");
			return;
		}
		// calculating every pixel's frequency
		for (i = 0; i < h; ++i) {
			for (j = 0; j < w; ++j) {
				f[image[i][j].r]++;
			}
		}
		// determining the dimension of one bin
		int size = 256 / y;
		// calculating the maximum frequency of the bins
		int max_f = 0;
		for (i = 0; i < 256; i += size) {
			int s = 0;
			for (j = 0; j < size; ++j) {
				s += f[i + j];

			}
			if (s > max_f) {
				max_f = s;
			}
		}
		//printing the histogram
		for (i = 0; i < 256; i += size) {
			int s = 0;
			for (j = 0; j < size; ++j) {
				s += f[i + j];

			}
			int star = (double)1.0 * (s * x) / max_f;
			printf("%d\t|\t", star);
			for (int t = 0; t < star; ++t) {
				printf("*");
			}
			printf("\n");
		}
	} else {
		// error case
		printf("Invalid command\n");
	}
}
