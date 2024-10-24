#ifndef GZMATRIX_EXTENSION_H
#define GZMATRIX_EXTENSION_H

// define a 4*4 float matrix
// as a replacement of GzMatrix
typedef float _gzmatrix[4][4];

// identity matrix
void identity_matrix(_gzmatrix& matrix) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			matrix[i][j] = (i == j) ? 1 : 0;
		}
	}
}

// matrix copy
// copy src matrix to dest matrix
void copy_matrix(_gzmatrix& dest, _gzmatrix src) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			dest[i][j] = src[i][j];
		}
	}
}

// matrix transpose
void matrix_transpose(_gzmatrix& matrix) {
	_gzmatrix temp;
	copy_matrix(temp, matrix);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			matrix[i][j] = temp[j][i];
		}
	}
}

void matrix_transpose(const _gzmatrix& matrix, _gzmatrix& result) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result[i][j] = matrix[j][i];
		}
	}
}

// matrix addition
void matrix_add(_gzmatrix& result, const _gzmatrix& a, const _gzmatrix& b) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result[i][j] = a[i][j] + b[i][j];
		}
	}
}

void matrix_add(_gzmatrix& result, const _gzmatrix a, const _gzmatrix b) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result[i][j] = a[i][j] + b[i][j];
		}
	}
}

// matrix subtraction
void matrix_subtract(_gzmatrix& result, const _gzmatrix& a, const _gzmatrix& b) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result[i][j] = a[i][j] - b[i][j];
		}
	}
}

void matrix_subtract(_gzmatrix& result, const _gzmatrix a, const _gzmatrix b) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result[i][j] = a[i][j] - b[i][j];
		}
	}
}



// Matrix multiplication
// result = a * b
void matrix_multiply(_gzmatrix& result, const _gzmatrix& a, const _gzmatrix& b) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				result[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}

void matrix_multiply(_gzmatrix& result, const _gzmatrix a, const _gzmatrix b) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				result[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}

// matrix scalar multiplication
void matrix_scale(_gzmatrix& matrix, float scalar) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			matrix[i][j] *= scalar;
		}
	}
}

void matrix_scale(const _gzmatrix& matrix, _gzmatrix& result, float scalar) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result[i][j] = matrix[i][j] * scalar;
		}
	}
}

// matrix inverse scale
void matrix_inverse_scale(_gzmatrix& matrix, float scalar) {
	if (scalar == 0) {
		return;
	}
	matrix_scale(matrix, 1 / scalar);
}


// helper functions
void matrix_3by3_scale(_gzmatrix matrix, float scalar) {
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			matrix[i][j] *= scalar;
		}
	}
}

void matrix_3by3_inverse_scale(_gzmatrix matrix, float scalar) {
	if (scalar == 0) {
		return;
	}
	matrix_3by3_scale(matrix, 1 / scalar);
}




#endif // !GZMATRIX_EXTENSION_H