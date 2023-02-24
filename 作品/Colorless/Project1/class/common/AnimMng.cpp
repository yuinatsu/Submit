#include "AnimMng.h"
#include "ImageMng.h"

bool AnimMng::CheckAnim(std::string key, std::string state)
{
	if (animMap_[key].find(state) != animMap_[key].end()) 
	{
		return true;
	}
	return false;
}

bool AnimMng::SetXml(std::string key, std::string f_name)
{
	XmlItem tmpXml = tmx_.LoadXmlItem(f_name);
	for (auto tmp : tmpXml.data_)
	{
		xmlItem_[key].data_.try_emplace(tmp.first, tmp.second);
	}
	for (auto tmp : tmpXml.item_)
	{
		xmlItem_[key].item_.try_emplace(tmp.first, tmp.second);
	}
	for (auto tmp : tmpXml.loop_) 
	{
		xmlItem_[key].loop_.try_emplace(tmp.first, tmp.second);
	}

	return true;
}

bool AnimMng::SetItem(std::string key, const std::string state)
{
	AnimVector data;
	int frame = 0;
	for (auto& item : xmlItem_[key].data_[state]) 
	{
		frame += item.second;
		data.emplace_back(lpImageMng.GetID(xmlItem_[key].item_["name"])[item.first], frame);
	}
	if (xmlItem_[key].loop_[state] == -1)
	{
		data.emplace_back(-1, -1);
	}
	SetAnim(key, state, data);

	return true;
}

bool AnimMng::SetAnim(std::string key, const std::string state, AnimVector& data)
{
	return animMap_[key].try_emplace(state, std::move(data)).second;
}

int AnimMng::GetAnimID(std::string key, std::string state, int animframe)
{
	return animMap_[key][state][animframe].first;
}

int AnimMng::GetAnimFrame(std::string key, std::string state, int animframe)
{
	return animMap_[key][state][animframe].second;
}

int AnimMng::GetAnimSize(std::string key, std::string state)
{
	return static_cast<int>(animMap_[key][state].size());
}

Vector2 AnimMng::GetImageSize(std::string key)
{
	return Vector2(std::atoi(xmlItem_[key].item_["width"].c_str()), std::atoi(xmlItem_[key].item_["height"].c_str()));
}