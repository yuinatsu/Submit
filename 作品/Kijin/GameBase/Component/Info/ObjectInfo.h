#pragma once
#include "../ComponentBase.h"
#include "ObjectAttribute.h"

// オブジェクトの情報を保持するクラス
class ObjectInfo :
    public ComponentBase
{
public:
	ObjectInfo();
	void Begin(ObjectManager& objectManager) final;

	void End(ObjectManager& objectManager) final;

	/// <summary>
	/// オブジェクトが有効かを調べる
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const bool IsActive(void) const;

	/// <summary>
	/// オブジェクトを破棄する
	/// </summary>
	/// <param name=""></param>
	void Destory(void)
	{
		isActive_ = false;
	}

	/// <summary>
	/// ファクトリーを使って削除処理を行うか
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool UseFactory(void)
	{
		return useFactory_;
	}

	ComponentID GetID(void) const override
	{
		return id_;
	}

	void SetUseFactory(const bool flag)
	{
		useFactory_ = flag;
	}

	void SetAttribute(const ObjectAttribute atr)
	{
		attribute_ = atr;
	}

	const ObjectAttribute GetAttribute(void) const
	{
		return attribute_;
	}

	SetID(ComponentID::Info, ComponentID::Info)
private:

	// activeかのフラグ
	bool isActive_ = false;

	// factoryを使った削除を行うか
	bool useFactory_ = false;


	// オブジェクトの属性(敵かプレイヤーのかそれ以外か)
	ObjectAttribute attribute_;
};

