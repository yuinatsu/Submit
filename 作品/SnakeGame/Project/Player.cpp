#include <DxLib.h>
#include "Player.h"
#include "_debug/_DebugDispOut.h"
#include "_debug/_DebugConOut.h"

int Player::pCnt = 0;                // �����l  �v���C���[�����O

// Player�̐���
//---------------
Player::Player()
{
    id_ = pCnt % PLAYER_MAX;                            // �v���C���[�����̗]��Ŗ������id_�𑝂��Ȃ��悤�ɂ���
    pCnt++;                                             // �J�E���g�A�b�v

    Init(CHAR_ID::MAX);                                 // Init���Ăяo��

    memset(keyNow_, 0, sizeof(keyNow_));                // keyNow���Z�b�g
    memset(keyOld_, 0, sizeof(keyOld_));                // keyOld���Z�b�g
}


// Player�̔j��
//---------------
Player::~Player()
{
    pCnt--;                                             // �J�E���g����
}


// Player�̍X�V
//---------------
PL_ST Player::Update(MapCtl& mapCtl, char* keyData, char* keyDataOld)
{
    if (state_ == PL_ST::DEAD)                                                  // �������łɎ���ł����烊�^�[��
    {
        return state_;                                                          // DEAD�����^�[��
    }
    
    GetKeyState(keyData);                                                       // �L�[�f�[�^�擾

    // �L�����ړ��֌W
    //-----------------
    if ((pos_.x % CHIP_SIZE == 0) && (pos_.y % CHIP_SIZE == 0))                 // �u���b�N�̒��S��ʂ��Ă邩���f        ���W % �P�̃u���b�N�̃T�C�Y = 0 �̎��A�ړ����Ă��铹�̓u���b�N�̒����ɂȂ�
    {
        if (mapCtl.isOnBlock(pos_))                                             // isOnBlock��true�ł����state_�̏�Ԃ�ω�������
        {
            state_ = PL_ST::DEAD;                                               // ���񂾏�Ԃɂ���
            return state_;                                                      // DEAD�����^�[��
        }

        mapCtl.SetBlock(pos_);                                                  // �u���b�N�̔z�u

        for (int dir = 0; dir < static_cast<int>(DIR::MAX); dir++)
        {
            if (keyNow_[dir])
            {
                dir_ = static_cast<DIR>(dir);                                   // �����w��
            }
        }
    }

    switch (dir_)                                                               // �L�����̈ړ�
    {
    case DIR::LEFT:
        pos_.x -= PLAYER_DEF_SPEED;
        break;

    case DIR::RIGHT:
        pos_.x += PLAYER_DEF_SPEED;
        break;

    case DIR::UP:
        pos_.y -= PLAYER_DEF_SPEED;
        break;

    case DIR::DOWN:
        pos_.y += PLAYER_DEF_SPEED;
        break;
    }

    pAnimCnt_++;                                                                // �A�j���[�V�����J�E���g�A�b�v

    return state_;                                                              // �����Ȃ����ALIVE�����^�[��
}


// �L�[�f�[�^�擾
//-----------------
void Player::GetKeyState(char* keyData)
{
    memcpy(keyOld_, keyNow_, sizeof(keyNow_));                          // �ŐV�̏��(keyNow)��keyOld�Ɋi�[

    for (int dir = 0; dir < static_cast<int>(DIR::MAX); dir++)          // keyList�̃f�[�^��keyNow�Ɋi�[
    {
        keyNow_[dir] = keyData[keyList_[dir]];
    }
}


// Player�̕`��
//---------------
void Player::Draw(void)
{
    DrawGraph(DRAW_OFFSET_X + pos_.x, DRAW_OFFSET_Y + pos_.y - CHIP_SIZE / 3, charImage_.chipImage[static_cast<int>(dir_)][abs(((pAnimCnt_ / 10) % 4) - 2)], true);                       // �L�����`��           // ���ړ��̍ۈ�a��������̂ŁA�O���t�B�b�N�������炵�ē���������������ɂ��邱�Ƃŉ���������

    if (id_ == 0)
    {
        DrawTurnGraph(IconPos_.x, 0, charImage_.iconImage_, true);                      // 1P�L�����A�C�R��
    }
    else
    {
        DrawGraph(IconPos_.x, 0, charImage_.iconImage_, true);                          // 2P�L�����A�C�R��
    }
}


// Player�̏�����
//-----------------
bool Player::Init(CHAR_ID charID)
{
    if (id_ == 0)                                                   // 1P�̏��
    {
        charID_ = CHAR_ID::REIMU;
        keyList_[static_cast<int>(DIR::DOWN)] = KEY_INPUT_S;        // ��
        keyList_[static_cast<int>(DIR::LEFT)] = KEY_INPUT_A;        // ��
        keyList_[static_cast<int>(DIR::RIGHT)] = KEY_INPUT_D;       // �E
        keyList_[static_cast<int>(DIR::UP)] = KEY_INPUT_W;          // ��

        dir_ = DIR::RIGHT;                                          // ����

        IconPos_.x = DRAW_OFFSET_X + CHIP_SIZE;                     // �A�C�R����X���W

        pos_.x = CHIP_SIZE * 4;                                     // �v���C���[��X���W
    }
    else                                                                 // 2P�̏��
    {
        charID_ = CHAR_ID::MARISA;
        keyList_[static_cast<int>(DIR::DOWN)] = KEY_INPUT_DOWN;          // ��
        keyList_[static_cast<int>(DIR::LEFT)] = KEY_INPUT_LEFT;          // ��
        keyList_[static_cast<int>(DIR::RIGHT)] = KEY_INPUT_RIGHT;        // �E
        keyList_[static_cast<int>(DIR::UP)] = KEY_INPUT_UP;              // ��

        dir_ = DIR::LEFT;                                                // ����

        IconPos_.x = DRAW_OFFSET_X + CHIP_SIZE * 19 + (CHIP_SIZE / 2);   // �A�C�R����X���W

        pos_.x = CHIP_SIZE * (MAP_CHIP_COUNT_X - 5);                     // �v���C���[��X���W
    }

    state_ = PL_ST::NON;                                // �L�����̏�����Ԃ̏��

    if (charID != CHAR_ID::MAX)
    {
        charID_ = charID;                               // charID_�Ɏ󂯎������������
        charImage_.Init(charID_);                       // �L�����摜��ǂݍ���
        state_ = PL_ST::ALIVE;                          // �L�����̏�ԕω�
    }

    pos_.y = CHIP_SIZE * 5;                             // �v���C���[��Y���W

    speed_ = PLAYER_DEF_SPEED;                          // �v���C���[�̈ړ���

    pAnimCnt_ = 0;                                      // �A�j���[�V�����J�E���g

    return true;
}


// Player�̉��
//---------------
bool Player::Relese(void)
{
    charImage_.Release();                   // charImage�̉��
    return true;
}
