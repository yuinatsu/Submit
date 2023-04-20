#pragma once
#include <map>
#include <string>
#include "Vector2.h"
#include "../../Tiled/TmxObj.h"
#include "../Animation/Animation.h"

#define lpAnimMng AnimMng::GetInstance()

class AnimMng
{
public:
	static AnimMng& GetInstance() 
	{
		static AnimMng s_Instance;
		return s_Instance;
	}
	// Anim��Item����ݒ肷��(XmlItem)
	bool CheckAnim(std::string key, std::string state);							// �A�j���[�V���������邩�ǂ���
	bool SetXml(std::string key, std::string f_name);							// xml�f�[�^�Z�b�g
	bool SetItem(std::string key, const std::string state);						// �A�C�e���Z�b�g
	bool SetAnim(std::string key, const std::string state, AnimVector& data);	// �A�j���[�V�����Z�b�g

	int GetAnimID(std::string key, std::string state, int animframe);			// �A�j���[�V����ID�擾
	int GetAnimFrame(std::string key, std::string state, int animframe);		// �A�j���[�V�����t���[���擾
	int GetAnimSize(std::string key, std::string state);						// �A�j���[�V�������擾

	Vector2 GetImageSize(std::string key);										// �C���[�W�T�C�Y�擾
private:
	std::map<std::string, std::map<std::string, AnimVector>> animMap_;			// �A�j���[�V����
	TmxObj tmx_;																// tmx�f�[�^
	std::map<std::string, XmlItem> xmlItem_;									// xml�f�[�^
};

