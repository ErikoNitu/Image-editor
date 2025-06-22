//312CA_NituErikoLaurentiu_Tema3
#include <stdio.h>
#include <string.h>
#include "headers.h"
#include <stdlib.h>
#include <math.h>

int main(void)
{
	int ok = 0;
	int stop = 0;
	int x1, y1, x2, y2; // the selection coordinates
	char command[50];
	FILE *file = NULL;
	int w, h, max_pix;
	rgb_t **image;
	char format[3];
	// reading the input until the EXIT command
	while (stop == 0) {
		//reading each command
		fgets(command, 50, stdin);
		command[strcspn(command, "\n")] = '\0';
		if (strstr(command, "LOAD") != 0) {
			// if the previous LOAD command was successful
			if (ok) {
				fclose(file);
				free_mat(&image, h);
			}
			load_image(command, &file, &image, &w, &h, &max_pix, format, &ok);
			// initialising the image coordinates
			init_selection(&x1, &y1, &x2, &y2, h, w);
		} else if (strstr(command, "SELECT") != 0) {
			select_image(command, file, &x1, &y1, &x2, &y2, h, w);
		} else if (strstr(command, "ROTATE") != 0) {
			rotate_image(command, file, &image, &w, &h, &x1, &y1, &x2, &y2);
		} else if (strstr(command, "EQUALIZE") != 0) {
			equalize_image(file, &image, w, h, format);
		} else if (strstr(command, "CROP") != 0) {
			crop_image(file, &image, &w, &h, &x1, &y1, &x2, &y2);
		} else if (strstr(command, "APPLY") != 0) {
			apply_image(command, file, &image, x1, y1, x2, y2, h, w, format);
		} else if (strstr(command, "SAVE") != 0) {
			save_image(command, file, &image, w, h, max_pix, format);
		} else if (strstr(command, "HISTOGRAM") != 0) {
			histogram(command, file, image, w, h, format);
		} else if (strstr(command, "EXIT") != 0) {
			if (!(file)) {
				printf("No image loaded\n");
			}
			stop = 1; // stopping the command reading
		} else {
			printf("Invalid command\n");
		}
	}
	// freeing the memory if the last LOAD command was successful
	if (ok == 1) {
		free_mat(&image, h);
		fclose(file);
	}

}
