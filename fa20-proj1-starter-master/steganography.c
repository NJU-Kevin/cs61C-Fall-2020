/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	Color *result = (Color *)malloc(sizeof(Color));
	Color color = image->image[row][col];
	if (color.B & 1) {
		result->B = 255;
		result->G = 255;
		result->R = 255;
	} else {
		result->B = 0;
		result->G = 0;
		result->R = 0;
	}
	return result;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
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
			Color *newColor = evaluateOnePixel(image, i, j);
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
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 2) {
		fprintf(stderr, "Invalid number of arguments");
		return -1;
	}
	Image *image = readData(argv[1]);
	if (image == NULL) {
		return -1;
	}
	Image *proceeded = steganography(image);
	if (proceeded == NULL) {
		freeImage(image);
		return -1;
	}

	writeData(proceeded);

	freeImage(proceeded);
	freeImage(image);
	return 0;
}
