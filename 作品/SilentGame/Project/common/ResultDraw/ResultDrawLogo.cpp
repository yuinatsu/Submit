#include <DxLib.h>
#include "../ImageMng.h"
#include "../ShaderMng.h"
#include "ResultDrawLogo.h"
#include "../ConstantBuffer.h"
#include "../../Shader/ShaderDrawGraph.h"

ResultDrawLogo::ResultDrawLogo(const std::string& imgName, float startTime, const Math::Vector2& pos, float delta, bool IsClear) :
	ResultDraw{startTime,pos}, imgName_{imgName}
{
	t = 0.0f;
	delta_ = delta;
	IsClear_ = IsClear;
	cb_ = std::make_unique<ConstantBuffer<float>>();
	if (IsClear_)
	{
		psID_ = ShaderID::GameClear;
		lpShaderMng.LoadpixelShader("Shader/GameClear.pso", psID_);
	}
	else
	{
		psID_ = ShaderID::GameOver;
		lpShaderMng.LoadpixelShader("Shader/GameOver.pso", psID_);
	}
	
	lpImageMng.GetID(imgName_);
}

ResultDrawLogo::~ResultDrawLogo()
{
}

void ResultDrawLogo::DrawOwn(void)
{
	if (IsClear_)
	{
		t += delta_ / 3;
		*cb_ = t;
		SetUsePixelShader(lpShaderMng.Get(psID_));
		cb_->Update();
		SetShaderConstantBuffer(cb_->GetHandle(), DX_SHADERTYPE_PIXEL, 0);
		SetUseTextureToShader(1, lpImageMng.GetID("Resource/image/ResultScene/ptn.png")[0]);
		MyDrawGraph(300, 60, lpImageMng.GetID(imgName_)[0]);
	}
	if (!IsClear_)
	{
		t += delta_ / 5;
		*cb_ = t;
		SetUsePixelShader(lpShaderMng.Get(psID_));
		cb_->Update();
		SetShaderConstantBuffer(cb_->GetHandle(), DX_SHADERTYPE_PIXEL, 0);
		SetUseTextureToShader(1, lpImageMng.GetID("Resource/image/ResultScene/ptn.png")[0]);
		MyDrawGraph(300, 60, lpImageMng.GetID(imgName_)[0]);
	}
}
