#pragma once
#include <map>
#include <vector>
#include <string>
#include "Vector2.h"

#define lpImageMng ImageMng::GetInstance()

using VecInt = std::vector<int>;					// int�^�̔z��

class ImageMng
{
public:
	static ImageMng& GetInstance()
	{
		static ImageMng s_Instance;				// s_Instance	�V���O���g���C���X�^���X
		return s_Instance;
	}
	const VecInt& GetID(std::string key);																	// GetID(�L�[�l)			���݂��Ȃ��L�[�l�������ꍇ�Ɏg�p
	const VecInt& GetID(std::string f_name, std::string key, Vector2 divSize, Vector2 divCnt);				// GetID(�t�@�C���l�[��, �L�[�l, ��������, ������)		����͂킩��₷���d����string���g�p
private:
	ImageMng();
	~ImageMng();
	std::map<std::string, VecInt> ImageMap_;						// 
};

