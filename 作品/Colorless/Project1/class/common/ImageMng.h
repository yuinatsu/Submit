#pragma once
#include <vector>
#include <string>
#include <map>
#include "Vector2.h"

#define lpImageMng ImageMng::GetInstance()

class ImageMng
{
public:
	static ImageMng& GetInstance() 
	{
		static ImageMng s_Instance;
		return s_Instance;
	}
	const VecInt& GetID(std::string key);													// ID�擾(�L�[���̂�)
	const VecInt& GetID(std::string f_name,std::string key);								// ID�擾(�n���h���l�[���A�L�[���)
	const VecInt& GetID(std::string f_name,std::string key,Vector2 divSize,Vector2 divCnt);	// ID�擾(�n���h���l�[���A�L�[���A���������ۂ̃T�C�Y�A������)

private:
	ImageMng();
	~ImageMng();
	std::map<std::string, VecInt> imageMap_;				// �摜
};

