//312CA_NituErikoLaurentiu_Tema3
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headers.h"
// function which initialises the image coordinates after loading one
void init_selection(int *x1, int *y1, int *x2, int *y2, int h, int w)
{
	*x1 = 0;
	*x2 = w;
	*y1 = 0;
	*y2 = h;
}

void select_image(char command[50], FILE *file, int *x1, int *y1, int *x2,
				  int *y2, int h, int w)
{
	if (!(file)) {
		printf("No image loaded\n");
		return;
	}
	// checking if the command was to select all image or a section
	if (strstr(command, "ALL") != 0) {
		*x1 = 0;
		*x2 = w;
		*y1 = 0;
		*y2 = h;
		printf("Selected ALL\n");
	} else {
		int new_x1, new_y1, new_x2, new_y2;
		// checking if the coordinates are valid
		if (sscanf(command, "SELECT %d %d %d %d", &new_x1, &new_y1, &new_x2,
				   &new_y2) == 4) {
			if (new_x1 < 0 || new_x2 > w || new_y1 < 0 || new_y2 > h ||
				new_x1 > w || new_y1 > h || new_x2 < 0 || new_y2 < 0 ||
				new_x1 == new_x2 || new_y1 == new_y2) {
				printf("Invalid set of coordinates\n");
				return;
			}
			// assigning the correct order for the coordinates
			if (new_x1 > new_x2) {
				int aux = new_x1;
				new_x1 = new_x2;
				new_x2 = aux;
			}
			if (new_y1 > new_y2) {
				int aux = new_y1;
				new_y1 = new_y2;
				new_y2 = aux;
			}
			*x1 = new_x1;
			*x2 = new_x2;
			*y1 = new_y1;
			*y2 = new_y2;
			printf("Selected %d %d %d %d\n", *x1, *y1, *x2, *y2);
		} else {
			printf("Invalid command\n");
		}

	}

}

// function which frees memory
void free_mat(rgb_t ***mat, int h)
{
	for (int i = 0; i < h; ++i) {
		free((*mat)[i]);
	}
	free(*mat);
}
