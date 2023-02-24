#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include "DxLib.h"
#include "AsoUtility.h"

std::vector<std::string> AsoUtility::Split(std::string& line, char delimiter)
{

    std::istringstream stream(line);
    std::string field;
    std::vector<std::string> result;
    
    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }

    return result;

}

double AsoUtility::Rad2DegD(double rad)
{
    return rad * (180.0 / DX_PI);
}

double AsoUtility::Deg2RadD(double deg)
{
    return deg * (DX_PI / 180.0);
}

float AsoUtility::Rad2DegF(float rad)
{
    return rad * (180.0f / DX_PI_F);
}

float AsoUtility::Deg2RadF(float deg)
{
    return deg * (DX_PI_F / 180.0f);
}

double AsoUtility::DegIn360(double deg)
{

    deg = fmodf(deg, 360.0);
    if (deg < 0.0)
    {
        deg += 360.0;
    }

    return deg;

}

double AsoUtility::RadIn2PI(double rad)
{

    rad = fmodf(rad, DX_TWO_PI);
    if (rad < 0.0f)
    {
        rad += DX_TWO_PI;
    }

    return rad;
}

int AsoUtility::DirNearAroundRad(float from, float to)
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

int AsoUtility::DirNearAroundDeg(float from, float to)
{

    float ret = 1.0f;

    float diff = to - from;

    if (diff >= 0.0f)
    {

        // ��r���������v���Ɉʒu����

        if (diff > 180.0f)
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

        if (diff < -180.0f)
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

double AsoUtility::Lerp(double start, double end, double t)
{
    // ���`���
    if (t >= 1.0)
    {
        return end;
    }

    int ret = start;
    ret += t * (end - start);
    return ret;
}

double AsoUtility::LerpDeg(double start, double end, double t)
{

    float ret;

    float diff = end - start;
    if (diff < -180.0)
    {
        end += 360.0f;
        ret = Lerp(start, end, t);
        if (ret >= 360.0)
        {
            ret -= 360.0;
        }
    }
    else if (diff > 180.0)
    {
        end -= 360.0;
        ret = Lerp(start, end, t);
        if (ret < 0.0)
        {
            ret += 360.0;
        }
    }
    else
    {
        ret = Lerp(start, end, t);
    }

    return ret;

}

VECTOR AsoUtility::RotXZPos(VECTOR centerPos, VECTOR radiusPos, double rad)
{
    // �������Ȃ��Ă悢
    float x = ((radiusPos.x - centerPos.x) * cos(rad)) - ((radiusPos.z - centerPos.z) * sin(rad));
    float z = ((radiusPos.x - centerPos.x) * sin(rad)) + ((radiusPos.z - centerPos.z) * cos(rad));
    return { centerPos.x + x, radiusPos.y, centerPos.z + z };
}

double AsoUtility::Magnitude(VECTOR v)
{
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

double AsoUtility::Distance(VECTOR v1, VECTOR v2)
{
    return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2));
}
