//312CA_NituErikoLaurentiu_Tema3
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headers.h"
#include <math.h>

int clamp(int x, int min, int max)
{
	if (x < min) {
		return min;
	}
	if (x > max) {
		return max;
	}
	return x;
}

void apply_image(char command[50], FILE *file, rgb_t ***image, int x1, int y1,
				 int x2, int y2, int h, int w, char format[3])
{
	char effect[20];
	if (!(file)) {
		printf("No image loaded\n");
		return;
	}
	// checking if the command's parameter is valid
	if (sscanf(command, "APPLY %s", effect) == 1) {
		if (strcmp(format, "P2") == 0 || strcmp(format, "P5") == 0) {
			printf("Easy, Charlie Chaplin\n");
			return;
		}
	} else {
		printf("Invalid command\n");
		return;
	}
	// calling the functions to apply the effects
	if (strcmp(effect, "SHARPEN") == 0) {
		apply_sharpen(&(*image), x1, y1, x2, y2, h, w);
	} else if (strcmp(effect, "BLUR") == 0) {
		apply_blur(&(*image), x1, y1, x2, y2, h, w);
	} else if (strcmp(effect, "GAUSSIAN_BLUR") == 0) {
		apply_gaussian_blur(&(*image), x1, y1, x2, y2, h, w);
	} else if (strcmp(effect, "EDGE") == 0) {
		apply_edge(&(*image), x1, y1, x2, y2, h, w);
	} else {
		printf("APPLY parameter invalid\n");
		return;
	}

	printf("APPLY %s done\n", effect);

}

// for each function we used a buffer matrix in which we determinated the image
// with the effect applied and then copied it in the initial image
void apply_sharpen(rgb_t ***image, int x1, int y1, int x2, int y2, int h, int w)
{
	int i, j;
	rgb_t s, **buffer;
	// calculating the selection coordinates
	int sh = y2 - y1;
	int sw = x2 - x1;
	buffer = malloc(sh * sizeof(rgb_t *));
	if (!buffer) {
		fprintf(stderr, "Error allocation");
	}
	for (i = 0; i < sh; ++i) {
		buffer[i] = malloc(sw * sizeof(rgb_t));
		if (!buffer[i]) {
			fprintf(stderr, "Error allocation");
		}
	}
	// kernel image
	int sharp[3][3] = {
		{0, -1, 0},
		{-1, 5, -1},
		{0, -1, 0}
	};
	// aplying the kernel
	for (i = 0; i < sh; ++i) {
		for (j = 0; j < sw; ++j) {
			// if the effect is applied on the entire image we skip
			// the border
			if (i + y1 == 0 || i + y1 == h - 1 || j + x1 == 0 ||
				j + x1 == w - 1) {
				buffer[i][j].r = (*image)[i + y1][j + x1].r;
				buffer[i][j].g = (*image)[i + y1][j + x1].g;
				buffer[i][j].b = (*image)[i + y1][j + x1].b;
				continue;
			}
			s.r = 0;
			s.g = 0;
			s.b = 0;
			for (int k = 0; k < 3; ++k) {
				for (int t = 0; t < 3; ++t) {
					int a = i - 1 + k + y1;
					int b = j - 1 + t + x1;
					if (a >= 0 && a < h && b >= 0 && b < w) {
						s.r += (*image)[a][b].r * sharp[k][t];
						s.g += (*image)[a][b].g * sharp[k][t];
						s.b += (*image)[a][b].b * sharp[k][t];
					}
				}
			}
			// keeping each pixel value between 0 and 255
			buffer[i][j].r = clamp(s.r, 0, 255);
			buffer[i][j].g = clamp(s.g, 0, 255);
			buffer[i][j].b = clamp(s.b, 0, 255);
		}
	}
	// updating the selection with the modified values
	for (i = 0; i < sh; ++i) {
		for (j = 0 ; j < sw; ++j) {
			(*image)[i  + y1][j + x1].r = buffer[i][j].r;
			(*image)[i  + y1][j + x1].g = buffer[i][j].g;
			(*image)[i  + y1][j + x1].b = buffer[i][j].b;
		}
	}

	free_mat(&buffer, y2 - y1);
}

void apply_blur(rgb_t ***image, int x1, int y1, int x2, int y2, int h, int w)
{
	int i, j;
	rgb_t s, **buffer;
	// calculating the selection coordinates
	int sh = y2 - y1;
	int sw = x2 - x1;
	buffer = malloc(sh * sizeof(rgb_t *));
	if (!buffer) {
		fprintf(stderr, "Error allocation");
	}
	for (i = 0; i < sh; ++i) {
		buffer[i] = malloc(sw * sizeof(rgb_t));
		if (!buffer[i]) {
			fprintf(stderr, "Error allocation");
		}
	}
	// kernel image
	int blur[3][3] = {
		{1, 1, 1},
		{1, 1, 1},
		{1, 1, 1}
	};
	// aplying the kernel
	for (i = 0; i < sh; ++i) {
		for (j = 0; j < sw; ++j) {
			if (i + y1 == 0 || i + y1 == h - 1 || j + x1 == 0 ||
				j + x1 == w - 1) {
				// if the effect is applied on the entire image we skip
				// the border
				buffer[i][j].r = (*image)[i + y1][j + x1].r;
				buffer[i][j].g = (*image)[i + y1][j + x1].g;
				buffer[i][j].b = (*image)[i + y1][j + x1].b;
				continue;
			}
			s.r = 0;
			s.g = 0;
			s.b = 0;
			for (int k = 0; k < 3; ++k) {
				for (int t = 0; t < 3; ++t) {
					int a = i - 1 + k + y1;
					int b = j - 1 + t + x1;
					if (a >= 0 && a < h && b >= 0 && b < w) {
						s.r += (*image)[a][b].r * blur[k][t];
						s.g += (*image)[a][b].g * blur[k][t];
						s.b += (*image)[a][b].b * blur[k][t];
					}
				}
			}
			// keeping each pixel value between 0 and 255
			buffer[i][j].r = clamp(s.r / 9, 0, 255);
			buffer[i][j].g = clamp(s.g / 9, 0, 255);
			buffer[i][j].b = clamp(s.b / 9, 0, 255);
		}
	}

	// updating the selection with the modified values
	for (i = 0; i < sh; ++i) {
		for (j = 0 ; j < sw; ++j) {
			(*image)[i  + y1][j + x1].r = buffer[i][j].r;
			(*image)[i  + y1][j + x1].g = buffer[i][j].g;
			(*image)[i  + y1][j + x1].b = buffer[i][j].b;
		}
	}
	free_mat(&buffer, y2 - y1);
}

void apply_gaussian_blur(rgb_t ***image, int x1, int y1, int x2, int y2, int h,
						 int w)
{
	int i, j;
	rgb_t s, **buffer;
	// calculating the selection coordinates
	int sh = y2 - y1;
	int sw = x2 - x1;
	buffer = malloc(sh * sizeof(rgb_t *));
	if (!buffer) {
		fprintf(stderr, "Error allocation");
	}
	for (i = 0; i < sh; ++i) {
		buffer[i] = malloc(sw * sizeof(rgb_t));
		if (!buffer[i]) {
			fprintf(stderr, "Error allocation");
		}
	}
	// kernel image
	int g_blur[3][3] = {
		{1, 2, 1},
		{2, 4, 2},
		{1, 2, 1}
	};
	// aplying the kernel
	for (i = 0; i < sh; ++i) {
		for (j = 0; j < sw; ++j) {
			if (i + y1 == 0 || i + y1 == h - 1 || j + x1 == 0 ||
				j + x1 == w - 1) {
				// if the effect is applied on the entire image we skip
				// the border
				buffer[i][j].r = (*image)[i + y1][j + x1].r;
				buffer[i][j].g = (*image)[i + y1][j + x1].g;
				buffer[i][j].b = (*image)[i + y1][j + x1].b;
				continue;
			}
			s.r = 0;
			s.g = 0;
			s.b = 0;
			for (int k = 0; k < 3; ++k) {
				for (int t = 0; t < 3; ++t) {
					int a = i - 1 + k + y1;
					int b = j - 1 + t + x1;
					if (a >= 0 && a < h && b >= 0 && b < w) {
						s.r += (*image)[a][b].r * g_blur[k][t];
						s.g += (*image)[a][b].g * g_blur[k][t];
						s.b += (*image)[a][b].b * g_blur[k][t];
					}
				}
			}
			// keeping each pixel value between 0 and 255
			buffer[i][j].r = clamp(s.r / 16, 0, 255);
			buffer[i][j].g = clamp(s.g / 16, 0, 255);
			buffer[i][j].b = clamp(s.b / 16, 0, 255);
		}
	}
	// updating the selection with the modified values
	for (i = 0; i < sh; ++i) {
		for (j = 0 ; j < sw; ++j) {
			(*image)[i  + y1][j + x1].r = buffer[i][j].r;
			(*image)[i  + y1][j + x1].g = buffer[i][j].g;
			(*image)[i  + y1][j + x1].b = buffer[i][j].b;
		}
	}
	free_mat(&buffer, y2 - y1);
}

void apply_edge(rgb_t ***image, int x1, int y1, int x2, int y2, int h, int w)
{
	int i, j;
	rgb_t s, **buffer;
	// calculating the selection coordinates
	int sh = y2 - y1;
	int sw = x2 - x1;
	buffer = malloc(sh * sizeof(rgb_t *));
	if (!buffer) {
		fprintf(stderr, "Error allocation");
	}
	for (i = 0; i < sh; ++i) {
		buffer[i] = malloc(sw * sizeof(rgb_t));
		if (!buffer[i]) {
			fprintf(stderr, "Error allocation");
		}
	}
	// kernel image
	int edge[3][3] = {
		{-1, -1, -1},
		{-1, 8, -1},
		{-1, -1, -1}
	};
	// aplying the kernel
	for (i = 0; i < sh; ++i) {
		for (j = 0; j < sw; ++j) {
			if (i + y1 == 0 || i + y1 == h - 1 || j + x1 == 0 ||
				j + x1 == w - 1) {
				// if the effect is applied on the entire image we skip
				// the border
				buffer[i][j].r = (*image)[i + y1][j + x1].r;
				buffer[i][j].g = (*image)[i + y1][j + x1].g;
				buffer[i][j].b = (*image)[i + y1][j + x1].b;
				continue;
			}
			s.r = 0;
			s.g = 0;
			s.b = 0;
			for (int k = 0; k < 3; ++k) {
				for (int t = 0; t < 3; ++t) {
					int a = i - 1 + k + y1;
					int b = j - 1 + t + x1;
					if (a >= 0 && a < h && b >= 0 && b < w) {
						s.r += (*image)[a][b].r * edge[k][t];
						s.g += (*image)[a][b].g * edge[k][t];
						s.b += (*image)[a][b].b * edge[k][t];
					}
				}
			}
			// keeping each pixel value between 0 and 255
			buffer[i][j].r = clamp(s.r, 0, 255);
			buffer[i][j].g = clamp(s.g, 0, 255);
			buffer[i][j].b = clamp(s.b, 0, 255);
		}
	}
	// updating the selection with the modified values
	for (i = 0; i < sh; ++i) {
		for (j = 0 ; j < sw; ++j) {
			(*image)[i  + y1][j + x1].r = buffer[i][j].r;
			(*image)[i  + y1][j + x1].g = buffer[i][j].g;
			(*image)[i  + y1][j + x1].b = buffer[i][j].b;
		}
	}
	free_mat(&buffer, y2 - y1);
}
