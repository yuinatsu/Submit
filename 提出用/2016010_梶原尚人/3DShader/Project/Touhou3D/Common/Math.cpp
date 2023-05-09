#include <DxLib.h>
#include <cmath>
#include "Math.h"

float GetNormalizedAngle(float angle, float valMin, float valMax)
{
    float cycle = valMax - valMin;
    float result = std::fmod((angle - valMin), cycle + valMin);
    if (result < valMin)
    {
        result += cycle;
    }
    return result;
}

double GetNormalizedAngle(double angle, double valMin, double valMax)
{
    double cycle = valMax - valMin;
    double result = std::fmod((angle - valMin), cycle + valMin);
    if (result < valMin)
    {
        result += cycle;
    }
    return result;
}

int DirNearAroundRad(float from, float to)
{
    float ret = 1.0f;
    float diff = to - from;
    if (diff >= 0.0f)
    {
        // ��r���������v���Ɉʒu����
        if (diff > DX_PI_F)
        {
            // �ł��A180�x�ȏ㗣��Ă���̂ŁA�����v���̕����߂�
            ret = -1.0f;
        }
        else
        {
            // ���v���
            ret = 1.0f;
        }
    }
    else
    {
        // ��r�����������v���Ɉʒu����
        if (diff < -DX_PI_F)
        {
            // �ł��A180�x�ȏ㗣��Ă���̂ŁA���v���̕����߂�
            ret = 1.0f;
        }
        else
        {
            // �����v���
            ret = -1.0f;
        }
    }
    return ret;
}

float RadIn2PI(double rad)
{
    rad = fmodf(rad, DX_TWO_PI);
    if (rad < 0.0f)
    {
        rad += DX_TWO_PI;
    }
    return rad;
}

void Vector3ToDxVector(const Vector3& vec3, VECTOR& dxVec)
{
    dxVec = VGet(vec3.x, vec3.y, vec3.z);
}

