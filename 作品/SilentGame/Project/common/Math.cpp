#include <DxLib.h>
#include <cmath>
#include "Math.h"

float Math::GetNormalizedAngle(float angle, float valMin, float valMax)
{
	float cycle = valMax - valMin;
	float result = std::fmod((angle - valMin), cycle + valMin);
	if (result < valMin)
	{
		result += cycle;
	}
	return result;
}

double Math::GetNormalizedAngle(double angle, double valMin, double valMax)
{
	double cycle = valMax - valMin;
	double result = std::fmod((angle - valMin), cycle + valMin);
	if (result < valMin)
	{
		result += cycle;
	}
	return result;
}




Math::Matrix Math::IdentityMat()
{

	Math::Matrix ret = {};
	ret.m[0][0] = ret.m[1][1] = ret.m[2][2] = 1;
	return ret;
}

Math::Matrix Math::TranslateMat(float x, float y)
{
	Math::Matrix ret = {};
	ret = Math::IdentityMat();
	ret.m[0][2] = x;
	ret.m[1][2] = y;
	return ret;
}

Math::Matrix Math::RotateMat(float angle)
{
	Math::Matrix ret = {};
	ret = IdentityMat();
	ret.m[0][0] = cos(angle);
	ret.m[0][1] = -sin(angle);
	ret.m[1][0] = sin(angle);
	ret.m[1][1] = cos(angle);
	return ret;
}

Math::Matrix Math::MultipleMat(const Matrix& lmat, const Matrix& rmat)
{
	Math::Matrix ret = {};
	for (int k = 0; k <= 2; k++)
	{
		for (int j = 0; j <= 2; j++)
		{
			for (int i = 0; i <= 2; i++)
			{
				ret.m[k][j] += lmat.m[k][j] * rmat.m[i][j];
			}
		}
	}
	return ret;
}

Math::Vector2 Math::MultipleVec(const Matrix& mat, const Math::Vector2& vec)
{
	Math::Vector2 ret = {};
	ret.x = mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2];
	ret.y = mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2];
	return ret;
}

Math::Matrix Math::operator*(const Matrix& lmat, const Matrix& rmat)
{
	return Math::MultipleMat(lmat, rmat);
}

Math::Vector2 Math::operator*(const Matrix& mat, const Math::Vector2& vec)
{
	return Math::MultipleVec(mat,vec);
}
