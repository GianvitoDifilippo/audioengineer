#include "vec4.h"

namespace audioengineer {
	namespace maths {

		vec4::vec4()
		{
			x = 0.f;
			y = 0.f;
			z = 0.f;
			w = 0.f;
		}

		vec4::vec4(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		float vec4::length() const
		{
			return sqrt(x * x + y * y + z * z + w * w);
		}

		float vec4::length2() const
		{
			return x * x + y * y + z * z + w * w;
		}

		void vec4::normalize()
		{
			float l_inv = 1.f / length();
			x *= l_inv;
			y *= l_inv;
			z *= l_inv;
			w *= l_inv;
		}

		vec4 vec4::normalized() const
		{
			float l_inv = 1.f / length();
			return vec4(x * l_inv, y * l_inv, z * l_inv, w * l_inv);
		}

		vec4 vec4::operator+(const vec4& right) const
		{
			return vec4(x + right.x, y + right.y, z + right.z, w + right.w);
		}

		vec4 vec4::operator-(const vec4& right) const
		{
			return vec4(x - right.x, y - right.y, z - right.z, w - right.w);
		}

		vec4 vec4::operator|(const vec4& right) const
		{
			return vec4(x * right.x, y * right.y, z * right.z, w * right.w);
		}

		vec4 operator*(const vec4& left, float right)
		{
			return vec4(left.x * right, left.y * right, left.z * right, left.w * right);
		}

		vec4 operator/(const vec4& left, float right)
		{
			return vec4(left.x / right, left.y / right, left.z / right, left.w / right);
		}

		vec4 operator*(float left, const vec4& right)
		{
			return vec4(left * right.x, left * right.y, left * right.z, left * right.w);
		}

		float vec4::operator*(const vec4& right) const
		{
			return x * right.x + y * right.y + z * right.z + w * right.w;
		}

		vec4& vec4::operator+=(const vec4& right)
		{
			x += right.x;
			y += right.y;
			z += right.z;
			w += right.w;
			return *this;
		}

		vec4& vec4::operator-=(const vec4& right)
		{
			x -= right.x;
			y -= right.y;
			z -= right.z;
			w -= right.w;
			return *this;
		}

		vec4& vec4::operator|=(const vec4& right)
		{
			x *= right.x;
			y *= right.y;
			z *= right.z;
			w *= right.w;
			return *this;
		}

		vec4& vec4::operator*=(float right)
		{
			x *= right;
			y *= right;
			z *= right;
			w *= right;
			return *this;
		}

		vec4& vec4::operator/=(float right)
		{
			x /= right;
			y /= right;
			z /= right;
			w /= right;
			return *this;
		}

		std::ostream& operator<<(std::ostream& os, const vec4& vec)
		{
			return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
		}
	}
}