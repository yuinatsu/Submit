#pragma once
#include "DxLib.h"
class IntVector3
{

public:

	int x;
	int y;
	int z;

	// �R���X�g���N�^
	IntVector3();

	// �R���X�g���N�^
	IntVector3(int vX, int vY, int vZ);

	// �R���X�g���N�^
	IntVector3(VECTOR v);

	bool operator<(const IntVector3& value) const;
	
	void Add(int v);
	void Sub(int v);
	void Scale(int v);

};

