#pragma once
#include <deque>
#include <tuple>
#include "Math.h"

using Collision = std::pair<Math::Vector2, Math::Vector2>;			// �n�_�@,�@���ƍ���
using ColData = std::deque<Collision>;

class MapCollMng
{
public:
	struct Ray
	{
		Math::Vector2 p;		// �n�_
		Math::Vector2 vec;		// �����x�N�g��
	};
	struct Line
	{
		Math::Vector2 p;		// �n�_
		Math::Vector2 end;		// �I�_
	};
	static std::pair<bool, Math::Vector2> CheckCollision(Ray ray, Collision col);


	/// <summary> ���C�ƃR���W������񂩂瓖���������ǂ����̃t���O�Ɠ��������Ƃ���܂ł̃x�N�g���ƃR���W�����̃x�N�g��(���K��)��Ԃ� </summary>
	/// <param name="ray"> ���C </param>
	/// <param name="col"> �R���W���� </param>
	/// <returns></returns>
	static std::tuple<bool, Math::Vector2, Math::Vector2> CheckColl2(Ray& ray, Collision& col);
	static std::pair<bool, Math::Vector2> CheckRay(Ray ray, Line line);
private:
};

