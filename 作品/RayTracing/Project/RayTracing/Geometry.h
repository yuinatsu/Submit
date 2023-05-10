#pragma once


//サイズを表す構造体
struct Size {
	float w;//幅
	float h;//高さ
};

//2D座標・ベクトルを表す構造体
struct Vector2 {
	Vector2():x(0),y(0){}
	Vector2(float inx,float iny):x(inx),y(iny){}
	float x, y;
	///ベクトルの大きさを返します
	float Magnitude()const;
	
	///正規化(大きさを１に)します
	void Normalize();

	///正規化ベクトルを返します
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

///内積を返す
float Dot(const Vector2& va, const Vector2& vb);

///外積を返す
float Cross(const Vector2& va, const Vector2& vb);

///内積演算子
float operator*(const Vector2& va, const Vector2& vb);

///外積演算子
float operator%(const Vector2& va, const Vector2& vb);

//とりあえず「座標」って意味だとベクタより
//Positionのほうがよくね？って理由でこの名前
typedef Vector2 Position2;


//3D座標・ベクトルを表す構造体
struct Vector3 {
	Vector3() :x(0), y(0) ,z(0){}
	Vector3(float inx, float iny,float inz) :x(inx), y(iny) ,z(inz){}
	float x, y,z;
	///ベクトルの大きさを返します
	float Magnitude()const;

	///正規化(大きさを１に)します
	void Normalize();

	///正規化ベクトルを返します
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

///内積を返す
float Dot(const Vector3& va, const Vector3& vb);

///外積を返す
Vector3 Cross(const Vector3& va, const Vector3& vb);

///内積演算子
float operator*(const Vector3& va, const Vector3& vb);

///外積演算子
Vector3 operator%(const Vector3& va, const Vector3& vb);
typedef Vector3 Position3;

///円を表す構造体
struct Circle {
	float radius;//半径
	Position2 pos; //中心座標
	Circle() :radius(0), pos(0, 0) {}
	Circle(float r, Position2& p) :radius(r), pos(p) {}
};

///矩形を表す構造体
struct Rect {
	Position2 pos; //中心座標
	int w, h;//幅,高さ
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
	void Draw();//自分の矩形を描画する
	void Draw(Vector2& offset);//自分の矩形を描画する(オフセット付き)
};

struct Ray {
	Position3 start;//レイ開始地点
	Vector3 vec;// レイベクトル
};

using Color = Vector3;

///模様
enum class Pattern {
	none,//1色
	stripe,//縞模様
	checker,//市松模様
};

///マテリアル構造体
struct Material {
	Color color1 = {};//色①
	Color color2 = {};//色②
	float ambientRate = 0.0f;//環境光比率
	float specularRate = 1.0f;//鏡面反射比率
	float specularity;//鏡面反射光強さ(R^nのnを表す)
	float reflectionRate = 0.0f;//反射率
	Pattern pattern = Pattern::none;//模様
	Material() {};
	Material(const Color& col1, const Color& col2, float ambRate, float specRate, float specularPow, float refRate, Pattern pat);
};

///物体（球とか平面の基底クラス）
struct Primitive {
	Material material;
	Primitive(const Material& mat) : material(mat) {};
	///レイと自分が衝突したかを返す
	///純粋仮想関数
	///@param ray レイ構造体
	///@param t 交点の距離
	///@param N 交点における法線
	virtual bool IsHitRay(const Ray& ray,float& t, Vector3& N)const = 0;
};


///球を表す構造体
struct Sphere : public Primitive{
	float radius;//半径
	Position3 pos; //中心座標
	Sphere() :radius(0), pos(0, 0, 0), Primitive(Material()) {}
	Sphere(float r, Position3& p, const Material& mat) :radius(r), pos(p), Primitive(mat){}

	///レイ(光線)と球体の当たり判定
	///@param ray (視点からスクリーンピクセルへのベクトル)
	///@param sphere 球
	///@param t[out] 交点までの距離
	///@hint レイは正規化しといたほうが使いやすいだろう
	virtual bool IsHitRay(const Ray& ray, float& t, Vector3& N)const override;
};

// 平面の構造体(Plane)
struct Plane : public Primitive {
	// ax + by + cz + d = 0に合わせるなら
	// float a, b, c, d; だけど
	Vector3 N;			// 法線ベクトル(正規化済み)
	float offset;		// 原点からのオフセット
	Plane(const Vector3& inN, float inOffset, const Material& mat) : N(inN), offset(inOffset), Primitive(mat) {};
	virtual bool IsHitRay(const Ray& ray, float& t, Vector3& N)const override;
};
