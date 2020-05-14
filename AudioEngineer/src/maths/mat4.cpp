#include "mat4.h"

namespace audioengineer {
	namespace maths {

		mat4::mat4()
		{
			for (int i = 0; i < 16; i++) elements[i] = 0.f;
		}

		mat4::mat4(float elements[16])
		{
			for (int i = 0; i < 16; i++) this->elements[i] = elements[i];
		}

		mat4::mat4(std::initializer_list<float> elements)
		{
			const float* ptr = elements.begin();
			for (int i = 0; i < 16; i++) this->elements[i] = *ptr++;
		}

		float mat4::at(uint i, uint j) const
		{
			return elements[i * 4 + j];
		}

		void mat4::transpose()
		{
			for (int i = 1; i < 4; i++) {
				for (int j = 0; j < i; j++) {
					float temp = elements[i * 4 + j];
					elements[i * 4 + j] = elements[j * 4 + i];
					elements[j * 4 + i] = temp;
				}
			}
		}

		mat4 mat4::transposed() const
		{
			mat4 result(*this);
			result.transpose();
			return result;
		}

		float& mat4::operator()(uint i, uint j)
		{
			return elements[i * 4 + j];
		}

		mat4 mat4::operator+(const mat4& right) const
		{
			mat4 result;
			for (int i = 0; i < 16; i++) result.elements[i] = elements[i] + right.elements[i];
			return result;
		}

		mat4 mat4::operator-(const mat4& right) const
		{
			mat4 result;
			for (int i = 0; i < 16; i++) result.elements[i] = elements[i] - right.elements[i];
			return result;
		}

		mat4 mat4::operator|(const mat4& right) const
		{
			mat4 result;
			for (int i = 0; i < 16; i++) result.elements[i] = elements[i] * right.elements[i];
			return result;
		}

		mat4 mat4::operator*(const mat4& right) const
		{
			mat4 result;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					for (int k = 0; k < 4; k++) {
						result(i, j) += at(i, k) * right.at(k, j);
					}
				}
			}
			return result;
		}

		mat4 operator*(const mat4& left, float right)
		{
			mat4 result;
			for (int i = 0; i < 16; i++) result.elements[i] = left.elements[i] * right;
			return result;
		}

		mat4 operator/(const mat4& left, float right)
		{
			mat4 result;
			for (int i = 0; i < 16; i++) result.elements[i] = left.elements[i] / right;
			return result;
		}

		mat4 operator*(float left, const mat4& right)
		{
			mat4 result;
			for (int i = 0; i < 16; i++) result.elements[i] = left * right.elements[i];
			return result;
		}

		mat4& mat4::operator+=(const mat4& right)
		{
			for (int i = 0; i < 16; i++) elements[i] += right.elements[i];
			return *this;
		}

		mat4& mat4::operator-=(const mat4& right)
		{
			for (int i = 0; i < 16; i++) elements[i] -= right.elements[i];
			return *this;
		}

		mat4& mat4::operator|=(const mat4& right)
		{
			for (int i = 0; i < 16; i++) elements[i] *= right.elements[i];
			return *this;
		}

		mat4& mat4::operator*=(const mat4& right)
		{
			float new_elements[16];
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					new_elements[i * 4 + j] = 0.f;
					for (int k = 0; k < 4; k++) {
						new_elements[i * 4 + j] += elements[i * 4 + k] * right.elements[k * 4 + j];
					}
				}
			}
			for (int i = 0; i < 16; i++) elements[i] = new_elements[i];
			return *this;
		}

		mat4& mat4::operator^=(const mat4& right)
		{
			float new_elements[16];
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					new_elements[i * 4 + j] = 0.f;
					for (int k = 0; k < 4; k++) {
						new_elements[i * 4 + j] += right.elements[i * 4 + k] * elements[k * 4 + j];
					}
				}
			}
			for (int i = 0; i < 16; i++) elements[i] = new_elements[i];
			return *this;
		}

		mat4& mat4::operator*=(float right)
		{
			for (int i = 0; i < 16; i++) elements[i] *= right;
			return *this;
		}

		mat4& mat4::operator/=(float right)
		{
			for (int i = 0; i < 16; i++) elements[i] /= right;
			return *this;
		}

		std::ostream& operator<<(std::ostream& os, const mat4& mat)
		{
			return os
				<< "[" << mat.elements[ 0] << ", " << mat.elements[ 1] << ", " << mat.elements[ 2] << ", " << mat.elements[ 3] << "]"
				<< "[" << mat.elements[ 4] << ", " << mat.elements[ 5] << ", " << mat.elements[ 6] << ", " << mat.elements[ 7] << "]"
				<< "[" << mat.elements[ 8] << ", " << mat.elements[ 9] << ", " << mat.elements[10] << ", " << mat.elements[11] << "]"
				<< "[" << mat.elements[12] << ", " << mat.elements[13] << ", " << mat.elements[14] << ", " << mat.elements[15] << "]";
		}
	}
}