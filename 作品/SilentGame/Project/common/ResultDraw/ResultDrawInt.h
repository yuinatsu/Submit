#pragma once
#include <vector>
#include "ResultDrawValue.h"

// ���U���g�V�[���Ō��ʂ�`��(�����l)
class ResultDrawInt :
    public ResultDrawValue
{
public:
    ResultDrawInt(int val,const std::string& strImg, const std::string& numImg,float startTime, const Math::Vector2& pos);
    virtual ~ResultDrawInt();
};

