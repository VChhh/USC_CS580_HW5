#ifndef VECTOR2_H
#define VECTOR2_H

#include<cmath>
#include<stdexcept>

template <typename T>
struct Vector2 {
	union {
		T data[2];
		struct {
			T x, y;
		};
		struct {
			T u, v;
		};
		struct {
			T s, t;
		};
	};

	Vector2() {
		data[0] = 0;
		data[1] = 0;
	}

	Vector2(T x, T y) {
		data[0] = x;
		data[1] = y;
	}

	Vector2(const Vector2<T>& v) {
		data[0] = v.data[0];
		data[1] = v.data[1];
	}

	Vector2(T* arr) {
		data[0] = arr[0];
		data[1] = arr[1];
	}

	Vector2<T>& operator=(const Vector2<T>& v) {
		data[0] = v.data[0];
		data[1] = v.data[1];
		return *this;
	}

	Vector2<T> operator+(const Vector2<T>& v) const {
		return Vector2<T>(data[0] + v.data[0], data[1] + v.data[1]);
	}

	Vector2<T> operator-(const Vector2<T>& v) const {
		return Vector2<T>(data[0] - v.data[0], data[1] - v.data[1]);
	}

	Vector2<T> operator*(const Vector2<T>& v) const {
		return Vector2<T>(data[0] * v.data[0], data[1] * v.data[1]);
	}

	Vector2<T> operator/(const Vector2<T>& v) const {
		return Vector2<T>(data[0] / v.data[0], data[1] / v.data[1]);
	}

	Vector2<T> operator+(T s) const {
		return Vector2<T>(data[0] + s, data[1] + s);
	}

	Vector2<T> operator-(T s) const {
		return Vector2<T>(data[0] - s, data[1] - s);
	}

	Vector2<T> operator*(T s) const {
		return Vector2<T>(data[0] * s, data[1] * s);
	}

	Vector2<T> operator/(T s) const {
		return Vector2<T>(data[0] / s, data[1] / s);
	}

	Vector2<T>& operator+=(const Vector2<T>& v) {
		data[0] += v.data[0];
		data[1] += v.data[1];
		return *this;
	}

	Vector2<T>& operator-=(const Vector2<T>& v) {
		data[0] -= v.data[0];
		data[1] -= v.data[1];
		return *this;
	}

	Vector2<T>& operator*=(T v) {
		data[0] *= v;
		data[1] *= v;
		return *this;
	}

	Vector2<T>& operator/=(T v) {
		if (v == 0) {
			throw std::invalid_argument("Division by zero");
		}
		data[0] /= v;
		data[1] /= v;
		return *this;
	}

	Vector2<T> operator-() const {
		return Vector2<T>(-data[0], -data[1]);
	}

	T dot(const Vector2<T>& v) const {
		return data[0] * v.data[0] + data[1] * v.data[1];
	}

	T magnitude() const {
		return sqrt(data[0] * data[0] + data[1] * data[1]);
	}

	Vector2<T> normalize() const {
		T mag = magnitude();
		return Vector2<T>(data[0] / mag, data[1] / mag);
	}

	void swap(Vector2<T>& v) {
		Vector2<T> temp(v);
		v = *this;
		*this = temp;
	}
};

// define common types
typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;
typedef Vector2<short> Vector2s;

#endif // !VECTOR2_H