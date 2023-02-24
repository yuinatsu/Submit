#pragma once
#include <list>
#include <memory>
#include "../BehaviorBaseNode.h"

using NodeList = std::list<std::shared_ptr<BehaviorBaseNode>>;
using NodeListItr = NodeList::iterator;

// セレクターorシークエンスノードとかの基底クラス
class ParentNode :
	public BehaviorBaseNode
{
public:
	ParentNode();
	virtual ~ParentNode();

	/// <summary> 子ノードを追加する </summary>
	/// <param name="child"> 追加したいノード </param>
	/// <returns> 成功時true失敗時false </returns>
	bool PushChildren(std::unique_ptr<BehaviorBaseNode>&& child);

protected:

	/// <summary> 再度ルートからやり直す時に呼ぶ </summary>
	/// <param name=""></param>
	void Reset(void) override;

	// 子ノードリスト
	NodeList children_;

	// 現在処理中の子ノード
	NodeListItr nowItr_;
};

