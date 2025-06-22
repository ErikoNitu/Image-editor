//312CA_NituErikoLaurentiu_Tema3
#ifndef HEADERS_H
#define HEADERS_H
#include <stdio.h>
#include <math.h>
typedef struct {
	int r, g, b;
} rgb_t;
int clamp(int x, int min, int max);
void free_mat(rgb_t ***mat, int h);
void load_image(char command[50], FILE **file, rgb_t ***image, int *w, int *h,
				int *max_pix, char format[3], int *ok);
void save_image(char command[50], FILE *file, rgb_t ***image, int w, int h,
				int max_pix, char format[3]);
void crop_image(FILE *file, rgb_t ***image, int *w, int *h, int *x1, int *y1,
				int *x2, int *y2);
void select_image(char command[50], FILE *file, int *x1, int *y1, int *x2,
				  int *y2, int h, int w);
void rotate_image(char command[50], FILE *file, rgb_t ***image, int *w, int *h,
				  int *x1, int *y1, int *x2, int *y2);
void rotate_right(rgb_t ***image, int w, int h, int x1, int y1);
void rotate_left(rgb_t ***image, int w, int h, int x1, int y1);
void init_selection(int *x1, int *y1, int *x2, int *y2, int h, int w);
void rotate_whole_image_right(rgb_t ***image, int *w, int *h, int *x1, int *y1,
							  int *x2, int *y2);
void rotate_whole_image_left(rgb_t ***image, int *w, int *h, int *x1, int *y1,
							 int *x2, int *y2);
void apply_image(char command[50], FILE *file, rgb_t ***image, int x1, int y1,
				 int x2, int y2, int h, int w, char format[3]);
void apply_sharpen(rgb_t ***image, int x1, int y1, int x2, int y2, int h,
				   int w);
void apply_blur(rgb_t ***image, int x1, int y1, int x2, int y2, int h, int w);
void apply_gaussian_blur(rgb_t ***image, int x1, int y1, int x2, int y2, int h,
						 int w);
void apply_edge(rgb_t ***image, int x1, int y1, int x2, int y2, int h, int w);
void histogram(char command[50], FILE *file, rgb_t **image, int w, int h,
			   char format[3]);
void equalize_image(FILE *file, rgb_t ***image, int w, int h, char format[3]);

#endif
