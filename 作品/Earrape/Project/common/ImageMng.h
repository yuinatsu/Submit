#pragma once
#include <map>		// �A�z�z��
#include <vector>	// �������̘A�����A��A����
#include <string>
#include "Vector2.h"

#define lpImageMng ImageMng::GetInstance()
using VecInt = std::vector<int>;

class ImageMng
{
public:
	static ImageMng& GetInstance(void)
	{
		static ImageMng s_Instance;
		return s_Instance;
	}
	const VecInt& GetID(std::string key);	// �\���p
	const VecInt& GetID(std::string f_name, std::string key, Vector2 divSize, Vector2 divCnt);	// ���[�h�p

private:
	ImageMng();
	~ImageMng();

	std::map<std::string, VecInt> imgMap_;			// �C���[�W���
};
