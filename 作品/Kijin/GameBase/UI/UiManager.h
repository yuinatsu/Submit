#pragma once
#include <memory>
#include <list>
#include <vector>
#include <fstream>
#include <atomic>
#include <filesystem>
#include "../Scene/SceneID.h"
#include "../Common/Vector2.h"

class BaseScene;
class ObjectManager;
class UiBase;
class Controller;

class UiManager
{
public:
	UiManager(const std::filesystem::path& path, bool isAsync = true, bool isTutorial = false, bool isCreateCursor = true);
	~UiManager();


	void Begin(BaseScene& scene);


	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="delta"></param>
	/// <param name="scene"></param>
	/// <param name="obj"></param>
	/// <param name="controller"></param>
	void Update(float delta, BaseScene& scene, ObjectManager& obj, Controller& controller);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="mainScr">���C���̃X�N���[��</param>
	void Draw(int mainScr);

	const std::list<std::unique_ptr<UiBase>>& GetUiList(void) const { return uiList_; }


	/// <summary>
	/// �w��̃t�@�C���p�X����ui�𐶐�������������
	/// </summary>
	/// <param name="path"> ui�f�[�^�̃t�@�C���p�X </param>
	/// <returns> ������true�A���s��false </returns>
	bool InitUi(const std::filesystem::path& path, bool isCreateCursor = true);

	/// <summary>
	/// ���[�h�������Ă��邩
	/// </summary>
	/// <param name=""></param>
	/// <returns> ���[�h������true�A��������false </returns>
	bool IsLoaded(void);
private:

	/// <summary>
	/// �t�@�C���X�g���[������{�^���𐶐�����
	/// </summary>
	/// <param name="file"> �t�@�C���X�g���[�� </param>
	/// <param name="num"> �� </param>
	void CreateButton(std::ifstream& file, int num);

	/// <summary>
	/// �t�@�C���X�g���[������摜(UI)�𐶐�����
	/// </summary>
	/// <param name="file"> �t�@�C���X�g���[�� </param>
	/// <param name="num"> �� </param>
	void CreateImage(std::ifstream& file, int num);

	/// <summary>
	/// �t�@�C���X�g���[������X���C�_�[�𐶐�����
	/// </summary>
	/// <param name="file"> �t�@�C���X�g���[�� </param>
	/// <param name="num"> �� </param>
	void CreateSlider(std::ifstream& file, int num);

	/// <summary>
	/// �t�@�C���X�g���[������C���v�b�g�Z�b�g�𐶐�����
	/// </summary>
	/// <param name="file"> �t�@�C���X�g���[�� </param>
	/// <param name="num"> �� </param>
	void CreateInputSet(std::ifstream& file, int num);

	/// <summary>
	/// �t�@�C���X�g���[�����烍�[�h�\���𐶐�����
	/// </summary>
	/// <param name="file"> �t�@�C���X�g���[�� </param>
	/// <param name="num"> �� </param>
	void CreateLoadIndicator(std::ifstream& file, int num);

	/// <summary>
	/// �t�@�C���X�g���[������Gage�𐶐�����
	/// </summary>
	/// <param name="file"> �t�@�C���X�g���[�� </param>
	/// <param name="num"> �� </param>
	void CreateGage(std::ifstream& file, int num);

	/// <summary>
	/// �t�@�C���X�g���[������摜(divgraph��)�𐶐�����
	/// </summary>
	/// <param name="file"></param>
	/// <param name="num"></param>
	void CreateImageArray(std::ifstream& file, int num);

	/// <summary>
	/// �t�@�C���X�g���[������R���{����UI�𐶐�����
	/// </summary>
	/// <param name="file"></param>
	/// <param name="num"></param>
	void CreateCombo(std::ifstream& file, int num);

	/// <summary>
	/// �t�@�C���X�g���[������X�C�b�`�{�^���𐶐�����
	/// </summary>
	/// <param name="file"></param>
	/// <param name="num"></param>
	void CreateSwitchButton(std::ifstream& file, int num);

	/// <summary>
	/// �t�@�C���X�g���[������~�j�}�b�v�̍쐬
	/// </summary>
	/// <param name="file"></param>
	/// <param name="num"></param>
	void CreateMinMap(std::ifstream& file, int num);

	// UI�p�̃��X�g
	std::list<std::unique_ptr<UiBase>> uiList_;

	// ���[�h�������Ă��邩�̃t���O
	std::atomic_bool isLoaded_;
};

