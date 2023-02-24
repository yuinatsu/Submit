#include <DxLib.h>
#include "ImageMng.h"
#include "../_debug/_DebugConOut.h"


const VecInt& ImageMng::GetID(std::string key)
{
	// �K�[�h����
	if (ImageMap_.find(key) == ImageMap_.end())					// key�̒l�����݂��Ȃ������璆�ɓ���
	{
		ImageMap_[key].resize(1);								// �v�f���P�̉摜�Ƃ��ēǂݍ���
		ImageMap_[key][0] = LoadGraph(key.c_str());				// �摜�̓ǂݍ���
	}
	return ImageMap_[key];										// �摜�����^�[��
}


const VecInt& ImageMng::GetID(std::string f_name, std::string key, Vector2 divSize, Vector2 divCnt)
{
	if (ImageMap_.find(key) == ImageMap_.end())																							// �摜���ǂݍ���ł��Ȃ������璆�ɓ���		(ImageMap_.end ... ImageMap.find�̌��ʍŏ�����Ō�܂łǂ��ɂ��Ȃ������ꍇ�^�ɂȂ�)			//ImageMap_.at(f_name);
	{
		//TRACE("�t�@�C���̓ǂݍ���\n");
		ImageMap_[key].resize(divCnt.x * divCnt.y);																						// �摜�̃t�@�C���l�[���ǂݍ���
		LoadDivGraph(f_name.c_str(), divCnt.x * divCnt.y, divCnt.x, divCnt.y, divSize.x, divSize.y, &ImageMap_[key][0], true);			// �摜�̓ǂݍ���	c_str ... char�^��string�^�ɕς���
	}
	return ImageMap_[key];																												// �摜�����^�[��
}


ImageMng::ImageMng()
{
}


ImageMng::~ImageMng()
{
}
