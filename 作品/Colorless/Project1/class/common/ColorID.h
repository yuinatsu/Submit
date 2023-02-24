#pragma once

#define lpColorMng ColorID::GetInstance()

// �F�̎��
enum class Color
{
	black,				// ��
	door,				// �h�A
	// ���A�C�e���F
	lightBlueItem,		// ���F
	purpleItem,			// ��
	yellowItem,			// ���F
	greenItem,			// ��
	redItem,			// ��
	orangeItem,			// �I�����W
	blueItem,			// ��
	ItemMax,
	// ���F�ς��\�F
	lightBlue,			// ���F
	purple,				// ��
	yellow,				// ���F
	green,				// ��
	red,				// ��
	orange,				// �I�����W
	blue,				// ��
	max,
};

class ColorID
{
public:
	static ColorID& GetInstance()
	{
		static ColorID s_Instance;
		return s_Instance;
	}
	void Init(void);								// ������
	bool GetColor(Color color);						// �F�̗L���̎擾 GetColor(�F�̎��)
	void SetColor(Color color, bool flag);			// �F�̗L���̃Z�b�g�@SetColor(�F�̎��, �t���O)
	Color GetNowColor();							// ���݂̔w�i�F�̎擾
	void SetNowColor(Color color);					// ���݂̔w�i�F�̃Z�b�g
	bool GetItemFlag(Color color);
	void SetItemFlag(Color color, bool flag);
private:
	ColorID();
	// �F�i�[�p�ϐ�
	bool black_;
	bool lightBlue_;
	bool purple_;
	bool yellow_;
	bool green_;
	bool red_;
	bool orange_;
	bool blue_;
	bool door_;
	// ���݂̔w�i�F
	Color color_;
	// �A�C�e���֌W
	bool itemFlag_[static_cast<int>(Color::ItemMax) - static_cast<int>(Color::lightBlueItem)];
};

