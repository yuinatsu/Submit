#include "AnimMng.h"
#include "ImageMng.h"
#include "Debug.h"



bool AnimMng::Load(const std::string& fname, const std::string& key)
{
	
	rapidxml::file<> file = fname.c_str();
	xmldoc_.parse<0>(file.data());

	for (auto tmpNode = xmldoc_.first_node("image"); tmpNode != nullptr; tmpNode = tmpNode->next_sibling("image"))
	{
		// �c���̃T�C�Y���擾
		Math::Vector2 divSize{ std::stof(tmpNode->first_attribute("width")->value()),std::stof(tmpNode->first_attribute("height")->value()) };

		// ���������擾
		Math::Vector2 divCnt{ std::stof(tmpNode->first_attribute("divwidth")->value()),std::stof(tmpNode->first_attribute("divheight")->value()) };

		// �摜�̃t�@�C�������擾
		std::string sourceName{ tmpNode->first_attribute("source")->value() };

		// �摜��ǂݍ���
		lpImageMng.GetID(sourceName, key, divSize, divCnt);

		// �A�j���[�V�����f�[�^��ǂݍ���
		for (auto node = tmpNode->first_node("animation"); node != nullptr; node = node->next_sibling("animation"))
		{
			// ���[�v����ǂݍ���
			int loop = std::stoi(node->first_attribute("loop")->value());

			// �X�e�[�g����ǂݍ���
			std::string stateName{ node->first_attribute("name")->value() };

			std::list<std::pair<int, float>> animData;
			for (auto dataNode = node->first_node("data"); dataNode != nullptr; dataNode = dataNode->next_sibling("data"))
			{
				// �\���ʒu��\�����Ԃ�ǂݍ��ݒǉ�
				animData.emplace_back(
					std::make_pair(
						std::stoi(dataNode->first_attribute("count")->value()),
						std::stof(dataNode->first_attribute("time")->value())
					));
			}

			// �L�[�ɑ΂��ă��[�v���ƃf�[�^������
			dataMap_[key].emplace(stateName, AnimationData{ loop,animData });
		}
	}

	xmldoc_.clear();
	return true;
}

const AnimationData& AnimMng::GetDate(const std::string& key, const std::string& state) const
{
	return dataMap_.at(key).at(state);
}


