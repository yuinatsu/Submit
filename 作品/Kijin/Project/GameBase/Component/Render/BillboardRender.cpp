#include "BillboardRender.h"
#include "../../Common/ResourceMng.h"
#include "../../SceneManager.h"
#include "../../Object/ObjectManager.h"

void BillboardRender::Load(const std::filesystem::path& path)
{
	lpSceneMng.GetResourceMng().LoadTexture(handle_, path);
}

void BillboardRender::Draw(int shadowMap, int buff)
{
	if (transform_.IsActive())
	{
		// �r���{�[�h�`��
		const auto& pos{ transform_->GetPos() };
		DrawBillboard3D(VGet(pos.x, pos.y, pos.z), 0.5f, 0.5f,100.0f, 0.0f, *handle_, true);
	}
}

void BillboardRender::SetUpDepthTex(int ps, int buff)
{
}
