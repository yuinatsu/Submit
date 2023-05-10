#include <Dxlib.h>
#include <array>

using namespace std;

void MyDrawGraph(int x, int y, int imgHandle, int secondImg, int thirdImg, int psHandle) {
	int width, height;
	GetGraphSize(imgHandle, &width, &height);
	array<VERTEX2DSHADER, 4> verts;
	for (auto & v : verts) {
		v.rhw = 1.0f;
		v.dif = GetColorU8(0xff, 0xff, 0xff, 0xff);
		v.spc = GetColorU8(0xff, 0xff, 0xff, 0xff);
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
	}
	// 左上
	verts[0].pos.x = x;
	verts[0].pos.y = y;
	verts[0].u = 0.0f;
	verts[0].v = 0.0f;
	// 右上
	verts[1].pos.x = x + width;
	verts[1].pos.y = y;
	verts[1].u = 1.0f;
	verts[1].v = 0.0f;
	// 左下
	verts[2].pos.x = x;
	verts[2].pos.y = y + height;
	verts[2].u = 0.0f;
	verts[2].v = 1.0f;
	// 右下
	verts[3].pos.x = x + width;
	verts[3].pos.y = y + height;
	verts[3].u = 1.0f;
	verts[3].v = 1.0f;

	// テクスチャのα値を抜く
	int alphaMode, alphaParam;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	GetDrawAlphaTest(&alphaMode, &alphaParam);
	SetDrawAlphaTest(DX_CMP_GREATER, 0);
	SetUseAlphaTestFlag(true);
	SetUsePixelShader(psHandle);
	SetUseTextureToShader(0, imgHandle);		// tex
	SetUseTextureToShader(1, secondImg);		// ptn
	SetUseTextureToShader(2, thirdImg);		// norm
	DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);
}


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	ChangeWindowMode(true);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	int img = LoadGraph(L"img.png");
	int ptn = LoadGraph(L"pattern.png");
	int normImg = LoadGraph(L"normImg.png");
	int bg = LoadGraph(L"bgImg.png");
	int ps = LoadPixelShader(L"PixelShader.pso");
	int psPP = LoadPixelShader(L"PostProcess.pso");
	//int imgPP = LoadGraph(L"normalmap.png");// 水面揺れ
	int imgPP = LoadGraph(L"cracknormalmap.png");// ひび割れ
	//int imgPP = LoadGraph(L"bulletnormalmap.png");// 弾痕
	// １．定数Bufferの確保(VRAM上)
	int cbuff = DxLib::CreateShaderConstantBuffer(sizeof(float) * 4);
	// グラボのメモリは実は直接いじれない
	// RAM上のメモリでシミュレーションしてる
	float* threthold = static_cast<float*>(DxLib::GetBufferShaderConstantBuffer(cbuff));
	float tmp = 0.0f;
	float angle = 0.0f;
	char keystate[256];
	int sw, sh;
	GetDrawScreenSize(&sw, &sh);
	int handleForPP = DxLib::MakeScreen(sw, sh);
	int shakeTime = 0;
	while (ProcessMessage() == 0)
	{
		SetDrawScreen(handleForPP);
		angle += 0.01f;
		GetHitKeyStateAll(keystate);
		if (keystate[KEY_INPUT_UP] && tmp > 0.1)
		{
			tmp -= 0.01f;
		}
		if (keystate[KEY_INPUT_DOWN] && tmp < 1.2)
		{
			tmp += 0.01f;
		}
		if (keystate[KEY_INPUT_SPACE])
		{
			shakeTime = 60;
		}
		if (shakeTime > 0)
		{
			--shakeTime;
		}
		ClsDrawScreen();
		DrawGraph(-50, -50, bg, ps);
		threthold[0] = tmp;
		threthold[1] = angle;
		UpdateShaderConstantBuffer(cbuff);
		SetShaderConstantBuffer(cbuff, DX_SHADERTYPE_PIXEL, 0);
		MyDrawGraph(90, 30, img, ptn, normImg, ps);

		SetDrawScreen(DX_SCREEN_BACK);

		MyDrawGraph(0, 0, handleForPP, imgPP, -1, psPP);		// ポストエフェクト

		//DrawGraph(0, 0, handleForPP, false);														// 無反応
		//GraphFilter(handleForPP, DX_GRAPH_FILTER_GAUSS, 32, 1000);								// 画面フィルター（ぼかし）
		//GraphFilter(handleForPP, DX_GRAPH_FILTER_MONO, -60, 7);									// 画面フィルター（モノクロ）
		//DrawRotaGraph(320, 240, 0.75f, DX_PI / 4.0f, handleForPP, false);							// 斜め描写
		//DrawRotaGraph(320 + sin(angle * 20) * 4, 240, 1.0f, 0.0f, handleForPP, false);			// 揺れ（非推奨）
		//DrawRotaGraph(320 + (shakeTime % 4 - 2) * 2, 240, 1.0f, 0.0f, handleForPP, false);		// 揺れ

		ScreenFlip();
	}
	DxLib_End();
}

