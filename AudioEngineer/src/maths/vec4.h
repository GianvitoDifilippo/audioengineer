#pragma once

#include <iostream>
#include "..\defines.h"

namespace audioengineer {
	namespace maths {

		struct vec4
		{
			float x, y, z, w;

			vec4();
			vec4(float x, float y, float z, float w);

			float length() const;
			float length2() const;

			void normalize();
			vec4 normalized() const;

			vec4 operator+(const vec4& right) const;
			vec4 operator-(const vec4& right) const;
			vec4 operator|(const vec4& right) const;
			friend vec4 operator*(const vec4& left, float right);
			friend vec4 operator/(const vec4& left, float right);
			friend vec4 operator*(float left, const vec4& right);
			float operator*(const vec4& right) const;
			vec4& operator+=(const vec4& right);
			vec4& operator-=(const vec4& right);
			vec4& operator|=(const vec4& right);
			vec4& operator*=(float right);
			vec4& operator/=(float right);
			friend std::ostream& operator<<(std::ostream& os, const vec4& vec);
		};

	}
}