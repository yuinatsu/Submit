#pragma once

struct Vector2 {
	float x, y;
	Vector2() : x(0.0f), y(0.0f) {}
	Vector2(float inx, float iny) : x(inx), y(iny) {}
	void operator+=(const Vector2& val);
	void operator-=(const Vector2& val);
	void operator*=(float scale);
	void operator/=(float div);
	bool operator==(const Vector2& val);

	/// <summary>
	/// �x�N�g���̑傫����Ԃ�
	/// </summary>
	/// <returns>�x�N�g���̑傫��</returns>
	float Magnitude()const;

	/// <summary>
	/// �x�N�g���̑傫����2���Ԃ�(������SQRT���Ȃ�)
	/// </summary>
	/// <returns>�x�N�g���̑傫����2��</returns>
	float SQMagnitude()const;

	/// <summary>
	/// �x�N�g���𐳋K������
	/// </summary>
	void Normalize();

	/// <summary>
	/// ���K���x�N�g����Ԃ�
	/// </summary>
	/// <returns></returns>
	Vector2 Normalized()const;

	/// <summary>
	/// 90�x��]����
	/// </summary>
	void Rotate90();
	Vector2 Rotated90()const;
	//static const Vector2 Zero() { return { 0.0f,0.0f }; }
	static const Vector2 ZERO;
};

using Position2 = Vector2;

// �錾�̂�
Vector2 operator+(const Vector2& lval, const Vector2& rval);
Vector2 operator-(const Vector2& lval, const Vector2& rval);
Vector2 operator*(const Vector2& lval, float scale);
Vector2 operator/(const Vector2& lval, float div);
//inline Vector2 operator+(const Vector2& lval, const Vector2& rval) {
//	Vector2 ret = { lval.x + rval.x,lval.y + rval.y };
//	return ret;
//}

