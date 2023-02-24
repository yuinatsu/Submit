#pragma once
#include <list>
#include <DxLib.h>
#include <fstream>
#include "../../Common/Math.h"
#include "../ComponentBase.h"



// トランスフォームのクラス
class Transform :
	public ComponentBase
{
public:
	Transform();

	/// <summary>
	/// 子オブジェクトのIDを追加
	/// </summary>
	/// <param name="id"> 子オブジェクト </param>
	void AddChild(const ObjectID& id);

	/// <summary>
	/// 子オブジェクトのIDの削除
	/// </summary>
	/// <param name="id"></param>
	void RemoveChild(const ObjectID& id);
	
	/// <summary>
	/// 親オブジェクトのIDをセット
	/// </summary>
	/// <param name="id"></param>
	void SetParent(const ObjectID& id)
	{
		parent_ = id;
	}

	/// <summary>
	/// 親オブジェクトのIDの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const ObjectID& GetParentID(void) const&
	{
		return parent_;
	}

	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;
	void Sync(Transform& parent);
	void End(ObjectManager& objectManager) final;

	/// <summary>
	/// 前方方向のベクトルを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3 GetForward(void) const;

	/// <summary>
	/// 上方向のベクトルを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3 GetUp(void) const;


	/// <summary>
	/// 右方向のベクトルを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3 GetRight(void) const;

	
	/// <summary>
	/// トランスフォームを指定ベクトルへ傾ける
	/// </summary>
	/// <param name="vec"> 傾ける方向のベクトル </param>
	void LookAt(const Vector3& vec);


	/// <summary>
	/// x,y,zで回転をセットする
	/// </summary>
	/// <param name="eulerRot"></param>
	void SetRotFromEulerRot(const Vector3& eulerRot);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="eulerRot"></param>
	void AddRotFromEulerRot(const Vector3& eulerRot);

	/// <summary>
	/// 回転を行列をセットする
	/// </summary>
	/// <param name="matrix"> 行列 </param>
	void SetRotationMatrix(const MATRIX& matrix)
	{
		rot_ = matrix;
	}

	MATRIX& Rot(void)&
	{
		return rot_;
	}

	/// <summary>
	/// 回転を追加する
	/// </summary>
	/// <param name="matrix"></param>
	void AddRotationMatrix(const MATRIX& matrix);

	/// <summary>
	/// ローカル回転をx,y,zでセットする
	/// </summary>
	/// <param name="eulerRot"></param>
	void SetLocalRotFromEulerRot(const Vector3& eulerRot);

	/// <summary>
	/// ローカル回転をx,y,zで追加する
	/// </summary>
	/// <param name="eulerRot"></param>
	void AddLocalRotFromEulerRot(const Vector3& eulerRot);

	void SetLocalRotaionMatrix(const MATRIX& matrix)
	{
		localRot_ = matrix;
	}

	/// <summary>
	/// 回転情報の行列を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns> 回転の行列 </returns>
	MATRIX GetRotationMatrix(void) const&
	{
		return MMult(MMult(localRot_,parentRot_), rot_);
	}

	const Vector3& GetPos(void) const&
	{
		return pos_;
	}

	Vector3& Pos(void)&
	{
		return pos_;
	}

	const Vector3& GetScale(void) const&
	{
		return scale_;
	}
	
	Vector3& Scale(void)&
	{
		return scale_;
	}

	Transform& operator=(const Transform& transform);

	

	/// <summary>
	/// ファイルからデータを読み込み適応する
	/// </summary>
	/// <param name="file"></param>
	void Read(std::ifstream& file);

	/// <summary>
	/// ファイルから座標、回転、拡縮のデータを取得する
	/// </summary>
	/// <param name="file"></param>
	/// <returns></returns>
	static std::tuple<Vector3, Vector3, Vector3> Load(std::ifstream& file);

	SetID(ComponentID::Transform, ComponentID::Transform)
		ComponentID GetID(void) const override
	{
		return id_;
	}
private:

	// 子オブジェクトのリスト
	std::list<ObjectID> children_;

	// 親オブジェクト
	ObjectID parent_;
	// 行列
	MATRIX rot_;

	// local座標
	MATRIX localRot_;

	// 親の回転
	MATRIX parentRot_;

	// 座標
	Vector3 pos_;

	// 拡大縮小
	Vector3 scale_{ 1.0f,1.0f,1.0f };

	// 親からのオフセット位置
	Vector3 offset_{ zeroVector3<float> };
};

