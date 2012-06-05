#ifndef ROTATION_H
#define ROTATION_H

#include <omp.h>
#include <stdio.h>
#include <math.h>
#include "IO/bmpfile.h"

// min/max macro
#define min(a, b)  (((a) < (b)) ? (a) : (b)) 
#define max(a, b)  (((a) > (b)) ? (a) : (b))

int rotation(char* in_image_path, double deg){
    bmpfile_t *input_img = NULL, *output_img = NULL;
    int in_width = 0, in_height = 0, out_width = 0, out_height = 0;
    double deg_radian = 0.0;
    // define pi
    const double PI = atan(1.0)*4;
    deg_radian = deg*PI/180.0;
    
    // calculate the sin/cos of given degree
    double sine = 0.0, cosine = 0.0;
    sine = sin(deg_radian);
    cosine = cos(deg_radian);
    
    // read input image
    input_img = bmp_create_8bpp_from_file(in_image_path);

    if (input_img){
        in_width = bmp_get_width(input_img);
        in_height = bmp_get_height(input_img);
    }
    else {
        printf("Cannot read from file %s\n", in_image_path);
        exit(1);
    }
 
    printf("Read image width is %d, height is %d\n", in_width, in_height);
    
    // Assume the origin to be the left-upper corner
    // First compute the other three corners
    double x1 = -in_height*sine, y1 = in_height*cosine;
    double x2 = in_width*cosine-in_height*sine, y2 = in_height*cosine+in_width*sine;
    double x3 = in_width*cosine, y3 = in_width*sine;
    
    // calcuate the new height and width
    double minx = min(0, min(x1, min(x2, x3)));
    double miny = min(0, min(y1, min(y2, y3)));
    double maxx = max(x1, max(x2, x3));
    double maxy = max(y1, max(y2, y3));
    
    out_width = (int)abs(maxx - minx);
    out_height = (int)abs(maxy - miny);
    
    printf("New width is %d, new height is %d\n", out_width, out_height);
    
    // apply rotation operation
    output_img = bmp_create(out_width, out_height, 8);
    if (!output_img) {
        printf("Fail to create rotation output image!\n");
        exit(1);
    }
    
    int i,j;
    for (i = 0; i < out_width; ++i){
        for (j = 0; j < out_height; ++j){
            int src_x = (int)((i+minx)*cosine+(j+miny)*sine);
            int src_y = (int)((j+miny)*cosine-(i+minx)*sine);
            if (src_x >= 0 && src_x < in_width && src_y >= 0 && src_y < in_height){
                rgb_pixel_t *pixel = bmp_get_pixel(input_img, src_x, src_y);
                bmp_set_pixel(output_img, i, j, *pixel);
            }
        }
    }
    
    bmp_save(output_img, "rotation.bmp");
    
    bmp_destroy(input_img);
    bmp_destroy(output_img);
    
    return 0;
}

int rotation_parallel(char* in_image_path, double deg, int num_of_threads, int chunk_size){
}

#endif






