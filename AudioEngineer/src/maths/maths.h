#pragma once

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat2.h"
#include "mat3.h"
#include "mat4.h"

namespace audioengineer {
	namespace maths {

		vec2 operator*(const mat2& mat, const vec2& vec);
		vec3 operator*(const mat3& mat, const vec3& vec);
		vec4 operator*(const mat4& mat, const vec4& vec);

		uint rgba2color(const vec4& argb);
		vec4 color2rgba(uint color);
	}
}