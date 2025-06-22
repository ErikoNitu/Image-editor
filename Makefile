CC = gcc
CFLAGS = -Wall -Wextra -lm
SRC = main.c load.c save.c select.c crop.c rotate.c apply.c histogram.c equalize.c
EXE = image_editor

.PHONY: build
build:
	$(CC) $(CFLAGS) -o $(EXE) $(SRC) 

.PHONY: clean
clean:
	rm -f $(EXE) *~