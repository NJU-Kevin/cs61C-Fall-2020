#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

Image *encode(Image *carrier, Image *secret) {
    if (carrier->cols != secret->cols || carrier->rows != secret->rows) {
        fprintf(stderr, "Invalid dimensions");
        return NULL;
    }
    Image *result = (Image *)malloc(sizeof(Image));
    if (result == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        return NULL;
    }
    result->cols = carrier->cols;
    result->rows = carrier->rows;
    result->image = (Color **)malloc(result->rows * sizeof(Color *));
    if (result->image == NULL) {
        fprintf(stderr, "Failed to allocate memory for image");
        free(result);
        return NULL;
    }
    for (uint32_t i = 0; i < result->rows; i++) {
        result->image[i] = (Color *)malloc(result->cols * sizeof(Color));
        if (result->image[i] == NULL) {
            fprintf(stderr, "Failed to allocate memory for image[%d]", i);
            for (uint32_t j = 0; j < i; j++) {
                free(result->image[j]);
            }
            free(result->image);
            free(result);
            return NULL;
        }

        for (uint32_t j = 0; j < result->cols; j++) {
            result->image[i][j] = carrier->image[i][j];
            Color secretPixel = secret->image[i][j];
            int isWhite = (secretPixel.R >= 230 && secretPixel.G >= 230 && secretPixel.B >= 230);
            if (isWhite) {
                result->image[i][j].B |= 1;
            } else {
                result->image[i][j].B &= 0xFE;
            }
        }
    }
    return result;
}

void writePPM(Image *image, char *filename) {
    FILE *fp = fopen(filename, "w");
    fprintf(fp, "P3\n");
    fprintf(fp, "%u %u\n", image->cols, image->rows);
    fprintf(fp, "255\n");
    for (uint32_t i = 0; i < image->rows; i++) {
        for (uint32_t j = 0; j < image->cols; j++) {
            fprintf(fp, "%3u %3u %3u", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
            if (j != image->cols - 1) {
                fprintf(fp, "   ");
            }
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

int main(int argc, char **argv) {
    if (argc != 4) {
        return -1;
    }
    char *carrier_filename = argv[1];
    char *secret_filename = argv[2];
    char *output_filename = argv[3];
    Image *carrier = readData(carrier_filename);
    if (carrier == NULL) {
        return -1;
    }
    Image *secret = readData(secret_filename);
    if (secret == NULL) {
        freeImage(carrier);
        return -1;
    }
    Image *result = encode(carrier, secret);
    if (result == NULL) {
        freeImage(carrier);
        freeImage(secret);
        return -1;
    }
    writePPM(result, output_filename);
    freeImage(result);
    freeImage(carrier);
    freeImage(secret);
    return 0;
}