#pragma once
#include <vector>
#include <string>
#include <map>
#include "Math.h"

#define lpImageMng ImageMng::GetInstance()

using namespace Math;
using VecInt = std::vector<int>;

class ImageMng
{
public:
	static ImageMng& GetInstance()
	{
		static ImageMng s_Instance;
		return s_Instance;
	}

	const VecInt& GetID(std::string key);															// �摜�̕\��
	const VecInt& GetID(std::string f_name, std::string key);										// �摜�̕\��
	const VecInt& GetID(std::string f_name, std::string key, Vector2 divSize, Vector2 divCnt);		// �\������摜�̃T�C�Y���w�肵�ĕ\��
	const VecInt& GetID(std::string f_name, std::string key, Vector2I divSize, Vector2I divCnt);	// �\������摜�̃T�C�Y���w�肵�ĕ\��

	bool Relese(const std::string& key);
private:
	ImageMng();
	~ImageMng();
	std::map<std::string, VecInt> imageMap_;
};

