#include<dxlib.h>
#include<cmath>// sqrtのため
#include<vector>
#include<functional>
#include"Geometry.h"
using namespace std;
const int screen_width = 640;
const int screen_height = 480;

unsigned int canvas[screen_height][screen_width] = {};		// 独立メモリなのでマルチスレッドでエラーを起こさない

/// <summary>
/// 反射ベクトルを求める
/// </summary>
/// <param name="InVec">入射ベクトル</param>
/// <param name="N">法線ベクトル</param>
/// <returns>反射ベクトル</returns>
/// <returns>R=I-2(I・N)Nに基づいて計算する
/// ただし法線ベクトルは正規化済みとする
/// </returns>
Vector3 ReflectVector(const Vector3& InVec, const Vector3& N)
{
	return InVec - N * 2.0f * Dot(InVec, N);
}

/// <summary>
/// 指定範囲内に制約する
/// </summary>
/// <param name="value">範囲内に収めたい値</param>
/// <param name="minValue">最小値</param>
/// <param name="maxValue">最大値</param>
/// <returns>範囲内に収めた値</returns>
float Clamp(float value, float minValue = 0.0f, float maxValue = 1.0f)
{
	return max(min(value, maxValue), minValue);
}


/// <summary>
/// 色をRGBでピクセルを打つ
/// </sumarry>
/// <param name="x">画面のX座標</param>
/// <param name="y">画面のY座標</param>
/// <param name="r">赤(0.0~1.0)</param>
/// <param name="g">緑(0.0~1.0)</param>
/// <param name="b">青(0.0~1.0)</param>
void DrawPixelWithFloat(int x, int y, float r, float g, float b)
{
	DrawPixel(x, y, GetColor(static_cast<int>(255.0f * r), static_cast<int>(255.0f * g), static_cast<int>(255.0f * b)));
}

/// <summary>
/// 1本のRayをトレースする
/// </summary>
/// <param name="ray">現在のレイ（初回かもしれないし反射かもしれない）</param>
/// <param name="primitives">全プリミティブ</param>
/// <param name="startObject">呼び出し元オブジェクト</param>
/// <param name="limit">反射回数限界</param>
/// <returns></returns>
bool TraceRay(const Ray& ray, const vector<Primitive*>& primitives, const Primitive* startObject, int limit, Color& color, int imgHD)
{
	// ライト方向
	Vector3 lightVec = { 1.0f, -1.0f, -1.0f };
	lightVec.Normalize();
	if (limit == 0)
	{
		return false;
	}

	// デプスカラー
	std::pair<float, std::function<Color(void)>> depthAndColorFunc = 
	make_pair(numeric_limits<float>::max(),// MAX_FLOAT
		[]()->Color {
			return Color(0,0,0);
		}
	);

	bool isAnyHit = false;
	for (const auto object : primitives)
	{
		if (startObject == object)
		{
			continue;
		}
		float t = 0.0f;
		Vector3 N = Vector3();
		if (object->IsHitRay(ray, t, N))
		{
			auto func = [ray, t, N, &primitives, object, &lightVec, limit, imgHD]()->Color {
				Color color;
				auto hitpos = ray.start + ray.vec * t;
				const auto& material = object->material;
				// 自分の色
				Color selfColor = material.color1;
				switch (material.pattern)
				{
				case Pattern::stripe:
					break;
				case Pattern::checker:
					int r, g, b, a;
					GetPixelSoftImage(imgHD, (unsigned int)hitpos.x % 82, (unsigned int)hitpos.z % 82, &r, &g, &b, &a);
					selfColor = Color((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
					break;
				default:
					break;
				}

				if (material.ambientRate < 1.0f)
				{
					float ambient = material.ambientRate;					// これ以上暗くならない明るさ
					float diffuse = Clamp(Dot(-lightVec, N));				// float diffuse = min(max((400.0f - t) / 100.0f, 0.0f), 1.0f);
					selfColor = selfColor * max(diffuse, ambient);

					auto reflectVec = ReflectVector(lightVec, N);
					float power = material.specularity;
					float specular = pow(Clamp(Dot(-ray.vec, reflectVec)), power);
					specular += 0.9f * pow(1.0f - Clamp(Dot(-ray.vec, N)), 2.2);
					selfColor = Color(Clamp(selfColor.x + specular), Clamp(selfColor.y + specular), Clamp(selfColor.z + specular));
				}

				if (material.reflectionRate > 0.0f)
				{
					auto refVec = ReflectVector(ray.vec, N);
					Ray refRay = { ray.start + ray.vec * t, refVec };
					Color refCol;
					if (TraceRay(refRay, primitives, object, limit - 1, refCol, imgHD))
					{
						// 自分の色と反射色をα合成 src*(1-α)+dst*α
						color = selfColor * (1.0f - material.reflectionRate) + refCol * material.reflectionRate;
					}
					else
					{
						color = selfColor;
					}
				}
				else
				{
					// 影計算
					Color dummy;
					Ray shadowRay = { ray.start + ray.vec * t, -lightVec };
					if (TraceRay(shadowRay, primitives, object, 1, dummy, imgHD))
					{
						color = selfColor * 0.5f;
					}
					else
					{
						color = selfColor;
					}
				}
				return color;
			};
			if (t < depthAndColorFunc.first)
			{
				depthAndColorFunc = make_pair(t, func);
			}
			isAnyHit = true;
		}
	}
	if (isAnyHit)
	{
		color = depthAndColorFunc.second();
	}
	return isAnyHit;
}

///レイトレーシング
///@param eye 視点座標
///@param sphere 球オブジェクト(そのうち複数にする)
void RayTracing(const Position3& eye,const vector<Primitive*> primitives,int imgHD) 
{
	memset(canvas, 0, sizeof(unsigned int) * screen_height * screen_width);
	// ライトの座標
	Vector3 lightVec = { 1.0f, -1.0f, -1.0f };
	lightVec.Normalize();
#pragma omp parallel for 
	for (int y = 0; y < screen_height; ++y)//スクリーン縦方向
	{
#pragma omp parallel for 
		for (int x = 0; x < screen_width; ++x)//スクリーン横方向 
		{
			// 視点とスクリーン座標から視線ベクトルを作る
			Ray ray = { eye, {x - screen_width / 2 - eye.x, screen_height / 2 - y - eye.y, -eye.z} };
			ray.vec.Normalize();
			Color finalColor;
			// レイをトレースする
			if (TraceRay(ray, primitives, nullptr, 5, finalColor, imgHD))
			{
				finalColor *= 255;
				canvas[y][x] = GetColor(finalColor.x, finalColor.y, finalColor.z);
			}
			else
			{
				// 指定色で背景を塗りつぶす
				if (((x / 30) + (y / 30)) % 2 == 0)
				{
					canvas[y][x] = 0x77ff77;
				}
				else
				{
					canvas[y][x] = 0x444444;
				}
			}
		}
	}
	for (int y = 0; y < screen_height; ++y)
	{
		for (int x = 0; x < screen_width; ++x)
		{
			DrawPixel(x, y, canvas[y][x]);
		}
	}
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	ChangeWindowMode(true);
	SetGraphMode(screen_width, screen_height, 32);
	SetMainWindowText(_T("2016010_梶原尚人"));
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	int imgHD = LoadSoftImage(_T("image/image1.png"));

	// 球体
	Sphere sphere = Sphere(100, Position3(0, 0, -150), 
		Material(Color(1.0f, 0.25f, 0.5f), // 色①
			Color(), // 色②
			0.2f, // 環境光成分
			1.0f, // スペキュラ成分
			50.0f, // スペキュラ強さ
			0.5f, // 反射率
			Pattern::none));// 模様種別

	Sphere sphere2 = Sphere(100, Position3(250, 0, -150),
		Material(Color(0.5f, 1.0f, 0.25f), // 色①
			Color(), // 色②
			0.2f, // 環境光成分
			1.0f, // スペキュラ成分
			50.0f, // スペキュラ強さ
			0.5f, // 反射率
			Pattern::none));// 模様種別

	Sphere sphere3 = Sphere(100, Position3(-250, 0, -150),
		Material(Color(1.0f, 1.0f, 0.25f), // 色①
			Color(), // 色②
			0.2f, // 環境光成分
			1.0f, // スペキュラ成分
			50.0f, // スペキュラ強さ
			0.5f, // 反射率
			Pattern::none));// 模様種別

	// 床
	Plane plane = { Vector3(0,1,0), -100, 
		Material(Color(0.5f,1.0f,1.0f),  // 色①
			Color(0.125f,0.5f,0.5f), // 色②
			1.0f, // 環境光成分
			0.0f, // スペキュラ成分
			0.0f, // スペキュラ強さ
			0.0f, // 反射率
			Pattern::checker) };// 模様：チェック

	// 配列
	vector<Primitive*> primitives;
	primitives.push_back(&sphere);
	primitives.push_back(&sphere2);
	primitives.push_back(&sphere3);
	primitives.push_back(&plane);


	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();
		char keystate[256];
		GetHitKeyStateAll(keystate);
		if (keystate[KEY_INPUT_UP])
		{
			sphere.pos.y += 5;
		}
		if (keystate[KEY_INPUT_DOWN])
		{
			sphere.pos.y -= 5;
		}
		if (keystate[KEY_INPUT_RIGHT])
		{
			sphere.pos.x += 5;
		}
		if (keystate[KEY_INPUT_LEFT])
		{
			sphere.pos.x -= 5;
		}
		if (keystate[KEY_INPUT_X])
		{
			sphere.pos.z += 5;
		}
		if (keystate[KEY_INPUT_Z])
		{
			sphere.pos.z -= 5;
		}

		RayTracing(Position3(0, 0, 300), primitives, imgHD);

		auto fps = GetFPS();
		DrawFormatString(10, 10, 0xffffff, L"fps = %f", fps);
		ScreenFlip();
	}
	DxLib_End();
}