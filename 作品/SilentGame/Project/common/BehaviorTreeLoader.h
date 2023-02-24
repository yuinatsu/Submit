#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>
#include "../common/rapidxml_utils.hpp"

class BehaviorBaseNode;
class Obj;

enum class NodeState;


// Behavior�����t�@�C������ǂݍ����node�𐶐�����N���X
class BehaviorTreeLoader
{
	// �e�m�[�h�쐬�pfunction
	using CreateNodeFunc = std::function<std::unique_ptr<BehaviorBaseNode>(rapidxml::xml_node<char>*)>;

	// �A�N�V�����m�[�h�ɓn��function
	using ActFunc = std::function< NodeState(Obj&, float)>;

	// �A�N�V�����m�[�h�ɓn��function���܂Ƃ߂��}�b�v
	using ActFuncMap = std::unordered_map<std::string_view, std::function<ActFunc(rapidxml::xml_node<char>*)>>;

	// �R���f�B�V�����m�[�h�ɓn��function
	using CheckFunc = std::function<bool(Obj&, float)>;

	// �R���f�B�V�����m�[�h�ɓn��function���܂Ƃ߂��}�b�v
	using CheckFuncMap = std::unordered_map < std::string_view, std::function<CheckFunc(rapidxml::xml_node<char>*)>>;

public:
	BehaviorTreeLoader();
	~BehaviorTreeLoader();

	/// <summary> �w���xml�t�@�C����ǂݍ���BehviorNode��Ԃ� </summary>
	/// <param name="name"> �t�@�C���� </param>
	/// <returns> BehviorNode </returns>
	std::unique_ptr<BehaviorBaseNode> Load(const std::string& name);

private:

	/// <summary> �A�N�V�����m�[�h�ɓn���悤�֐��I�u�W�F�N�g�𐶐����� </summary>
	/// <param name=""></param>
	void CreateActFuncMap(void);

	/// <summary> �R���f�B�V�����m�[�h�ɓn���悤�֐��I�u�W�F�N�g�𐶐����� </summary>
	/// <param name=""></param>
	void CreateConditionFuncMap(void);

	// �ǂݍ��񂾃m�[�h���}�b�v�Ŏ���
	std::unordered_map<std::string, std::unique_ptr<BehaviorBaseNode>> nodeMap_;

	// xml�p
	rapidxml::xml_document<> xmldoc_;

	// �m�[�h�𐶐����邽�߂̊֐��I�u�W�F�N�g���}�b�v�Ŏ���
	std::unordered_map<std::string_view, CreateNodeFunc> createFuncMap_;

	// �A�N�V�����m�[�h�ɓn���悤�֐��I�u�W�F�N�g���}�b�v�Ŏ���
	ActFuncMap actFuncMap_;

	// �R���f�B�V�����m�[�h�ɓn���悤�֐��I�u�W�F�N�g���}�b�v�Ŏ���
	CheckFuncMap ckFuncMap_;
};

