#include<dxlib.h>
#include<cmath>// sqrt�̂���
#include<vector>
#include<functional>
#include"Geometry.h"
using namespace std;
const int screen_width = 640;
const int screen_height = 480;

unsigned int canvas[screen_height][screen_width] = {};		// �Ɨ��������Ȃ̂Ń}���`�X���b�h�ŃG���[���N�����Ȃ�

/// <summary>
/// ���˃x�N�g�������߂�
/// </summary>
/// <param name="InVec">���˃x�N�g��</param>
/// <param name="N">�@���x�N�g��</param>
/// <returns>���˃x�N�g��</returns>
/// <returns>R=I-2(I�EN)N�Ɋ�Â��Čv�Z����
/// �������@���x�N�g���͐��K���ς݂Ƃ���
/// </returns>
Vector3 ReflectVector(const Vector3& InVec, const Vector3& N)
{
	return InVec - N * 2.0f * Dot(InVec, N);
}

/// <summary>
/// �w��͈͓��ɐ��񂷂�
/// </summary>
/// <param name="value">�͈͓��Ɏ��߂����l</param>
/// <param name="minValue">�ŏ��l</param>
/// <param name="maxValue">�ő�l</param>
/// <returns>�͈͓��Ɏ��߂��l</returns>
float Clamp(float value, float minValue = 0.0f, float maxValue = 1.0f)
{
	return max(min(value, maxValue), minValue);
}


/// <summary>
/// �F��RGB�Ńs�N�Z����ł�
/// </sumarry>
/// <param name="x">��ʂ�X���W</param>
/// <param name="y">��ʂ�Y���W</param>
/// <param name="r">��(0.0~1.0)</param>
/// <param name="g">��(0.0~1.0)</param>
/// <param name="b">��(0.0~1.0)</param>
void DrawPixelWithFloat(int x, int y, float r, float g, float b)
{
	DrawPixel(x, y, GetColor(static_cast<int>(255.0f * r), static_cast<int>(255.0f * g), static_cast<int>(255.0f * b)));
}

/// <summary>
/// 1�{��Ray���g���[�X����
/// </summary>
/// <param name="ray">���݂̃��C�i���񂩂�����Ȃ������˂�������Ȃ��j</param>
/// <param name="primitives">�S�v���~�e�B�u</param>
/// <param name="startObject">�Ăяo�����I�u�W�F�N�g</param>
/// <param name="limit">���ˉ񐔌��E</param>
/// <returns></returns>
bool TraceRay(const Ray& ray, const vector<Primitive*>& primitives, const Primitive* startObject, int limit, Color& color, int imgHD)
{
	// ���C�g����
	Vector3 lightVec = { 1.0f, -1.0f, -1.0f };
	lightVec.Normalize();
	if (limit == 0)
	{
		return false;
	}

	// �f�v�X�J���[
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
				// �����̐F
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
					float ambient = material.ambientRate;					// ����ȏ�Â��Ȃ�Ȃ����邳
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
						// �����̐F�Ɣ��ːF�������� src*(1-��)+dst*��
						color = selfColor * (1.0f - material.reflectionRate) + refCol * material.reflectionRate;
					}
					else
					{
						color = selfColor;
					}
				}
				else
				{
					// �e�v�Z
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

///���C�g���[�V���O
///@param eye ���_���W
///@param sphere ���I�u�W�F�N�g(���̂��������ɂ���)
void RayTracing(const Position3& eye,const vector<Primitive*> primitives,int imgHD) 
{
	memset(canvas, 0, sizeof(unsigned int) * screen_height * screen_width);
	// ���C�g�̍��W
	Vector3 lightVec = { 1.0f, -1.0f, -1.0f };
	lightVec.Normalize();
#pragma omp parallel for 
	for (int y = 0; y < screen_height; ++y)//�X�N���[���c����
	{
#pragma omp parallel for 
		for (int x = 0; x < screen_width; ++x)//�X�N���[�������� 
		{
			// ���_�ƃX�N���[�����W���王���x�N�g�������
			Ray ray = { eye, {x - screen_width / 2 - eye.x, screen_height / 2 - y - eye.y, -eye.z} };
			ray.vec.Normalize();
			Color finalColor;
			// ���C���g���[�X����
			if (TraceRay(ray, primitives, nullptr, 5, finalColor, imgHD))
			{
				finalColor *= 255;
				canvas[y][x] = GetColor(finalColor.x, finalColor.y, finalColor.z);
			}
			else
			{
				// �w��F�Ŕw�i��h��Ԃ�
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
	SetMainWindowText(_T("2016010_�������l"));
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	int imgHD = LoadSoftImage(_T("image/image1.png"));

	// ����
	Sphere sphere = Sphere(100, Position3(0, 0, -150), 
		Material(Color(1.0f, 0.25f, 0.5f), // �F�@
			Color(), // �F�A
			0.2f, // ��������
			1.0f, // �X�y�L��������
			50.0f, // �X�y�L��������
			0.5f, // ���˗�
			Pattern::none));// �͗l���

	Sphere sphere2 = Sphere(100, Position3(250, 0, -150),
		Material(Color(0.5f, 1.0f, 0.25f), // �F�@
			Color(), // �F�A
			0.2f, // ��������
			1.0f, // �X�y�L��������
			50.0f, // �X�y�L��������
			0.5f, // ���˗�
			Pattern::none));// �͗l���

	Sphere sphere3 = Sphere(100, Position3(-250, 0, -150),
		Material(Color(1.0f, 1.0f, 0.25f), // �F�@
			Color(), // �F�A
			0.2f, // ��������
			1.0f, // �X�y�L��������
			50.0f, // �X�y�L��������
			0.5f, // ���˗�
			Pattern::none));// �͗l���

	// ��
	Plane plane = { Vector3(0,1,0), -100, 
		Material(Color(0.5f,1.0f,1.0f),  // �F�@
			Color(0.125f,0.5f,0.5f), // �F�A
			1.0f, // ��������
			0.0f, // �X�y�L��������
			0.0f, // �X�y�L��������
			0.0f, // ���˗�
			Pattern::checker) };// �͗l�F�`�F�b�N

	// �z��
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