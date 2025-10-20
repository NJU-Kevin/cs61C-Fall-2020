/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
	Color *newColor = (Color *)malloc(sizeof(Color));
	if (newColor == NULL) {
		printf("Failed to allocate memory for newColor");
		return NULL;
	}

	uint8_t oldR = image->image[row][col].R;
	uint8_t oldG = image->image[row][col].G;
	uint8_t oldB = image->image[row][col].B;
	uint8_t newR = 0;
	uint8_t newG = 0;
	uint8_t newB = 0;
	// For every bit of R, G, B, apply the rules
	for (int bit = 0; bit < 8; bit++) {
		int curBitR = (oldR >> bit) & 1;
		int curBitG = (oldG >> bit) & 1;
		int curBitB = (oldB >> bit) & 1;

		// Count the living neighbors of current grid respectively in R, G, B channels
		int countR = 0;
		int countG = 0;
		int countB = 0;

		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				if (dx == 0 && dy == 0) {
					continue;
				}
				int neighborRow = (row + dy + image->rows) % image->rows;
				int neighborCol = (col + dx + image->cols) % image->cols;
				Color neighborColor = image->image[neighborRow][neighborCol];
				countR += (neighborColor.R >> bit) & 1;
				countG += (neighborColor.G >> bit) & 1;
				countB += (neighborColor.B >> bit) & 1;
			}
		}
		int newBitR = 0;
		int newBitG = 0;
		int newBitB = 0;

		if (curBitR) {
			newBitR = ((rule >> 9) >> countR) & 1;
		} else {
			newBitR = (rule >> countR) & 1;
		}
		newR &= ~(1 << bit);
		newR |= (newBitR << bit);

		if (curBitG) {
			newBitG = ((rule >> 9) >> countG) & 1;
		} else {
			newBitG = (rule >> countG) & 1;
		}
		newG &= ~(1 << bit);
		newG |= (newBitG << bit);

		if (curBitB) {
			newBitB = ((rule >> 9) >> countB) & 1;
		} else {
			newBitB = (rule >> countB) & 1;
		}
		newB &= ~(1 << bit);
		newB |= (newBitB << bit);
	}
	newColor->R = newR;
	newColor->G = newG;
	newColor->B = newB;
	return newColor;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	Image *result = (Image *)malloc(sizeof(Image));
	if (result == NULL) {
		return NULL;
	}
	result->cols = image->cols;
	result->rows = image->rows;
	result->image = (Color **)malloc(result->rows * sizeof(Color *));
	if (result->image == NULL) {
		free(result);
		return NULL;
	}
	for (uint32_t i = 0; i < image->rows; i++) {
		result->image[i] = (Color *)malloc(result->cols * sizeof(Color));
		if (result->image[i] == NULL) {
			for (uint32_t j = 0; j < i; j++) {
				free(result->image[j]);
			}
			free(result->image);
			free(result);
			return NULL;
		}
		for (uint32_t j = 0; j < image->cols; j++) {
			Color *newColor = evaluateOneCell(image, i, j, rule);
			if (newColor == NULL) {
				for (uint32_t k = 0; k <= i; k++) {
					free(result->image[k]);
				}
				free(result->image);
				free(result);
				return NULL;
			}
			result->image[i][j] = *newColor;
			free(newColor);
		}
	}
	return result;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 3) {
		printf("usage: ./gameOfLife filename rule\n");
        printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
        printf("rule is a hex number beginning with 0x; Life is 0x1808.\n");
		return -1;
	}

	Image *image = readData(argv[1]);
	if (image == NULL) {
		printf("Failed to load image");
		return -1;
	}

	char *endptr;
    uint32_t rule = (uint32_t)strtoul(argv[2], &endptr, 16);
    if (*endptr != '\0') {
        printf("Invalid rule format: %s\n", argv[2]);
        return -1;
    }

	Image *result = life(image, rule);
	if (result == NULL) {
		printf("Failed to load new image");
		freeImage(image);
		return -1;
	}
	writeData(result);
	freeImage(result);
	freeImage(image);
	return 0;
}
