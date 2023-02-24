#include <DxLib.h>
#include "ShaderMng.h"

bool ShaderMng::LoadpixelShader(const std::string& fname, const ShaderID id)
{
	bool rtn{ true };
	if (!shaderMap_.contains(id))
	{
		int h{ LoadPixelShader(fname.c_str()) };
		auto result = shaderMap_.try_emplace(id, h);
		rtn = (h != -1) && result.second;
	}
	return rtn;
}

const int ShaderMng::Get(const ShaderID id) const
{
	if (!shaderMap_.contains(id))
	{
		return -1;
	}
	return shaderMap_.at(id);
}

void ShaderMng::Relese(const ShaderID id)
{
	if (shaderMap_.contains(id))
	{
		DeleteShader(shaderMap_[id]);
		shaderMap_.erase(id);
	}
}

ShaderMng::ShaderMng()
{
}

ShaderMng::~ShaderMng()
{
	for (auto& s : shaderMap_)
	{
		DeleteShader(s.second);
	}
}
