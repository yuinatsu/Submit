#pragma once
class ResultData
{
public:
	ResultData();
	~ResultData();

	/// <summary> �|�����������������Z���� </summary>
	/// <param name="val"> ���Z����|������ </param>
	void AddKillCount(int val);

	/// <summary> ������������+1���� </summary>
	/// <param name=""></param>
	void CountUpAlertCount(void);

	/// <summary> �N���A�܂ł̎��Ԃ��X�V���� </summary>
	/// <param name="delta"></param>
	void UpdateClearTime(float delta);

	/// <summary> �|���������擾 </summary>
	/// <param name=""></param>
	/// <returns> �|������ </returns>
	const int GetKillCount(void)const { return killCount_; }

	/// <summary> ���U���g�܂ł̌o�ߎ��Ԃ��擾 </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const float GetClearTime(void) const { return clearTime_; }

	/// <summary> �������������擾 </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const int GetAlertCount(void) const { return alertCount_; }

	/// <summary> �N���A���ǂ������Z�b�g���� </summary>
	/// <param name="flag"></param>
	void SetClear(bool flag);

	const bool IsClear(void) const { return isClear_; }

	void CountUpItemUseCount(void);

	const int GetItemUseCount(void) const { return itemUseCount_; }
private:

	// �N���A���Q�[���I�[�o�[��
	bool isClear_;

	// �|������
	int killCount_;

	// ���U���g�܂ł̎���
	float clearTime_;

	// �A���[�g��ԂɂȂ�����
	int alertCount_;

	// �A�C�e���g�p��
	int itemUseCount_;
};

