#pragma once


//�T�C�Y��\���\����
struct Size {
	float w;//��
	float h;//����
};

//2D���W�E�x�N�g����\���\����
struct Vector2 {
	Vector2():x(0),y(0){}
	Vector2(float inx,float iny):x(inx),y(iny){}
	float x, y;
	///�x�N�g���̑傫����Ԃ��܂�
	float Magnitude()const;
	
	///���K��(�傫�����P��)���܂�
	void Normalize();

	///���K���x�N�g����Ԃ��܂�
	Vector2 Normalized();

	void operator+=(const Vector2& v);
	void operator-=(const Vector2& v);
	void operator*=(float scale);
	Vector2 operator*(float scale);
	Vector2 operator-()const {
		return Vector2(-x, -y);
	}
};

Vector2 operator+(const Vector2& va, const Vector2 vb);
Vector2 operator-(const Vector2& va, const Vector2 vb);

///���ς�Ԃ�
float Dot(const Vector2& va, const Vector2& vb);

///�O�ς�Ԃ�
float Cross(const Vector2& va, const Vector2& vb);

///���ω��Z�q
float operator*(const Vector2& va, const Vector2& vb);

///�O�ω��Z�q
float operator%(const Vector2& va, const Vector2& vb);

//�Ƃ肠�����u���W�v���ĈӖ����ƃx�N�^���
//Position�̂ق����悭�ˁH���ė��R�ł��̖��O
typedef Vector2 Position2;


//3D���W�E�x�N�g����\���\����
struct Vector3 {
	Vector3() :x(0), y(0) ,z(0){}
	Vector3(float inx, float iny,float inz) :x(inx), y(iny) ,z(inz){}
	float x, y,z;
	///�x�N�g���̑傫����Ԃ��܂�
	float Magnitude()const;

	///���K��(�傫�����P��)���܂�
	void Normalize();

	///���K���x�N�g����Ԃ��܂�
	Vector3 Normalized();

	void operator+=(const Vector3& v);
	void operator-=(const Vector3& v);
	void operator*=(float scale);
	Vector3 operator*(float scale)const;
	Vector3 operator-()const {
		return Vector3(-x, -y,-z);
	}
};
Vector3 operator+(const Vector3& va, const Vector3 vb);
Vector3 operator-(const Vector3& va, const Vector3 vb);

///���ς�Ԃ�
float Dot(const Vector3& va, const Vector3& vb);

///�O�ς�Ԃ�
Vector3 Cross(const Vector3& va, const Vector3& vb);

///���ω��Z�q
float operator*(const Vector3& va, const Vector3& vb);

///�O�ω��Z�q
Vector3 operator%(const Vector3& va, const Vector3& vb);
typedef Vector3 Position3;

///�~��\���\����
struct Circle {
	float radius;//���a
	Position2 pos; //���S���W
	Circle() :radius(0), pos(0, 0) {}
	Circle(float r, Position2& p) :radius(r), pos(p) {}
};

///��`��\���\����
struct Rect {
	Position2 pos; //���S���W
	int w, h;//��,����
	Rect() : pos(0, 0), w(0), h(0) {}
	Rect(float x, float y, int inw, int inh) :
		pos(x, y), w(inw), h(inh) {}
	Rect(Position2& inpos, int inw, int inh) :
		pos(inpos), w(inw), h(inh)
	{}
	void SetCenter(float x, float y) {
		pos.x = x;
		pos.y = y;
	}
	void SetCenter(const Position2& inpos) {
		pos.x = inpos.x;
		pos.y = inpos.y;
	}
	Vector2 Center() {
		return pos;
	}
	float Left() { return pos.x - w / 2; }
	float Top() { return pos.y - h / 2; }
	float Right() { return pos.x + w / 2; }
	float Bottom() { return pos.y + h / 2; }
	void Draw();//�����̋�`��`�悷��
	void Draw(Vector2& offset);//�����̋�`��`�悷��(�I�t�Z�b�g�t��)
};

struct Ray {
	Position3 start;//���C�J�n�n�_
	Vector3 vec;// ���C�x�N�g��
};

using Color = Vector3;

///�͗l
enum class Pattern {
	none,//1�F
	stripe,//�Ȗ͗l
	checker,//�s���͗l
};

///�}�e���A���\����
struct Material {
	Color color1 = {};//�F�@
	Color color2 = {};//�F�A
	float ambientRate = 0.0f;//�����䗦
	float specularRate = 1.0f;//���ʔ��˔䗦
	float specularity;//���ʔ��ˌ�����(R^n��n��\��)
	float reflectionRate = 0.0f;//���˗�
	Pattern pattern = Pattern::none;//�͗l
	Material() {};
	Material(const Color& col1, const Color& col2, float ambRate, float specRate, float specularPow, float refRate, Pattern pat);
};

///���́i���Ƃ����ʂ̊��N���X�j
struct Primitive {
	Material material;
	Primitive(const Material& mat) : material(mat) {};
	///���C�Ǝ������Փ˂�������Ԃ�
	///�������z�֐�
	///@param ray ���C�\����
	///@param t ��_�̋���
	///@param N ��_�ɂ�����@��
	virtual bool IsHitRay(const Ray& ray,float& t, Vector3& N)const = 0;
};


///����\���\����
struct Sphere : public Primitive{
	float radius;//���a
	Position3 pos; //���S���W
	Sphere() :radius(0), pos(0, 0, 0), Primitive(Material()) {}
	Sphere(float r, Position3& p, const Material& mat) :radius(r), pos(p), Primitive(mat){}

	///���C(����)�Ƌ��̂̓����蔻��
	///@param ray (���_����X�N���[���s�N�Z���ւ̃x�N�g��)
	///@param sphere ��
	///@param t[out] ��_�܂ł̋���
	///@hint ���C�͐��K�����Ƃ����ق����g���₷�����낤
	virtual bool IsHitRay(const Ray& ray, float& t, Vector3& N)const override;
};

// ���ʂ̍\����(Plane)
struct Plane : public Primitive {
	// ax + by + cz + d = 0�ɍ��킹��Ȃ�
	// float a, b, c, d; ������
	Vector3 N;			// �@���x�N�g��(���K���ς�)
	float offset;		// ���_����̃I�t�Z�b�g
	Plane(const Vector3& inN, float inOffset, const Material& mat) : N(inN), offset(inOffset), Primitive(mat) {};
	virtual bool IsHitRay(const Ray& ray, float& t, Vector3& N)const override;
};
