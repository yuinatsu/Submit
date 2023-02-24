#pragma once
/// <summary>
/// �V���O���g���N���X
/// </summary>
class Singleton
{
public:
	/// <summary>
	/// public�Ɏ����̎Q�Ƃ�Ԃ��֐������
	/// </summary>
	/// <returns></returns>
	static Singleton& Instance() {
		// ���[�J��static�ϐ������
		static Singleton instance;
		return instance;
	}
	void OutputLife();
private:
	int life_;
	Singleton() {};// �v���C�x�[�g�ɃR���X�g���N�^
	// �R�s�[�E����֎~
	Singleton(const Singleton&) = delete;
	void operator=(const Singleton&) = delete;
};

