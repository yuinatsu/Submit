#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>
#include "../common/rapidxml_utils.hpp"

class BehaviorBaseNode;
class Obj;

enum class NodeState;


// Behavior情報をファイルから読み込んでnodeを生成するクラス
class BehaviorTreeLoader
{
	// 各ノード作成用function
	using CreateNodeFunc = std::function<std::unique_ptr<BehaviorBaseNode>(rapidxml::xml_node<char>*)>;

	// アクションノードに渡すfunction
	using ActFunc = std::function< NodeState(Obj&, float)>;

	// アクションノードに渡すfunctionをまとめたマップ
	using ActFuncMap = std::unordered_map<std::string_view, std::function<ActFunc(rapidxml::xml_node<char>*)>>;

	// コンディションノードに渡すfunction
	using CheckFunc = std::function<bool(Obj&, float)>;

	// コンディションノードに渡すfunctionをまとめたマップ
	using CheckFuncMap = std::unordered_map < std::string_view, std::function<CheckFunc(rapidxml::xml_node<char>*)>>;

public:
	BehaviorTreeLoader();
	~BehaviorTreeLoader();

	/// <summary> 指定のxmlファイルを読み込みBehviorNodeを返す </summary>
	/// <param name="name"> ファイル名 </param>
	/// <returns> BehviorNode </returns>
	std::unique_ptr<BehaviorBaseNode> Load(const std::string& name);

private:

	/// <summary> アクションノードに渡すよう関数オブジェクトを生成する </summary>
	/// <param name=""></param>
	void CreateActFuncMap(void);

	/// <summary> コンディションノードに渡すよう関数オブジェクトを生成する </summary>
	/// <param name=""></param>
	void CreateConditionFuncMap(void);

	// 読み込んだノードをマップで持つ
	std::unordered_map<std::string, std::unique_ptr<BehaviorBaseNode>> nodeMap_;

	// xml用
	rapidxml::xml_document<> xmldoc_;

	// ノードを生成するための関数オブジェクトをマップで持つ
	std::unordered_map<std::string_view, CreateNodeFunc> createFuncMap_;

	// アクションノードに渡すよう関数オブジェクトをマップで持つ
	ActFuncMap actFuncMap_;

	// コンディションノードに渡すよう関数オブジェクトをマップで持つ
	CheckFuncMap ckFuncMap_;
};

