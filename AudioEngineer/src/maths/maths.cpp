#include "maths.h"

namespace audioengineer {
	namespace maths {

		vec2 operator*(const mat2& mat, const vec2& vec)
		{
			return vec2(
				mat.elements[0] * vec.x + mat.elements[2] * vec.y,
				mat.elements[1] * vec.x + mat.elements[3] * vec.y);
		}

		vec3 operator*(const mat3& mat, const vec3& vec)
		{
			return vec3(
				mat.elements[0] * vec.x + mat.elements[3] * vec.y + mat.elements[6] * vec.z,
				mat.elements[1] * vec.x + mat.elements[4] * vec.y + mat.elements[7] * vec.z,
				mat.elements[2] * vec.x + mat.elements[5] * vec.y + mat.elements[8] * vec.z);
		}

		vec4 operator*(const mat4& mat, const vec4& vec)
		{
			return vec4(
				mat.elements[ 0] * vec.x + mat.elements[ 4] * vec.y + mat.elements[ 8] * vec.z + mat.elements[12] * vec.w,
				mat.elements[ 1] * vec.x + mat.elements[ 5] * vec.y + mat.elements[ 9] * vec.z + mat.elements[13] * vec.w,
				mat.elements[ 2] * vec.x + mat.elements[ 6] * vec.y + mat.elements[10] * vec.z + mat.elements[14] * vec.w,
				mat.elements[ 3] * vec.x + mat.elements[ 7] * vec.y + mat.elements[11] * vec.z + mat.elements[15] * vec.w
				);
		}

		uint argb2color(const vec4& argb)
		{
			return 0;
		}
	}
}