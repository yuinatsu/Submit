#pragma once
#include <list>
#include <map>
#include <string>
#include <utility>
#include "../Object/characterID.h"
#include "../Animation/Anim_ID.h"
#include "../common/Vector2.h"

struct AnimData
{
	// �\������
	std::list<int> flameData;
	// �Y����
	std::list<int> subscript;
};

struct AnimInfo
{
	int width = 0;
	int height = 0;
	int widthCnt = 0;
	int heightCnt = 0;
	int loop = 0;
	int widthOffset = 0;
	int heigthOffset = 0;
	std::string image = "";
	std::string imgKey = "";
};

#define lpAnimMng AnimationMng::GetInstance()
using Animation = std::pair<AnimInfo,AnimData>;

class AnimationMng
{
public:
	static AnimationMng& GetInstance()
	{
		static AnimationMng s_instance_;
		return s_instance_;
	}
	/// <summary>
	/// �L�����N�^�[�ɑΉ������A�j���[�V�����f�[�^�̓ǂݍ��݂Ɠo�^
	/// </summary>
	/// <param name="filepath">tmx�ւ̃p�X</param>
	/// <param name="id">�o�^����L������ID</param>
	/// <returns>true:���� false:���s</returns>
	bool LoadAnimTmx(const std::string& filepath, Object_ID id);
	/// <summary>
	/// �����Ŏw�肳�ꂽ�L�����̃A�j���[�V�����Ɋւ���f�[�^
	/// �w�肵���L�����A�������͎w�肵���A�j���[�V���������o�^�̏ꍇ���g�̂Ȃ�Animation���Ԃ��Ă���
	/// </summary>
	/// <param name="cID">�l���������L������ID</param>
	/// <param name="aID">�l���������A�j���[�V������ID</param>
	/// <returns>�A�j���[�V�����֘A�f�[�^</returns>
	const Animation GetAnimationData(Object_ID cID, Char_Anim_ID aID);
	/// <summary>
	/// �����Ŏw�肳�ꂽ�L�����̃A�j���[�V������Image�Ɋւ���f�[�^
	/// �w�肵���L�����A�������͎w�肵���A�j���[�V���������o�^�̏ꍇ���g�̂Ȃ�AnimInfo���Ԃ��Ă���
	/// </summary>
	/// <param name="cID">�l���������L������ID</param>
	/// <param name="aID">�l���������A�j���[�V������ID</param>
	/// <returns>�A�j���[�V�����̉摜�֘A�f�[�^</returns>
	const AnimInfo GetAnimInfo(Object_ID cID, Char_Anim_ID aID);
	/// <summary>
	/// �����Ŏw�肳�ꂽID��Info�����݂��邩�ǂ���
	/// </summary>
	/// <param name="cID">�L������ID</param>
	/// <param name="aID">�A�j���[�V������ID</param>
	/// <returns>true:���݂��� false:���݂��Ȃ�</returns>
	bool CheckAnimInfo(Object_ID cID, Char_Anim_ID aID);
	/// <summary>
	/// �����̏������ƂɌ��ݕ\������ׂ��摜��Ԃ�
	/// �L����ID��A�j���[�V����ID�ɑΉ������f�[�^�o�^����Ă��Ȃ��ꍇ-1���Ԃ��Ă���
	/// </summary>
	/// <param name="cID">�L����ID</param>
	/// <param name="aID">�A�j���[�V����ID</param>
	/// <param name="elapsed">�o�߃t���[���� ���[�v����ꍇ���Z�b�g�����</param>
	/// <param name="loopNum">���݃A�j���[�V�����J��Ԃ���</param>
	/// <returns>�f�[�^������ꍇ�\�����ׂ��摜 �Ȃ��ꍇ-1</returns>
	const int GetAnimImag(Object_ID cID, Char_Anim_ID aID, int& elapsed, int& loopNum);
	/// <summary>
	/// �����̏������Ƃ�loop���I����Ă��邩���擾����
	/// �o�^����Ă��Ȃ��L����ID��A�j���[�V����ID�������ꍇ�Đ��͏I����Ă�����̂Ƃ���
	/// </summary>
	/// <param name="cID">�L����ID</param>
	/// <param name="aID">�A�j���[�V����ID</param>
	/// <param name="elapsed">�o�߃t���[����</param>
	/// <param name="loopNum">���݌J��Ԃ���</param>
	/// <returns>true:�I����Ă��閔�̓f�[�^�̑��݂��Ȃ�ID false:���[�v��</returns>
	const bool CheckAnimLoopEnd(Object_ID cID, Char_Anim_ID aID,const int& elapsed,const int& loopNum);
	/// <summary>
	/// �`�悷��ۂ�offSet�l�� �f�[�^�����݂��Ȃ��ꍇ{0,0}�ŕԂ��Ă���
	/// </summary>
	/// <param name="cID">�l���������L������ID</param>
	/// <param name="aID">�l���������A�j���[�V������ID</param>
	/// <returns>�`�悷��ۂ�offset</returns>
	const Vector2 GetDrawOffSet(Object_ID cID, Char_Anim_ID aID);
	/// <summary>
	/// �w�肳�ꂽID�̎��̕�����摜�T�C�Y���l������
	/// �o�^����Ă��Ȃ��f�[�^�̏ꍇ{0,0}�ŕԂ��Ă���
	/// </summary>
	/// <param name="cID">�l���������L������ID</param>
	/// <param name="aID">�l���������A�j���[�V������ID </param>
	/// <returns>������摜�T�C�Y</returns>
	Vector2 GetDivImageSize(Object_ID cID, Char_Anim_ID aID);
private:
	AnimationMng();
	~AnimationMng() = default;
	// �L�����N�^�[�̎�ʂƃA�j���[�V������ʂ��L�[�ɂ����A�j���[�V�����֘A�f�[�^
	std::map<Object_ID, std::map<Char_Anim_ID,Animation>> animData_;
};
