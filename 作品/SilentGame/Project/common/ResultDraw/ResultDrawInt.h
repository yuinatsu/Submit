#pragma once
#include <vector>
#include "ResultDrawValue.h"

// リザルトシーンで結果を描画(整数値)
class ResultDrawInt :
    public ResultDrawValue
{
public:
    ResultDrawInt(int val,const std::string& strImg, const std::string& numImg,float startTime, const Math::Vector2& pos);
    virtual ~ResultDrawInt();
};

