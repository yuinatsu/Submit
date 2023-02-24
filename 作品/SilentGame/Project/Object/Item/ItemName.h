#pragma once

/// <summary> �A�C�e���̎�� </summary>
enum class ItemName
{
    Knife,          // �i�C�t
    Decoy,          // �f�R�C
    Alcohol,        // ����
    LaundMine,      // �n��
    Clock,          // ���v
    Key,            // �h�A�̃L�[�J�[�h
    Max
};

static ItemName begin(ItemName) { return ItemName::Knife; }
static ItemName end(ItemName) { return ItemName::Max; }
static ItemName operator++(ItemName& state) { return (state = static_cast<ItemName>(std::underlying_type<ItemName>::type(state) + 1)); }
static ItemName operator*(const ItemName& state) { return state; }
