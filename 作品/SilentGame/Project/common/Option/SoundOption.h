#pragma once
#include <functional>
#include <string>
#include "OptionWindow.h"

enum class SOUNDSELECT
{
    Bgm, // BGM�̒���
    Se, // SE�̒���
    Exit, // �I����ʂɖ߂�
    Max,
};



class SoundOption :
    public OptionWindow<SOUNDSELECT>
{
public:
    SoundOption(Controller& controller);

 
private:

    bool Update(float delta) final;
    void Draw(void) final;

    bool UpdateSoundSet(float delta);
    void DrawSoundSet(void);

    bool UpdateSEset(float delta);
    void DrawSet(void);

    bool UpdateBGset(float delta);
   
    int VolumeType_;
    std::function<bool(float)> updata_;
    std::function <void(void)> draw_;
};

