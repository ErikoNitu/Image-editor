//312CA_NituErikoLaurentiu_Tema3
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headers.h"

void save_image(char command[50], FILE *file, rgb_t ***image, int w, int h,
				int max_pix, char format[3])
{
	int i, j, ascii = 0;
	char file_name[50];
	FILE *out;
	if (sscanf(command, "SAVE %s", file_name) != 1) {
		printf("Invalid command\n");
		return;
	}
	if (!(file)) {
		printf("No image loaded\n");
		return;
	} // checking if the image has to be saved ascii or binary
	if (strstr(command, "ascii") != 0) {
		ascii = 1;
	} // saving the header in the output image according to the format
	if (ascii) {
		out = fopen(file_name, "wt");
		if (!(out)) {
			fprintf(stderr, "Error opening image\n");
			printf("Failed to load %s\n", command);
		}
		if (strcmp(format, "P3") == 0 || strcmp(format, "P6") == 0) {
			fprintf(out, "P3\n");
		} else if (strcmp(format, "P2") == 0 || strcmp(format, "P5") == 0) {
			fprintf(out, "P2\n");
		}
		fprintf(out, "%d %d\n%d\n", w, h, max_pix);
	} else {
		out = fopen(file_name, "wb");
		if (!(out)) {
			fprintf(stderr, "Error opening image\n");
			printf("Failed to load %s\n", command);
		}
		if (strcmp(format, "P3") == 0 || strcmp(format, "P6") == 0) {
			fwrite("P6\n", 1, 3, out);
		} else if (strcmp(format, "P2") == 0 || strcmp(format, "P5") == 0) {
			fwrite("P5\n", 1, 3, out);
		}
		char buffer[20];
		sprintf(buffer, "%d", w);
		fwrite(&buffer, sizeof(char), strlen(buffer), out);
		fwrite(" ", 1, 1, out);
		sprintf(buffer, "%d", h);
		fwrite(&buffer, sizeof(char), strlen(buffer), out);
		fwrite("\n", 1, 1, out);
		sprintf(buffer, "%d", max_pix);
		fwrite(&buffer, sizeof(char), 3, out);
		fwrite("\n", 1, 1, out);
	} // saving in the output image the pixels according to the format
	for (i = 0; i < h; ++i) {
		for (j = 0; j < w; ++j) {
			if (ascii) {
				if (strcmp(format, "P2") == 0 ||
					strcmp(format, "P5") == 0) {
					fprintf(out, "%d ", (*image)[i][j].r);
				} else if (strcmp(format, "P3") == 0 ||
						   strcmp(format, "P6") == 0) {
					fprintf(out, "%d %d %d ", (*image)[i][j].r,
							(*image)[i][j].g, (*image)[i][j].b);
				}
			} else {
				if (strcmp(format, "P2") == 0 || strcmp(format, "P5") == 0) {
				    fwrite(&(*image)[i][j].r, 1, 1, out);
				} else if (strcmp(format, "P3") == 0 ||
						   strcmp(format, "P6") == 0) {
					fwrite(&(*image)[i][j].r, 1, 1, out);
					fwrite(&(*image)[i][j].g, 1, 1, out);
					fwrite(&(*image)[i][j].b, 1, 1, out);
				}
			}
		}
		fflush(out);
		if (ascii) {
			fprintf(out, "\n");
		}
	}
	printf("Saved %s\n", file_name);
	fclose(out);
}
