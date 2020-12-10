/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "JeonYeonji",              /* name */

    "2019015414",     /* student ID */
    "jyj9647@hanyang.ac.kr",  /* student email */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}


char rotate_descr3[] = "rotate3: Current working version";
void rotate3(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    int bi, bj;
    int ldim = dim - 1;
    for(i = ldim; i >= 1; i--) {
        if(dim % i == 0) break;
    }
    int b = i;
    int n = dim / b;
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            for(bi = 0; bi < b; bi++) {
                for(bj = 0; bj < b; bj++) {
                    dst[RIDX(dim - 1 - RIDX(j, bj, b), RIDX(i, bi, b), dim)] = src[RIDX(RIDX(i, bi, b), RIDX(j, bj, b), dim)];
                }
            }
        }
    }
}

char rotate_descr4[] = "rotate4: Current working version";
void rotate4(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    int bi, bj;
    int ldim = dim - 1;
    for(i = ldim; i >= 1; i--) {
        if(dim % i == 0) break;
    }
    int b = i;
    int n = dim / b;
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            for(bi = 0; bi < b; bi++) {
                for(bj = 0; bj < b; bj++) {
                    dst[RIDX(dim - 1 - RIDX(i, bi, b), RIDX(j, bj, b), dim)] = src[RIDX(RIDX(j, bj, b), RIDX(i, bi, b), dim)];
                }
            }
        }
    }
}

char rotate_descr2[] = "rotate2: Current working version";
void rotate2(int dim, pixel *src, pixel *dst) 
{
    int i, j, k;
    int ldim = dim - 1;
    for(i = ldim; i >= 1; i--) {
        if(dim % i == 0) break;
    }
    int b = i;
    int n = dim / b;
    int idx;
    for(i = 0; i < n; i++) {
        for(j = 0; j < dim; j++) {
            for(k = 0; k < b; k++) {
                idx = RIDX(i, k, b);
                dst[RIDX(dim - 1 - j, idx, dim)] = src[RIDX(idx, j, dim)];
            }
        }
    }
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */

char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j, k;
    int ldim = dim - 1;
    for(i = ldim; i >= 1; i--) {
        if(dim % i == 0) break;
    }
    int b = i;
    int n = dim / b;
    int idx;
    for(i = 0; i < n; i++) {
        for(j = 0; j < dim; j++) {
            idx = i * b;
            for(k = 0; k < b; k++) {
                dst[RIDX(dim - 1 - j, idx, dim)] = src[RIDX(idx, j, dim)];
                idx++;
            }
        }
    }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
    
    add_rotate_function(&rotate2, rotate_descr2);   
    add_rotate_function(&rotate3, rotate_descr3);   
    add_rotate_function(&rotate4, rotate_descr4);   
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j, k;
    int ldim = dim - 1;
    for(i = ldim; i >= 1; i--) {
        if(dim % i == 0) break;
    }
    int b = i;
    int n = dim / b;
    for(i = 0; i < n; i++) {
        for(j = 0; j < dim; j++) {
            for(k = 0; k < b; k++) {
                dst[RIDX(j, RIDX(k, i, n), dim)] = avg(dim, j, RIDX(k, i, n), src);
            }
        }
    }
}

char smooth_descr3[] = "smooth3: Current working version";
void smooth3(int dim, pixel *src, pixel *dst) 
{
    int i, j, k;
    int ldim = dim - 1;
    for(i = ldim; i >= 1; i--) {
        if(dim % i == 0) break;
    }
    int b = i;
    int n = dim / b;
    for(i = 0; i < n; i++) {
        for(j = 0; j < dim; j++) {
            for(k = 0; k < b; k++) {
                dst[RIDX(j, RIDX(i, k, b), dim)] = avg(dim, j, RIDX(i, k, b), src);
            }
        }
    }
}

char smooth_descr2[] = "smooth2: Current working version";
void smooth2(int dim, pixel *src, pixel *dst) 
{
    int i, j, k;
    int ldim = dim - 1;
    for(i = ldim; i >= 1; i--) {
        if(dim % i == 0) break;
    }
    int b = i;
    int n = dim / b;
    int idx;
    for(i = 0; i < n; i++) {
        idx = i * b;
        for(j = 0; j < dim; j++) {
            for(k = 0; k < b; k++) {
                dst[RIDX(j, idx + k, dim)] = avg(dim, j, idx + k, src);
            }
        }
    }
}

char smooth_descr4[] = "smooth4: Current working version";
void smooth4(int dim, pixel *src, pixel *dst) 
{
    int i, j, k;
    int ldim = dim - 1;
    for(i = ldim; i >= 1; i--) {
        if(dim % i == 0) break;
    }
    int b = i;
    int n = dim / b;
    int idx;
    for(i = 0; i < n; i++) {
        for(j = 0; j < dim; j++) {
            idx = i * b;
            for(k = 0; k < b; k++) {
                dst[RIDX(j, idx, dim)] = avg(dim, j, idx, src);
                idx++;
            }
        }
    }
}

char smooth_descr5[] = "smooth5: Current working version";
void smooth5(int dim, pixel *src, pixel *dst) 
{
    int i, j, k;
    int ldim = dim - 1;
    int idx;
    for(i = ldim; i >= 1; i--) {
        if(dim % i == 0) break;
    }
    int b = i;
    int n = dim / b;
    for(i = 0; i < n; i++) {
        for(j = 0; j < dim; j++) {
            idx = i;
            for(k = 0; k < b; k++) {
                dst[RIDX(j, idx, dim)] = avg(dim, j, idx, src);
                idx += n;
            }
        }
    }
}

/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    add_smooth_function(&smooth, smooth_descr);
    /* ... Register additional test functions here */
    add_smooth_function(&smooth2, smooth_descr2);
    add_smooth_function(&smooth3, smooth_descr3);
    add_smooth_function(&smooth4, smooth_descr4);
    add_smooth_function(&smooth5, smooth_descr5);
}

