#pragma once
#include "ResultDraw.h"
#include <vector>

class ResultDrawValue :
    public ResultDraw
{
public:
    ResultDrawValue(const std::string& strImg, const std::string& numImg, float startTime, const Math::Vector2& pos);
    virtual ~ResultDrawValue();

protected:

    void DrawOwn(void) final;
    int drawMax_;
    std::vector<int> val_;
private:


    std::string numImgName_;
    Math::Vector2 imgPos_;
    std::string strImgName_;
    
    Math::Vector2I imgSize_;
    
};

