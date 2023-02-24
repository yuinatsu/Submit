#pragma once
#include "UiBase.h"
class LoadIndicatorUi :
    public UiBase
{
public:
    LoadIndicatorUi(const std::filesystem::path& path, const Vector2& pos);
    ~LoadIndicatorUi();
private:
    void Update(float delta, ObjectManager& objMng, Controller& controller) override;
    void Draw() override;
    UiID GetUIID(void) override { return UiID::Indicator; }

    // èôÅXÇ…âÒì]Ç∑ÇÈó 
    static constexpr float SpeedRot = 3.0f;
    static constexpr float SpeedRotRad = SpeedRot * (DX_PI_F / 180.0f);
    // ï`âÊäpìx
    double angle_;
    
    float time_;
    int cnt_;
    SharedGraphicHandle handle_;
};

