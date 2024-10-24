/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"

GzColor	*image=NULL;
int xs, ys;
int reset = 1;

/* Image texture function */
int tex_fun(float u, float v, GzColor color)
{
  unsigned char		pixel[3];
  unsigned char     dummy;
  char  		foo[8];
  int   		i, j;
  FILE			*fd;

  if (reset) {          /* open and load texture file */
    fd = fopen ("texture", "rb");
    if (fd == NULL) {
      fprintf (stderr, "texture file not found\n");
      exit(-1);
    }
    fscanf (fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
    image = (GzColor*)malloc(sizeof(GzColor)*(xs+1)*(ys+1));
    if (image == NULL) {
      fprintf (stderr, "malloc for texture image failed\n");
      exit(-1);
    }

    for (i = 0; i < xs*ys; i++) {	/* create array of GzColor values */
      fread(pixel, sizeof(pixel), 1, fd);
      image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
      image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
      image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
      }

    reset = 0;          /* init is done */
	fclose(fd);
  }

/* bounds-test u,v to make sure nothing will overflow image array bounds */
/* determine texture cell corner values and perform bilinear interpolation */
/* set color to interpolated GzColor value and return */
    // clamp uv
    if (u < 0) u = 0;
    if (u > 1) u = 1;
    if (v < 0) v = 0;
    if (v > 1) v = 1;

	// calculate the pixel position
	u = u * (xs - 1);
	v = v * (ys - 1);

	// get the corner pixels
	int lower_x = (int)floor(u);
	int upper_x = (int)ceil(u);
	int lower_y = (int)floor(v);
	int upper_y = (int)ceil(v);

	float s = u - (float)lower_x;
	float t = v - (float)lower_y;

    // get corner pixel color
	// A as upper left, B as upper right, C as lower right, D as lower left
    GzColor A = { image[lower_x + lower_y * xs][RED], image[lower_x + lower_y * xs][GREEN], image[lower_x + lower_y * xs][BLUE] };
	GzColor B = { image[upper_x + lower_y * xs][RED], image[upper_x + lower_y * xs][GREEN], image[upper_x + lower_y * xs][BLUE] };
	GzColor C = { image[upper_x + upper_y * xs][RED], image[upper_x + upper_y * xs][GREEN], image[upper_x + upper_y * xs][BLUE] };
	GzColor D = { image[lower_x + upper_y * xs][RED], image[lower_x + upper_y * xs][GREEN], image[lower_x + upper_y * xs][BLUE] };

    // calculate the color
    for (size_t i = 0; i < 3; ++i) {
		color[i] = s * t * C[i] + (1 - s) * t * D[i] + s * (1 - t) * B[i] + (1 - s) * (1 - t) * A[i];
    }

    return GZ_SUCCESS;
}

/* Procedural texture function */
int ptex_fun(float u, float v, GzColor color)
{

	return GZ_SUCCESS;
}

/* Free texture memory */
int GzFreeTexture()
{
	if(image!=NULL)
		free(image);
	return GZ_SUCCESS;
}

