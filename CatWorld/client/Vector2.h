#pragma once
#include<cmath>
class Vector2
{
public:
	float x = 0;
	float y = 0;
public:
	Vector2() = default;
	~Vector2() = default;

	Vector2(float x, float y)
		: x(x), y(y) {
	}


	Vector2 operator+(const Vector2& vec) const {
		return Vector2(x + vec.x, y + vec.y);
	}

	void operator+=(const Vector2& vec) {
		x += vec.x, y += vec.y;
	}

	Vector2 operator-(const Vector2& vec) const {
		return Vector2(x - vec.x, y - vec.y);
	}

	void operator-=(const Vector2& vec) {
		x -= vec.x, y -= vec.y;
	}

	float operator*=(const Vector2& vec) {
		return x * vec.x + y * vec.y;
	}
	Vector2 operator*(float vec) const {
		return Vector2(x * vec, y * vec);
	}
	void operator*=(float vec) {
		x *= vec, y *= vec;
	}
	Vector2 operator/(float vec) const {
		return Vector2(x / vec, y / vec);
	}
	void operator/=(float vec) {
		x /= vec, y /= vec;
	}
	float length() const {//Á½µã×ø±ê¾àÀë
		return sqrt(x * x + y * y);
	}
	bool approx(const Vector2& vec) const
	{
		return (*this - vec).length() <= 0.0001f;
	}
	Vector2 normalize() {
		float len = length();
		if (len == 0)
			return Vector2(0, 0);

		return Vector2(x / len, y / len);
	}

};
