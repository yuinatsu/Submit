#pragma once
#include "UiBase.h"
class ComboUi :
    public UiBase
{
public:
    ComboUi(const std::filesystem::path& path, const Vector2& pos);
    ~ComboUi();
private:
    void Update(float delta, ObjectManager& objMng, Controller& controller) override;
    void Draw() override;
    UiID GetUIID(void) override { return UiID::Combo; }

    int hd_[10];
    int combo_;
    SharedGraphicHandle handle_;
};

