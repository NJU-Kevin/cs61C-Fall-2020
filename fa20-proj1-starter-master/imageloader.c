/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	FILE *fp = fopen(filename, "r");
	char head[3];
    if (fscanf(fp, "%2s", head) != 1 || strcmp(head, "P3") != 0) {
        fprintf(stderr, "Error: Not a valid PPM P3 file\n");
        fclose(fp);
        return NULL;
    }
	// Read rows and cols
	uint32_t rows, cols;
	if (fscanf(fp, "%u %u", &cols, &rows) != 2) {
		fprintf(stderr, "Error: Invalid dimensions");
		fclose(fp);
		return NULL;
	}
	// Read max value
	uint32_t maxValue;
	if (fscanf(fp, "%u", &maxValue) != 1) {
		fprintf(stderr, "Error: Invalid max value");
		fclose(fp);
		return NULL;
	}
	Image *result = (Image *)malloc(sizeof(Image));
	result->cols = cols;
	result->rows = rows;
	// image is an array of rows of colors
	// each row is an array of cols of colors at certain row
	result->image = (Color **)malloc(rows * sizeof(Color*));
	for (uint32_t i = 0; i < rows; i++) {
		result->image[i] = (Color *)malloc(cols * sizeof(Color));
	}
	// Read RGB values
	for (uint32_t i = 0; i < rows; i++) {
		for (uint32_t j = 0; j < cols; j++) {
			uint8_t r, g, b;
			if (fscanf(fp, "%hhu %hhu %hhu", &r, &g, &b) != 3) {
				fprintf(stderr, "Error: Invalid pixel data");
				for (uint32_t k = 0; k < rows; k++) {
					free(result->image[k]);
				}
				free(result->image);
				free(result);
				fclose(fp);
				return NULL;
			}
			if (maxValue != 255) {
				result->image[i][j].R = (uint8_t)(r * 255 / maxValue);
				result->image[i][j].G = (uint8_t)(g * 255 / maxValue);
				result->image[i][j].B = (uint8_t)(b * 255 / maxValue);
			} else {
				result->image[i][j].R = r;
				result->image[i][j].G = g;
				result->image[i][j].B = b;
			}
		}
	}
	fclose(fp);
	return result;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	printf("P3\n");
	printf("%u %u\n", image->cols, image->rows);
	printf("255\n");
	for (uint32_t i = 0; i < image->rows; i++) {
		for (uint32_t j = 0; j < image->cols; j++) {
			printf("%3u %3u %3u", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
			if (j != image->cols - 1) {
				printf("   ");
			}
		}
		printf("\n");
	}
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	for (uint32_t i = 0; i < image->rows; i++) {
		free(image->image[i]);
	}
	free(image->image);
	free(image);
}