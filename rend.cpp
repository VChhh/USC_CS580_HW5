/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"

#include "Vector3.h" // self-defined vector3 class
#include "Vector4.h" // self-defined vector4 class
#include "GzMatrix_extension.h" // self-defined matrix extension

#define PI (float) 3.14159265358979323846

int push_matrix_to_stack(short& matlevel, GzMatrix matrix, GzMatrix* stack) {
	if (matlevel >= MATLEVELS) return GZ_FAILURE;
	if (matlevel == -1) {
		// put the matrix at the base of the stack
		for (size_t i = 0; i < 4; ++i) {
			for (size_t j = 0; j < 4; ++j) {
				stack[0][i][j] = matrix[i][j];
			}
		}
	}
	else {
		// multiply the matrix with the top of the stack
		GzMatrix result;
		matrix_multiply(result, stack[matlevel], matrix);
		for (size_t i = 0; i < 4; ++i) {
			for (size_t j = 0; j < 4; ++j) {
				stack[matlevel + 1][i][j] = result[i][j];
			}
		}
	}
	// increment the matlevel
	++matlevel;
	return GZ_SUCCESS;
}

// transform the vector by the matrix
template<typename T>
Vector4<T> transform_vector4D(Vector4<T> vector, GzMatrix mat) {
	Vector4<T> result;
	for (size_t i = 0; i < 4; ++i) {
		T sum = 0;
		for (size_t j = 0; j < 4; ++j) {
			sum += mat[i][j] * vector[j];
		}
		result[i] = sum;
	}
	return result;
}

// interpolate function
// return a vector4f that represents the plane
Vector4f interpolate(Vector3f v1, Vector3f v2, Vector3f v3) {
	Vector4f plane;
	// get the normal
	Vector3f normal = (v2 - v1).cross(v3 - v1);
	float D = -normal.dot(v1);
	return Vector4f(normal.x, normal.y, normal.z, D);
}

float clip(Vector4f plane, int i, int j) {
	return -(plane.A * (float)i + plane.B * (float)j + plane.D) / plane.C;
}

Vector3f calculate_intensity_dl(Vector3f normal, GzLight directional_light, GzColor Ks, GzColor Kd, float spec) {
	Vector3f E(0, 0, -1.0f); // the eye vector
	Vector3f intensity(0, 0, 0);
	float n_dot_l = normal.dot(directional_light.direction);
	float n_dot_e = normal.dot(E);
	// check if n_dot_l and n_dot_e have same sign
	if ((n_dot_l > 0 && n_dot_e > 0) || (n_dot_l < 0 && n_dot_e < 0)) {
		Vector3f R = (2 * n_dot_l * normal - directional_light.direction).normalize();
		float r_dot_e = clamp(R.dot(E), 0.0f, 1.0f);
		// add specular
		intensity += (Vector3f(Ks) * pow(r_dot_e, spec)) * (directional_light.color);
		// add diffuse
		if (n_dot_l > 0 && n_dot_e > 0) intensity += (Vector3f(Kd) * n_dot_l) * (directional_light.color);
		else if (n_dot_l < 0 && n_dot_e < 0) intensity += (Vector3f(Kd) * -n_dot_l) * (directional_light.color);
	}
	return intensity;
}

Vector3f calculate_intensity(Vector3f normal, GzLight* directional_lights, int numlights, GzLight ambient_light, GzColor Ks, GzColor Kd, GzColor Ka, float spec) {
	Vector3f intensity(0, 0, 0);
	for (size_t i = 0; i < numlights; ++i) {
		intensity += calculate_intensity_dl(normal, directional_lights[i], Ks, Kd, spec);
	}
	// add ambient light
	intensity += Vector3f(Ka) * (ambient_light.color);
	return clamp(intensity, 0.0f, 1.0f);
}

int GzRender::GzRotXMat(float degree, GzMatrix mat)
{
/* HW 3.1
// Create rotate matrix : rotate along x axis
// Pass back the matrix using mat value
*/
	float radian = degree * PI / 180.0f;
	// initialize the matrix
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			mat[i][j] = 0;
		}
	}
	mat[0][0] = 1.0f;
	mat[1][1] = cos(radian);
	mat[1][2] = -sin(radian);
	mat[2][1] = sin(radian);
	mat[2][2] = cos(radian);
	mat[3][3] = 1.0f;
	return GZ_SUCCESS;
}

int GzRender::GzRotYMat(float degree, GzMatrix mat)
{
/* HW 3.2
// Create rotate matrix : rotate along y axis
// Pass back the matrix using mat value
*/
	float radian = degree * PI / 180.0f;
	// initialize the matrix
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			mat[i][j] = 0;
		}
	}
	mat[0][0] = cos(radian);
	mat[0][2] = sin(radian);
	mat[1][1] = 1.0f;
	mat[2][0] = -sin(radian);
	mat[2][2] = cos(radian);
	mat[3][3] = 1.0f;
	return GZ_SUCCESS;
}

int GzRender::GzRotZMat(float degree, GzMatrix mat)
{
/* HW 3.3
// Create rotate matrix : rotate along z axis
// Pass back the matrix using mat value
*/
	float radian = degree * PI / 180.0f;
	// initialize the matrix
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			mat[i][j] = 0;
		}
	}
	mat[0][0] = cos(radian);
	mat[0][1] = -sin(radian);
	mat[1][0] = sin(radian);
	mat[1][1] = cos(radian);
	mat[2][2] = 1.0f;
	mat[3][3] = 1.0f;
	return GZ_SUCCESS;
}

int GzRender::GzTrxMat(GzCoord translate, GzMatrix mat)
{
/* HW 3.4
// Create translation matrix
// Pass back the matrix using mat value
*/
	// initialize the matrix
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			mat[i][j] = 0;
		}
	}
	mat[0][0] = 1.0f;
	mat[1][1] = 1.0f;
	mat[2][2] = 1.0f;
	mat[3][3] = 1.0f;
	mat[0][3] = translate[X];
	mat[1][3] = translate[Y];
	mat[2][3] = translate[Z];
	return GZ_SUCCESS;
}


int GzRender::GzScaleMat(GzCoord scale, GzMatrix mat)
{
/* HW 3.5
// Create scaling matrix
// Pass back the matrix using mat value
*/
	// initialize the matrix
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			mat[i][j] = 0;
		}
	}
	mat[0][0] = scale[X];
	mat[1][1] = scale[Y];
	mat[2][2] = scale[Z];
	mat[3][3] = 1.0f;
	return GZ_SUCCESS;
}


GzRender::GzRender(int xRes, int yRes)
{
/* HW1.1 create a framebuffer for MS Windows display:
 -- set display resolution
 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
 -- allocate memory for pixel buffer
 */
	xres = xRes <= 0 ? 0 : (xRes > MAXXRES ? MAXXRES : xRes);
	yres = yRes <= 0 ? 0 : (yRes > MAXYRES ? MAXYRES : yRes);
	framebuffer = (char*)malloc(3 * sizeof(char) * xRes * yRes); // provided
	// framebuffer = new char[3 * xres * yres];
	pixelbuffer = new GzPixel[xres * yres];

/* HW 3.6
- setup Xsp and anything only done once 
- init default camera 
*/ 
	matlevel = -1;
	// initialize normal_matlevel and other values
	normal_matlevel = -1;
	numlights = 0;

	// assign 0 to all Xsp elements
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			Xsp[i][j] = 0;
		}
	}

	// set up Xsp
	Xsp[0][0] = xres / 2.0f;
	Xsp[1][1] = -yres / 2.0f;
	Xsp[2][2] = MAXINT;
	Xsp[0][3] = xres / 2.0f;
	Xsp[1][3] = yres / 2.0f;
	Xsp[3][3] = 1.0f;

	// set up default camera
	m_camera.position[X] = DEFAULT_IM_X;
	m_camera.position[Y] = DEFAULT_IM_Y;
	m_camera.position[Z] = DEFAULT_IM_Z;
	m_camera.lookat[X] = 0.0f;
	m_camera.lookat[Y] = 0.0f;
	m_camera.lookat[Z] = 0.0f;
	m_camera.worldup[X] = 0.0f;
	m_camera.worldup[Y] = 1.0f;
	m_camera.worldup[Z] = 0.0f;
	m_camera.FOV = DEFAULT_FOV;
}

GzRender::~GzRender()
{
/* HW1.2 clean up, free buffer memory */
	delete[] framebuffer;
	delete[] pixelbuffer;
}

int GzRender::GzDefault()
{
/* HW1.3 set pixel buffer to some default values - start a new frame */
	for (size_t i = 0; i < xres * yres; ++i) {
		framebuffer[3 * i] = 0;
		framebuffer[3 * i + 1] = 0;
		framebuffer[3 * i + 2] = 0;
		pixelbuffer[i] = { 2055, 1798, 1541, 0, INT_MAX };
	}
	return GZ_SUCCESS;
}

int GzRender::GzBeginRender()
{
/* HW 3.7 
- setup for start of each frame - init frame buffer color,alpha,z
- compute Xiw and projection xform Xpi from camera definition 
- init Ximage - put Xsp at base of stack, push on Xpi and Xiw 
- now stack contains Xsw and app can push model Xforms when needed 
*/ 
	// initialize Xpi and Xiw
	GzMatrix Xpi;
	GzMatrix Xiw;
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			Xpi[i][j] = 0;
			Xiw[i][j] = 0;
		}
	}

	// construct Xpi
	// let dd = 1/d = tan(fov/2)
	float dd = tan(m_camera.FOV * PI / 180.0f / 2.0f);
	Xpi[0][0] = 1.0f;
	Xpi[1][1] = 1.0f;
	Xpi[2][2] = dd;
	Xpi[3][2] = dd;
	Xpi[3][3] = 1.0f;

	// construct Xiw
	// get the camera information in vector3f form
	Vector3f vector_lookat = Vector3f(m_camera.lookat[X], m_camera.lookat[Y], m_camera.lookat[Z]);
	Vector3f vector_position = Vector3f(m_camera.position[X], m_camera.position[Y], m_camera.position[Z]);
	Vector3f vector_worldup = Vector3f(m_camera.worldup[X], m_camera.worldup[Y], m_camera.worldup[Z]);

	// Z axis
	Vector3f Z_axis = (vector_lookat - vector_position).normalize();

	// up
	Vector3f up_modified = vector_worldup - Z_axis * vector_worldup.dot(Z_axis);

	// Y axis
	Vector3f Y_axis = up_modified.normalize();

	// X axis
	Vector3f X_axis = Y_axis.cross(Z_axis);

	Xiw[0][0] = X_axis[0];
	Xiw[0][1] = X_axis[1];
	Xiw[0][2] = X_axis[2];
	Xiw[0][3] = -X_axis.dot(vector_position);
	Xiw[1][0] = Y_axis[0];
	Xiw[1][1] = Y_axis[1];
	Xiw[1][2] = Y_axis[2];
	Xiw[1][3] = -Y_axis.dot(vector_position);
	Xiw[2][0] = Z_axis[0];
	Xiw[2][1] = Z_axis[1];
	Xiw[2][2] = Z_axis[2];
	Xiw[2][3] = -Z_axis.dot(vector_position);
	Xiw[3][3] = 1.0f;

	//put Xpi and Xiw to the camera
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			m_camera.Xiw[i][j] = Xiw[i][j];
			m_camera.Xpi[i][j] = Xpi[i][j];
		}
	}

	// push into the stack
	GzPushMatrix(Xsp);
	GzPushMatrix(Xpi);
	GzPushMatrix(Xiw);

	return GZ_SUCCESS;
}

int GzRender::GzPutCamera(GzCamera camera)
{
/* HW 3.8 
/*- overwrite renderer camera structure with new camera definition
*/
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			m_camera.Xiw[i][j] = camera.Xiw[i][j];
			m_camera.Xpi[i][j] = camera.Xpi[i][j];
		}
	}
	for (size_t i = 0; i < 3; ++i) {
		m_camera.position[i] = camera.position[i];
		m_camera.lookat[i] = camera.lookat[i];
		m_camera.worldup[i] = camera.worldup[i];
	}
	m_camera.FOV = camera.FOV;
	return GZ_SUCCESS;	
}

int GzRender::GzPushMatrix(GzMatrix	matrix)
{
/* HW 3.9 
- push a matrix onto the Ximage stack
- check for stack overflow
*/
	// check for stack overflow
	// checked in push_matrix_to_stack function

	// push the matrix
	int push_matrices_result = push_matrix_to_stack(matlevel, matrix, Ximage);

	// create an identity matrix
	GzMatrix I_matrix;
	identity_matrix(I_matrix);

	// push the normal matrix
	// for Xnorm, when pushing Xsp and Xpi, push identity matrix instead
	if (normal_matlevel < 1) push_matrices_result |= push_matrix_to_stack(normal_matlevel, I_matrix, Xnorm);
	else {
		// only rotation
		GzMatrix normal_matrix;
		copy_matrix(normal_matrix, matrix);
		// set the translation part to 0
		for (size_t i = 0; i < 3; ++i) {
			normal_matrix[i][3] = 0;
		}
		// zero out the scale part
		float scalar = sqrt(normal_matrix[0][0] * normal_matrix[0][0] + normal_matrix[0][1] * normal_matrix[0][1] + normal_matrix[0][2] * normal_matrix[0][2]);
		matrix_scale(normal_matrix, scalar);
		push_matrices_result |= push_matrix_to_stack(normal_matlevel, normal_matrix, Xnorm);
	}
	return push_matrices_result;
	// return GZ_SUCCESS;
}

int GzRender::GzPopMatrix()
{
/* HW 3.10
- pop a matrix off the Ximage stack
- check for stack underflow
*/
	// check for stack underflow
	if (matlevel < 0) return GZ_FAILURE;
	--matlevel;
	return GZ_SUCCESS;
}

int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
/* HW1.4 write pixel values into the buffer */
	if (pixelbuffer == NULL) return GZ_FAILURE;
	if (i < 0 || i >= xres || j < 0 || j >= yres) return GZ_FAILURE;

	if (z < pixelbuffer[ARRAY(i, j)].z) {
		// clamp the value to its range
		GzIntensity red = clamp(r, GzIntensity(0), GzIntensity(4095));
		GzIntensity green = clamp(g, GzIntensity(0), GzIntensity(4095));
		GzIntensity blue = clamp(b, GzIntensity(0), GzIntensity(4095));

		pixelbuffer[ARRAY(i, j)] = GzPixel{ red, green, blue, a, z };
	}
	return GZ_SUCCESS;
}


int GzRender::GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
/* HW1.5 retrieve a pixel information from the pixel buffer */
	if (pixelbuffer == NULL) return GZ_FAILURE;
	if (i < 0 || i >= xres || j < 0 || j >= yres) return GZ_FAILURE;

	*r = pixelbuffer[ARRAY(i, j)].red;
	*g = pixelbuffer[ARRAY(i, j)].green;
	*b = pixelbuffer[ARRAY(i, j)].blue;
	*a = pixelbuffer[ARRAY(i, j)].alpha;
	*z = pixelbuffer[ARRAY(i, j)].z;
	return GZ_SUCCESS;
}


int GzRender::GzFlushDisplay2File(FILE* outfile)
{
/* HW1.6 write image to ppm file -- "P6 %d %d 255\r" */
	if (pixelbuffer == NULL) return GZ_FAILURE;

	fprintf(outfile, "P6 %d %d 255\r", xres, yres);

	for (size_t i = 0; i < xres * yres; ++i) {
		// convert to 8 bits
		char pixel_red = (pixelbuffer[i].red >> 4) & 0xff;
		char pixel_green = (pixelbuffer[i].green >> 4) & 0xff;
		char pixel_blue = (pixelbuffer[i].blue >> 4) & 0xff;

		// write to outfile
		fwrite(&pixel_red, 1, 1, outfile);
		fwrite(&pixel_green, 1, 1, outfile);
		fwrite(&pixel_blue, 1, 1, outfile);
	}
	return GZ_SUCCESS;
}

int GzRender::GzFlushDisplay2FrameBuffer()
{
/* HW1.7 write pixels to framebuffer: 
	- put the pixels into the frame buffer
	- CAUTION: when storing the pixels into the frame buffer, the order is blue, green, and red 
	- NOT red, green, and blue !!!
*/
	if (pixelbuffer == NULL || framebuffer == NULL) return GZ_FAILURE;

	for (size_t i = 0; i < xres * yres; ++i) {
		framebuffer[3 * i] = pixelbuffer[i].blue >> 4;
		framebuffer[3 * i + 1] = pixelbuffer[i].green >> 4;
		framebuffer[3 * i + 2] = pixelbuffer[i].red >> 4;
	}
	return GZ_SUCCESS;
}


/***********************************************/
/* HW2 methods: implement from here */

int GzRender::GzPutAttribute(int numAttributes, GzToken	*nameList, GzPointer *valueList) 
{
/* HW 2.1
-- Set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
-- In later homeworks set shaders, interpolaters, texture maps, and lights
*/

/*
- GzPutAttribute() must accept the following tokens/values:

- GZ_RGB_COLOR					GzColor		default flat-shade color
- GZ_INTERPOLATE				int			shader interpolation mode
- GZ_DIRECTIONAL_LIGHT			GzLight
- GZ_AMBIENT_LIGHT            	GzLight		(ignore direction)
- GZ_AMBIENT_COEFFICIENT		GzColor		Ka reflectance
- GZ_DIFFUSE_COEFFICIENT		GzColor		Kd reflectance
- GZ_SPECULAR_COEFFICIENT       GzColor		Ks coef's
- GZ_DISTRIBUTION_COEFFICIENT   float		spec power
*/
	if (numAttributes <= 0 || nameList == NULL || valueList == NULL) return GZ_FAILURE;
	for (size_t i = 0; i < numAttributes; ++i) {
		switch (nameList[i]) {
		case GZ_RGB_COLOR: {
			Vector3f color = *(Vector3f*)valueList[i];
			color.clamp(0.0f, 1.0f);
			// assign to flatcolor
			flatcolor[0] = color[0];
			flatcolor[1] = color[1];
			flatcolor[2] = color[2];
			break;
		}
		case GZ_INTERPOLATE: {
			interp_mode = *(int*)valueList[i];
			break;
		}
		case GZ_DIRECTIONAL_LIGHT: {
			if (numlights >= MAX_LIGHTS) return GZ_FAILURE;
			GzLight light = *(GzLight*)valueList[i];
			lights[numlights] = light;
			++numlights;
			break;
		}
		case GZ_AMBIENT_LIGHT: {
			ambientlight = *(GzLight*)valueList[i];
			break;
		}
		case GZ_DIFFUSE_COEFFICIENT: {
			Vector3f* diffuse = (Vector3f*)valueList[i];
			Kd[0] = diffuse[0][0];
			Kd[1] = diffuse[0][1];
			Kd[2] = diffuse[0][2];
			break;
		}
		case GZ_AMBIENT_COEFFICIENT: {
			Vector3f* ambient = (Vector3f*)valueList[i];
			Ka[0] = ambient[0][0];
			Ka[1] = ambient[0][1];
			Ka[2] = ambient[0][2];
			break;
		}
		case GZ_SPECULAR_COEFFICIENT: {
			Vector3f* specular = (Vector3f*)valueList[i];
			Ks[0] = specular[0][0];
			Ks[1] = specular[0][1];
			Ks[2] = specular[0][2];
			break;
		}
		case GZ_DISTRIBUTION_COEFFICIENT: {
			spec = *(float*)valueList[i];
			break;
		}
		default:
			break;
		}

	}
	return GZ_SUCCESS;
}

int GzRender::GzPutTriangle(int numParts, GzToken *nameList, GzPointer *valueList)
/* numParts - how many names and values */
{
/* HW 2.2
-- Pass in a triangle description with tokens and values corresponding to
      GZ_NULL_TOKEN:		do nothing - no values
      GZ_POSITION:		3 vert positions in model space
-- Return error code
*/
/*
-- Xform positions of verts using matrix on top of stack 
-- Clip - just discard any triangle with any vert(s) behind view plane 
		- optional: test for triangles with all three verts off-screen (trivial frustum cull)
-- invoke triangle rasterizer  
*/
	if (numParts <= 0 || nameList == NULL || valueList == NULL) return GZ_FAILURE;


	Vector3f* model_vertices = (Vector3f*)valueList[0];
	Vector3f* model_normals = (Vector3f*)valueList[1];

	Vector4f vertices_4D[3];
	Vector4f normals_4D[3];
	for (size_t j = 0; j < 3; ++j) {
		vertices_4D[j] = Vector4f(model_vertices[j].x, model_vertices[j].y, model_vertices[j].z, 1.0f);
		normals_4D[j] = Vector4f(model_normals[j].x, model_normals[j].y, model_normals[j].z, 1.0f);
		vertices_4D[j] = transform_vector4D(vertices_4D[j], Ximage[matlevel]);
		normals_4D[j] = transform_vector4D(normals_4D[j], Xnorm[normal_matlevel]);
	}

	if (vertices_4D[0].z < 0 || vertices_4D[1].z < 0 || vertices_4D[2].z < 0) return GZ_FAILURE;

	// 4D to 3D
	Vector3f vertices[3];
	Vector3f normals[3];
	for (size_t i = 0; i < 3; ++i) {
		vertices[i] = vertices_4D[i].toVector3Homogeneous();
		normals[i] = normals_4D[i].toVector3Homogeneous().normalize();
	}

	// save normal 0 for later use
	Vector3f normal_0(normals[0]);

	// sort by y
	if (vertices[0].y > vertices[1].y) {
		vertices[0].swap(vertices[1]);
		normals[0].swap(normals[1]);
	}
	if (vertices[0].y > vertices[2].y) {
		vertices[0].swap(vertices[2]);
		normals[0].swap(normals[2]);
	}
	if (vertices[1].y > vertices[2].y) {
		vertices[1].swap(vertices[2]);
		normals[1].swap(normals[2]);
	}

	// check special cases
	// (int)(val + 0.5f) is to round val to the nearest integer
	// if vertices 0 and 1 are on a horizontal line
	if ((int)(vertices[0].y + 0.5f) == (int)(vertices[1].y + 0.5f)) {
		if (vertices[0].x > vertices[1].x) {
			vertices[0].swap(vertices[1]);
			normals[0].swap(normals[1]);
		}
	}
	// if vertices 1 and 2 are on a horizontal line
	else if ((int)(vertices[1].y + 0.5f) == (int)(vertices[2].y + 0.5f)) {
		if (vertices[2].x > vertices[1].x) {
			vertices[2].swap(vertices[1]);
			normals[2].swap(normals[1]);
		}
	}
	else {
		float slope_02;
		float mid_x;
		// if vertices 0 and 2 are on a vertical line
		if ((int)(vertices[0].x + 0.5f) == (int)(vertices[2].x + 0.5f)) {
			mid_x = vertices[0].x;
			slope_02 = 0.0f;
		}
		else {
			slope_02 = (vertices[2].y - vertices[0].y) / (vertices[2].x - vertices[0].x);
			mid_x = vertices[0].x + (vertices[1].y - vertices[0].y) / slope_02; // x0 + deltax
		}
		// determine the L/R edge
		if (mid_x > vertices[1].x) {
			vertices[1].swap(vertices[2]);
			normals[1].swap(normals[2]);
		}
	}

	// get the bounding box
	int min_x = (int)(min_by_x(vertices, 3) + 0.5f);
	int max_x = (int)(max_by_x(vertices, 3) + 0.5f);
	int min_y = (int)(min_by_y(vertices, 3) + 0.5f);
	int max_y = (int)(max_by_y(vertices, 3) + 0.5f);

	// get the edge equations
	float edge1A = vertices[1].y - vertices[0].y;
	float edge1B = vertices[0].x - vertices[1].x;
	float edge1C = -edge1B * vertices[0].y - edge1A * vertices[0].x;
	float edge2A = vertices[2].y - vertices[1].y;
	float edge2B = vertices[1].x - vertices[2].x;
	float edge2C = -edge2B * vertices[1].y - edge2A * vertices[1].x;
	float edge3A = vertices[0].y - vertices[2].y;
	float edge3B = vertices[2].x - vertices[0].x;
	float edge3C = -edge3B * vertices[2].y - edge3A * vertices[2].x;

	// get the plane equation
	Vector4f plane = interpolate(vertices[0], vertices[1], vertices[2]);

	// calculate the intensities
	Vector3f intensities[3];
	for (size_t i = 0; i < 3; ++i) {
		intensities[i] = calculate_intensity(normals[i], lights, numlights, ambientlight, Ks, Kd, Ka, spec);
	}


	// interpolate the intensities
	Vector4f red_plane, green_plane, blue_plane;
	red_plane = interpolate(Vector3f(vertices[0].xy, intensities[0].x), Vector3f(vertices[1].xy, intensities[1].x), Vector3f(vertices[2].xy, intensities[2].x));
	green_plane = interpolate(Vector3f(vertices[0].xy, intensities[0].y), Vector3f(vertices[1].xy, intensities[1].y), Vector3f(vertices[2].xy, intensities[2].y));
	blue_plane = interpolate(Vector3f(vertices[0].xy, intensities[0].z), Vector3f(vertices[1].xy, intensities[1].z), Vector3f(vertices[2].xy, intensities[2].z));


	// interpolate the normals
	Vector4f normal_X, normal_Y, normal_Z;
	for (size_t i = 0; i < 3; ++i) {
		normal_X = interpolate(Vector3f(vertices[0].xy, normals[0].x), Vector3f(vertices[1].xy, normals[1].x), Vector3f(vertices[2].xy, normals[2].x));
		normal_Y = interpolate(Vector3f(vertices[0].xy, normals[0].y), Vector3f(vertices[1].xy, normals[1].y), Vector3f(vertices[2].xy, normals[2].y));
		normal_Z = interpolate(Vector3f(vertices[0].xy, normals[0].z), Vector3f(vertices[1].xy, normals[1].z), Vector3f(vertices[2].xy, normals[2].z));
	}


	// rasterize
	for (int i = min_x; i <= max_x; ++i) {
		for (int j = min_y; j <= max_y; ++j) {
			// get the LEE values
			float LEE1 = edge1A * i + edge1B * j + edge1C;
			float LEE2 = edge2A * i + edge2B * j + edge2C;
			float LEE3 = edge3A * i + edge3B * j + edge3C;

			// check if in the triangle
			if ((LEE1 > 0 && LEE2 > 0 && LEE3 > 0 && plane.C != 0)
				|| (LEE1 < 0 && LEE2 < 0 && LEE3 < 0 && plane.C != 0)
				|| (LEE1 == 0 || LEE2 == 0 || LEE3 == 0) &&
				(red_plane.C != 0 && green_plane.C != 0 && blue_plane.C != 0) &&
				(normal_X.C != 0 && normal_Y.C != 0 && normal_Z.C != 0)) {

				// calulate the z value and clip it
				int z = (int)(clip(plane, i, j) + 0.5f);

				if (z >= 0) {
					GzIntensity red, green, blue;
					if (interp_mode == GZ_FLAT) {
						/*red = ctoi(flatcolor[0]);
						green = ctoi(flatcolor[1]);
						blue = ctoi(flatcolor[2]);*/
						/*red = ctoi(normals[0].x);
						green = ctoi(normals[0].y);
						blue = ctoi(normals[0].z);*/
						Vector3f intensity = calculate_intensity(normal_0, lights, numlights, ambientlight, Ks, Kd, Ka, spec);
						red = ctoi(intensity.r);
						green = ctoi(intensity.g);
						blue = ctoi(intensity.b);
					}
					else if (interp_mode == GZ_COLOR) {
						red = ctoi(clip(red_plane, i, j));
						green = ctoi(clip(green_plane, i, j));
						blue = ctoi(clip(blue_plane, i, j));
					}
					else if (interp_mode == GZ_NORMAL) {
						Vector3f interpolated_normal = Vector3f(clip(normal_X, i, j), clip(normal_Y, i, j), clip(normal_Z, i, j));
						Vector3f intensity = calculate_intensity(interpolated_normal, lights, numlights, ambientlight, Ks, Kd, Ka, spec);
						red = ctoi(intensity.r);
						green = ctoi(intensity.g);
						blue = ctoi(intensity.b);
					}
					GzPut(i, j, red, green, blue, 1, z);
				}

				// put the pixel

			}
		}
	}


	return GZ_SUCCESS;
}

