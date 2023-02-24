#pragma once
#include <utility>
#include <map>
#include <string>
#include <memory>
#include <vector>
#include "../common/Vector2.h"

using ImgMap = std::map<std::string, std::vector<int>>;

#define lpImageMng ImageMng::GetInstance()
#define IMAGE_ID(KEY) (ImageMng::GetInstance().GetID(KEY))

class ImageMng
{
public:
	static ImageMng& GetInstance(void)
	{
		static ImageMng s_Instance;
		return s_Instance;
	};
	const std::vector<int>& GetID(const std::string& key);										// �Q�[���������p
	const std::vector<int>& GetID(const std::string& key, const std::string& fileName);			// �ꖇ�G�p
	const std::vector<int>& GetID(const std::string& key, const std::string& fileName,			// �����p
		const Vector2& divSize, const Vector2& divCnt);

private:
	ImgMap imgMap_;

	ImageMng();
	~ImageMng();
};

