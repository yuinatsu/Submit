#pragma once
#include "ResultDrawValue.h"
#include <vector>

class ResultDrawFloat :
    public ResultDrawValue
{
public:
    ResultDrawFloat(float val, const std::string& strImg, const std::string& numImg, float startTime, const Math::Vector2& pos);
    virtual ~ResultDrawFloat();

};

