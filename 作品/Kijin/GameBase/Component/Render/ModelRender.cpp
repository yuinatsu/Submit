#include "ModelRender.h"
#include <DxLib.h>
#include "../../Scene/BaseScene.h"
#include "../../Object/ObjectManager.h"
#include "../../Common/ResourceMng.h"
#include "../../Application.h"
#include <unordered_map>

#include "../../Common/Debug.h"

// 頂点タイプに合わせたシェーダパスのテーブル
const std::unordered_map<int, std::filesystem::path> vsShaderPathTbl{
	{DX_MV1_VERTEX_TYPE_1FRAME, "Resource/resource/Shader/Vertex/Mesh.vso"},
	{DX_MV1_VERTEX_TYPE_4FRAME,"Resource/resource/Shader/Vertex/Mesh4.vso"}
};


ModelRender::ModelRender()
{
}

ModelRender::~ModelRender()
{
}

void ModelRender::Draw(void)
{
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);
	
	// シェーダをセットして描画
	MV1SetUseOrigShader(true);
	SetUseVertexShader(*vs_);
	SetUsePixelShader(*ps_);
	MV1DrawModel(*handle_);
	MV1SetUseOrigShader(false); 
}

void ModelRender::Draw(int shadowMap, int buff)
{
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// シェーダをセットして描画
	MV1SetUseOrigShader(true);
	SetUseVertexShader(*vs_);
	SetUsePixelShader(*ps_);
	UpdateShaderConstantBuffer(buff);
	SetShaderConstantBuffer(buff, DX_SHADERTYPE_VERTEX, 4);
	// シャドウマップをテクスチャとしてセット
	SetUseTextureToShader(1, shadowMap);
	// ここでセットしたシャドウマップのフィルタリングとアドレスUVの設定を行う
	//MV1SetTextureSampleFilterMode(*handle_, 1, DX_DRAWMODE_ANISOTROPIC);
	//MV1SetTextureAddressMode(*handle_, 1, DX_TEXADDRESS_MIRROR, DX_TEXADDRESS_MIRROR);
	
	MV1DrawModel(*handle_);
	MV1SetUseOrigShader(false);

	// テストでカメラ範囲チェック用boxを表示する
	//DebugDrawCube(boxPos1_, boxPos2_, 0xffffff);
}

void ModelRender::SetUpShadowMap(void)
{
	if (ps == 0 && vs == 0)
	{
		LoadVertex();
	}
	MV1SetUseOrigShader(true);
	SetUsePixelShader(ps);
	SetUseVertexShader(vs);
	MV1DrawModel(*handle_);
	MV1SetUseOrigShader(false);
}

std::string ModelRender::Load(std::ifstream& file)
{
	int num{0};
	std::string str;
	file.read(reinterpret_cast<char*>(&num), sizeof(num));
	str.resize(num);
	file.read(str.data(), sizeof(str[0]) * num);
	return str;
}

void ModelRender::Load(const std::filesystem::path& path)
{
	lpResourceMng.LoadModel(handle_, path);
}

void ModelRender::LoadVertex(void)
{
	auto tlNum = MV1GetTriangleListNum(*handle_);
	tlbertType_ = -1;
	for (int i = 0; i < tlNum; ++i)
	{
		tlbertType_ = MV1GetTriangleListVertexType(*handle_, i);
		break;
	}
	switch (tlbertType_)
	{
	case DX_MV1_VERTEX_TYPE_1FRAME:
		vs = LoadVertexShader(L"Resource/resource/Shader/ShadowMap/Mesh.vso");
		break;
	case DX_MV1_VERTEX_TYPE_4FRAME:
		vs = LoadVertexShader(L"Resource/resource/Shader/ShadowMap/Mesh4.vso");
		break;
	case DX_MV1_VERTEX_TYPE_8FRAME:
		break;
	case DX_MV1_VERTEX_TYPE_NMAP_1FRAME:
		break;
	case DX_MV1_VERTEX_TYPE_NMAP_4FRAME:
		break;
	case DX_MV1_VERTEX_TYPE_NMAP_8FRAME:
		break;
	default:
		break;
	}
	//if (tlbertType_ == DX_MV1_VERTEX_TYPE_1FRAME) {
	//  vs = LoadVertexShader(L"Resource/resource/Shader/ShadowMap/Mesh.vso");
	//}
	//else if (tlbertType_ == DX_MV1_VERTEX_TYPE_4FRAME) {
	//	vs = LoadVertexShader(L"Resource/resource/Shader/ShadowMap/Mesh4.vso");
	//}
	//else if (tlbertType_ == DX_MV1_VERTEX_TYPE_8FRAME) {
	//}
	//else if (tlbertType_ == DX_MV1_VERTEX_TYPE_NMAP_1FRAME) {
	//}
	//else if (tlbertType_ == DX_MV1_VERTEX_TYPE_NMAP_4FRAME) {
	//}
	//else if (tlbertType_ == DX_MV1_VERTEX_TYPE_NMAP_8FRAME) {
	//}
	ps = LoadPixelShader(L"Resource/resource/Shader/ShadowMap/ShadowMap.pso");
}

void ModelRender::Update(BaseScene& scene,ObjectManager& objectManager, float delt, Controller& controller)
{
	if (transform_.IsActive())
	{
		const auto& pos = transform_.Get()->GetPos();
		const auto& scale = transform_.Get()->GetScale();
		MV1SetPosition(*handle_, VGet(pos.x, pos.y, pos.z));
		MV1SetScale(*handle_, VGet(scale.x, scale.y, scale.z));
		MV1SetRotationMatrix(*handle_, transform_->GetRotationMatrix());
		// 下のはテスト後で消す
		int meshNum = MV1GetMeshNum(*handle_);
		for (int i = 0; i < meshNum; i++)
		{
			MV1SetMeshBackCulling(*handle_, i, DX_CULLING_NONE);
		}
	}
}

void ModelRender::Begin(ObjectManager& objectManager)
{
	// ロード終了orオブジェクトが場に出る時にここにくる

	Render::Begin(objectManager);
	auto type = MV1GetTriangleListVertexType(*handle_, 0);
	if (vsShaderPathTbl.contains(type))
	{
		lpResourceMng.LoadVS(vs_, vsShaderPathTbl.at(type));
	}
	else
	{
		DebugLog("このタイプのシェーダは無いです");
	}

	if (type < 3)
	{
		lpResourceMng.LoadPS(ps_, "Resource/resource/Shader/Pixel/Tex.pso");
	}

	ps = 0;
	vs = 0;
}
