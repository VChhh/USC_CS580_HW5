/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"

// helper functions
float pi = 3.1415926535897932;

int checkboard(float u, float v, GzColor color, GzColor c1, GzColor c2) {
	int check_size = 40;
	int x = (int)(u * check_size) % 2;
	int y = (int)(v * check_size) % 2;

	if (x == y) {
		color[0] = c1[0];
		color[1] = c1[1];
		color[2] = c1[2];
	}
	else {
		color[0] = c2[0];
		color[1] = c2[1];
		color[2] = c2[2];
	}
	return GZ_SUCCESS;
}

float rings(float u, float v, float center_u, float center_v, int size) {
	// get the largest distance from the center
	float max_u = (center_u > 0.5) ? center_u : 1 - center_u;
	float max_v = (center_v > 0.5) ? center_v : 1 - center_v;
	float max_dist = sqrt(max_u * max_u + max_v * max_v);

	float distance = 0;
	if (max_dist != 0) {
		distance = sqrt((u - center_u) * (u - center_u) + (v - center_v) * (v - center_v)) / max_dist;
		distance = (sin((distance * size - 0.5) * pi * 2) + 1) / 2;
	}
	distance = (distance > 1) ? 1 : distance;

	return distance;
}

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
	GzColor c1 = { 1, 1, 1 };
	GzColor c2 = { .5, .5, .5 };
	GzColor checkboard_color = { 1, 1, 1 };
	checkboard(u, v, checkboard_color, c1, c2);


	float ring_coefficient_red = rings(u, v, 0.3, 0.7, 10);
	float ring_coefficient_green = rings(u, v, 0.7, 0.3, 15);
	float ring_coefficient_blue = rings(u, v, 0.5, 0.5, 20);

	// combine the two textures
	color[0] = checkboard_color[0] * ring_coefficient_red;
	color[1] = checkboard_color[1] * ring_coefficient_green;
	color[2] = checkboard_color[2] * ring_coefficient_blue;


	return GZ_SUCCESS;
}

/* Free texture memory */
int GzFreeTexture()
{
	if(image!=NULL)
		free(image);
	return GZ_SUCCESS;
}

