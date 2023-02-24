#pragma once
#include <DxLib.h>
#include <vector>
#include <memory>
#include "../common/Math.h"

class TmxObj;

// ����(1����)
class Footprints
{
public:
	Footprints(std::vector<Math::Vector2I>& route,std::shared_ptr<TmxObj>& tmxObj);
	~Footprints();
	void Draw(const Math::Vector2& offset);
private:
	
	/// <summary> �J�n���W�ƏI�����W���璸�_��index���Z�b�g���� </summary>
	/// <param name="start"> �J�n���W </param>
	/// <param name="end"> �I�����W </param>
	void SetVertexAndIndex(const Math::Vector2& start, const Math::Vector2& end);

	// ���_
	std::vector<VERTEX2D> vert_;

	// index
	std::vector<unsigned short> ind_;

	// ���Ոꃉ�C�����������邩�̐�
	int num_;
};

