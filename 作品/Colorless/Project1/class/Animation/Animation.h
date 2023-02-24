#pragma once
#include <map>
#include <vector>
#include <string>
#include "../common/Vector2.h"

using AnimVector = std::vector<std::pair<int, int>>;

// �X�e�[�^�X�̎��
enum class State
{
	up,
	left,
	right,
	down,
	MAX
};

class Animation
{
public:
	Animation(std::string key);
	~Animation();
	bool Init(void);											// ������
	bool Update(void);											// �X�V
	bool Draw(Vector2F pos,Vector2 size,float mag = 1.0f);		// �`�� Draw(���W, �T�C�Y, �{��)
	bool state(const std::string state);						// �X�e�[�^�X state(�X�e�[�^�X��)
	std::string GetKey(void);									// �L�[���擾
	std::string GetState(void);									// �X�e�[�^�X�擾
private:
	std::string key_;
	std::string state_;
	int animFrame_;
	int animCount_;
};

