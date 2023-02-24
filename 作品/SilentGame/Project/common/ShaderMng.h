#pragma once
#include <string>
#include <map>
#include "ShaderID.h"

#define lpShaderMng (ShaderMng::GetInstance())

class ShaderMng
{
public:

	

	bool LoadpixelShader(const std::string& fname, const ShaderID id);
	const int Get(const ShaderID id) const;
	void Relese(const ShaderID id);
	static ShaderMng& GetInstance(void)
	{
		static ShaderMng instance;
		return instance;
	}
private:

	// コンストラクタ
	ShaderMng();

	// デストラクタ
	~ShaderMng();

	ShaderMng(const ShaderMng&) = delete;
	ShaderMng& operator=(const ShaderMng&) = delete;
	using ShaderMap = std::map<ShaderID, int>;
	ShaderMap shaderMap_;
};

