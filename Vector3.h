#ifndef VECTOR3_H
#define VECTOR3_H

#include<cmath>
#include<stdexcept>

template <typename T>
T clamp(T val, T min_val, T max_val) {
	if (val < min_val) return min_val;
	if (val > max_val) return max_val;
	return val;
}

// define a 3D vector class
template <typename T>
struct Vector3 {
	union {
		T data[3];
		struct {
			T x, y, z;
		};
		struct {
			T r, g, b;
		};
		struct {
			T A, B, C;
		};
		struct {
			T xy[2];
			T z;
		};
	};

	Vector3() {
		data[0] = 0;
		data[1] = 0;
		data[2] = 0;
	};

	Vector3(T x, T y, T z) {
		data[0] = x;
		data[1] = y;
		data[2] = z;
	};

	Vector3(const Vector3<T>& v) {
		data[0] = v.data[0];
		data[1] = v.data[1];
		data[2] = v.data[2];
	};

	Vector3(T* arr) {
		data[0] = arr[0];
		data[1] = arr[1];
		data[2] = arr[2];
	};

	Vector3(T* xy, T z) {
		data[0] = xy[0];
		data[1] = xy[1];
		data[2] = z;
	}


	// overload operators
	Vector3<T>& operator=(const Vector3<T>& v) {
		data[0] = v.data[0];
		data[1] = v.data[1];
		data[2] = v.data[2];
		return *this;
	}

	T& operator[](int index) {
		if (index < 0 || index >= 3) throw std::out_of_range("Index out of range");
		return data[index];
	}

	const T& operator[](int index) const {
		if (index < 0 || index >= 3) throw std::out_of_range("Index out of range");
		return data[index];
	}

	Vector3<T> operator+(const Vector3<T>& v) const {
		return Vector3<T>(data[0] + v.data[0], data[1] + v.data[1], data[2] + v.data[2]);
	}

	Vector3<T> operator-(const Vector3<T>& v) const {
		return Vector3<T>(data[0] - v.data[0], data[1] - v.data[1], data[2] - v.data[2]);
	}

	Vector3<T> operator*(const Vector3<T>& v) const {
		return Vector3<T>(data[0] * v.data[0], data[1] * v.data[1], data[2] * v.data[2]);
	}

	Vector3<T> operator*(const T& k) const {
		return Vector3<T>(data[0] * k, data[1] * k, data[2] * k);
	}

	Vector3<T> operator/(const T& k) const {
		return Vector3<T>(data[0] / k, data[1] / k, data[2] / k);
	}

	Vector3<T> operator-() const {
		return Vector3<T>(-data[0], -data[1], -data[2]);
	}

	Vector3<T> operator+=(const Vector3<T>& v) {
		data[0] += v.data[0];
		data[1] += v.data[1];
		data[2] += v.data[2];
		return *this;
	}

	Vector3<T> operator-=(const Vector3<T>& v) {
		data[0] -= v.data[0];
		data[1] -= v.data[1];
		data[2] -= v.data[2];
		return *this;
	}

	Vector3<T> operator*=(const T& k) {
		data[0] *= k;
		data[1] *= k;
		data[2] *= k;
		return *this;
	}

	Vector3<T> operator/=(const T& k) {
		data[0] /= k;
		data[1] /= k;
		data[2] /= k;
		return *this;
	}
	// end of overload operators


	// vector operations
	T dot(const Vector3<T>& v) const {
		return data[0] * v.data[0] + data[1] * v.data[1] + data[2] * v.data[2];
	}

	Vector3<T> cross(const Vector3<T>& v) const {
		return Vector3<T>(
			data[1] * v.data[2] - data[2] * v.data[1],
			data[2] * v.data[0] - data[0] * v.data[2],
			data[0] * v.data[1] - data[1] * v.data[0]
		);
	}

	T magnitude() const {
		return std::sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]);
	}

	Vector3<T> normalize() const {
		T maganitude = magnitude();
		return Vector3<T>(data[0] / maganitude, data[1] / maganitude, data[2] / maganitude);
	}
	// end of vector operations


	// clamp operations
	Vector3<T> clamp(T min_val, T max_val) const {
		return Vector3<T>(
			::clamp(data[0], min_val, max_val),
			::clamp(data[1], min_val, max_val),
			::clamp(data[2], min_val, max_val)
		);
	}

	void clamp(T min_val, T max_val) {
		data[0] = ::clamp(data[0], min_val, max_val);
		data[1] = ::clamp(data[1], min_val, max_val);
		data[2] = ::clamp(data[2], min_val, max_val);
	}

	Vector3<T> clamp(Vector3<T> min_val_vec, Vector3<T> max_val_vec) const {
		return Vector3<T>(
			::clamp(data[0], min_val_vec.data[0], max_val_vec.data[0]),
			::clamp(data[1], min_val_vec.data[1], max_val_vec.data[1]),
			::clamp(data[2], min_val_vec.data[2], max_val_vec.data[2])
		);
	}

	void clamp(Vector3<T> min_val_vec, Vector3<T> max_val_vec) {
		data[0] = ::clamp(data[0], min_val_vec.data[0], max_val_vec.data[0]);
		data[1] = ::clamp(data[1], min_val_vec.data[1], max_val_vec.data[1]);
		data[2] = ::clamp(data[2], min_val_vec.data[2], max_val_vec.data[2]);
	}
	// end of clamp operations


	// compare operations
	// ignore the accuracy of floating point for now, will fix it later
	bool operator==(const Vector3<T>& v) const {
		return data[0] == v.data[0] && data[1] == v.data[1] && data[2] == v.data[2];
	}

	bool operator!=(const Vector3<T>& v) const {
		return data[0] != v.data[0] || data[1] != v.data[1] || data[2] != v.data[2];
	}

	bool lesser_x(const Vector3<T>& v) const {
		return data[0] < v.data[0];
	}

	bool lesser_y(const Vector3<T>& v) const {
		return data[1] < v.data[1];
	}

	bool lesser_z(const Vector3<T>& v) const {
		return data[2] < v.data[2];
	}

	bool greater_x(const Vector3<T>& v) const {
		return data[0] > v.data[0];
	}

	bool greater_y(const Vector3<T>& v) const {
		return data[1] > v.data[1];
	}

	bool greater_z(const Vector3<T>& v) const {
		return data[2] > v.data[2];
	}
	// end of compare operations


	// help functions
	void swap(Vector3<T>& v) {
		T temp[3] = { data[0], data[1], data[2] };
		data[0] = v.data[0];
		data[1] = v.data[1];
		data[2] = v.data[2];
		v.data[0] = temp[0];
		v.data[1] = temp[1];
		v.data[2] = temp[2];
	}
};

// non-member operation functions
template <typename T>
Vector3<T> operator*(const T& k, const Vector3<T>& v) {
	return Vector3<T>(v[0] * k, v[1] * k, v[2] * k);
}

template <typename T>
Vector3<T> dot(const Vector3<T>& v1, T* v2) {
	return v1.dot(v2);
}

template <typename T>
Vector3<T> dot(T* v1, T* v2) {
	return v1.dot(v2);
}

template <typename T>
Vector3<T> clamp(const Vector3<T>& v, T min_val, T max_val) {
	return v.clamp(min_val, max_val);
}


// min and max functions
template <typename T>
T min_by_x(Vector3<T> *vecs, int size) {
	T min_val = vecs[0].x;
	for (int i = 1; i < size; ++i) {
		if (vecs[i].x < min_val) min_val = vecs[i].x;
	}
	return min_val;
}

template <typename T>
T min_by_y(Vector3<T>* vecs, int size) {
	T min_val = vecs[0].y;
	for (int i = 1; i < size; ++i) {
		if (vecs[i].y < min_val) min_val = vecs[i].y;
	}
	return min_val;
}

template <typename T>
T min_by_z(Vector3<T>* vecs, int size) {
	T min_val = vecs[0].z;
	for (int i = 1; i < size; ++i) {
		if (vecs[i].z < min_val) min_val = vecs[i].z;
	}
	return min_val;
}

template <typename T>
T max_by_x(Vector3<T>* vecs, int size) {
	T max_val = vecs[0].x;
	for (int i = 1; i < size; ++i) {
		if (vecs[i].x > max_val) max_val = vecs[i].x;
	}
	return max_val;
}

template <typename T>
T max_by_y(Vector3<T>* vecs, int size) {
	T max_val = vecs[0].y;
	for (int i = 1; i < size; ++i) {
		if (vecs[i].y > max_val) max_val = vecs[i].y;
	}
	return max_val;
}

template <typename T>
T max_by_z(Vector3<T>* vecs, int size) {
	T max_val = vecs[0].z;
	for (int i = 1; i < size; ++i) {
		if (vecs[i].z > max_val) max_val = vecs[i].z;
	}
	return max_val;
}


// define common types
typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;
typedef Vector3<short> Vector3s;

// extension for T[3] form of vector3
template <typename T>
Vector3<T> operator*(T* v1, const T& k) {
	return Vector3<T>(v1[0] * k, v1[1] * k, v1[2] * k);
}

template <typename T>
Vector3<T> operator*(const T& k, T* v1) {
	return Vector3<T>(v1[0] * k, v1[1] * k, v1[2] * k);
}



#endif // VECTOR3_H