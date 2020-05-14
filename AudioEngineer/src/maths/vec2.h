#pragma once

#include <iostream>
#include "..\defines.h"

namespace audioengineer {
	namespace maths {

		struct vec2
		{
			float x, y;

			vec2();
			vec2(float x, float y);

			float length() const;
			float length2() const;

			void normalize();
			vec2 normalized() const;

			vec2 operator+(const vec2& right) const;
			vec2 operator-(const vec2& right) const;
			vec2 operator|(const vec2& right) const;
			friend vec2 operator*(const vec2& left, float right);
			friend vec2 operator/(const vec2& left, float right);
			friend vec2 operator*(float left, const vec2& right);
			float operator*(const vec2& right) const;
			vec2& operator+=(const vec2& right);
			vec2& operator-=(const vec2& right);
			vec2& operator|=(const vec2& right);
			vec2& operator*=(float right);
			vec2& operator/=(float right);
			friend std::ostream& operator<<(std::ostream& os, const vec2& vec);
		};

	}
}