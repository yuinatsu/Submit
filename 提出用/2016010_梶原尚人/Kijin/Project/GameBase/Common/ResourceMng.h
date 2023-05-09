#pragma once
#include <filesystem>
#include <map>
#include <mutex>
#include <list>
#include <tuple>
#include "SharedHandle.h"
#include "ScreenID.h"

// ���ɓǂݍ��񂾃��[�X���L��ΕԂ����߂̃N���X
class ResourceMng
{
	using HandleFlagPair = std::pair<std::shared_ptr<int>, bool>;
	using HandleMap = std::map<std::uint64_t, HandleFlagPair>;
	using HandleDivMap = std::map <std::uint64_t, std::tuple<std::vector<int>, std::shared_ptr<int>, bool>>;
	using ScreenMap = std::map<ScreenID, HandleFlagPair>;
	using ModelMap = std::map<std::uint64_t, std::tuple<std::shared_ptr<int>, bool>>;

public:
	ResourceMng();
	~ResourceMng();

	/// <summary>
	/// �摜�̓ǂݍ���
	/// </summary>
	/// <param name="path"> �摜��file�p�X </param>
	/// <param name="isNotRelese"> Relese���Ă΂ꂽ���ɍ폜���Ȃ���? </param>
	void LoadTexture(SharedGraphicHandle& out,const std::filesystem::path& path, bool isNotRelese = false);

	/// <summary>
	/// �摜�̕����ǂݍ���
	/// </summary>
	/// <param name="out"></param>
	/// <param name="path"> �摜��file�p�X </param>
	/// <param name="divCount"></param>
	/// <param name="divSize"></param>
	/// <param name="isNotRelese"> Relese���Ă΂ꂽ���ɍ폜���Ȃ���? </param>
	void LoadDivTexture(SharedDivGraphicHandle& out, const std::filesystem::path& path, const Vector2I& divCount, const Vector2I& divSize, bool isNotRelese = false);

	/// <summary>
	/// ��L��MV1��
	/// </summary>
	/// <param name="path"></param>
	/// <param name="isNotRelese"></param>
	/// <returns></returns>
	void LoadMv1Texture(SharedGraphicHandle& out,const std::filesystem::path& path, bool isNotRelese = false);

	/// <summary>
	/// ��v����O���t�B�b�N�n���h�����Ǘ�����폜����
	/// </summary>
	/// <param name="handle"></param>
	void Remove(SharedGraphicHandle& handle);

	/// <summary>
	/// ��v����O���t�B�b�N�n���h�����Ǘ�����폜����
	/// </summary>
	/// <param name="handle"></param>
	void Remove(SharedDivGraphicHandle& handle);

	/// <summary>
	/// �����_�[�^�[�Q�b�g���쐬����
	/// </summary>
	/// <param name="id"> ID </param>
	/// <param name="size"> �T�C�Y </param>
	/// <param name="alpha"> �A���t�@�`�����l�����g���� </param>
	/// <param name="isNotRelese"> Relese���Ă΂ꂽ���ɍ폜���Ȃ��� </param>
	/// <returns></returns>
	void MakeRenderTarget(SharedRenderTargetHandle& out,const ScreenID id,const Vector2& size, bool alpha, bool isNotRelese = false);

	/// <summary>
	/// �폜����
	/// </summary>
	/// <param name="id"></param>
	void Remove(const ScreenID id);

	/// <summary>
	/// model�����[�h����
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	void LoadModel(SharedModelHandle& out,const std::filesystem::path& path, bool isNotRelese = false);

	/// <summary>
	/// ��v���郂�f���̃n���h�����Ǘ�����폜����
	/// </summary>
	/// <param name="handle"></param>
	void RemoveModel(const int handle);

	/// <summary>
	/// �T�E���h�̃��[�h
	/// </summary>
	/// <param name="path"> �T�E���h�t�@�C���̃p�X </param>
	/// <param name="isNotRelese"> Relese���Ă΂ꂽ���ɍ폜���Ȃ��� </param>
	/// <returns></returns>
	void LoadSound(SharedSoundHandle& out,const std::filesystem::path& path, bool isNotRelese);


	/// <summary>
	/// ��v����T�E���h�̃n���h�����Ǘ�����폜����
	/// </summary>
	/// <param name="handle"></param>
	void RemoveSound(const int handle);

	/// <summary>
	/// �s�N�Z���V�F�[�_��ǂݍ���
	/// </summary>
	/// <param name="path"></param>
	/// <param name="isNotRelese"></param>
	/// <returns></returns>
	void LoadPS(SharedShaderHandle& out,const std::filesystem::path& path, bool isNotRelese = false);

	/// <summary>
	/// ���_�V�F�[�_��ǂݍ���
	/// </summary>
	/// <param name="path"></param>
	/// <param name="isNotRelese"></param>
	/// <returns></returns>
	void LoadVS(SharedShaderHandle& out,const std::filesystem::path& path, bool isNotRelese = false);

	/// <summary>
	/// ��v����V�F�[�_�n���h�����Ǘ�����폜���ׂ���
	/// </summary>
	/// <param name="handle"></param>
	/// <returns></returns>
	bool IsRemove(SharedShaderHandle& handle);

	/// <summary>
	/// ��v����V�F�[�_�n���h�����Ǘ�����폜����
	/// </summary>
	/// <param name="handle"></param>
	void Remove(SharedShaderHandle& handle);

	/// <summary>
	/// �G�t�F�N�V�A�̃G�t�F�N�g���\�[�X��ǂݍ���
	/// </summary>
	/// <param name="out"></param>
	/// <param name="path"></param>
	/// <param name="isNotRelese"></param>
	void LoadEffect(SharedEffectHandle& out, const std::filesystem::path& path, bool isNotRelese = false);

	/// <summary>
	/// �G�t�F�N�V�A�̃G�t�F�N�g���\�[�X���폜����
	/// </summary>
	/// <param name="handle"></param>
	void Remove(SharedEffectHandle& handle);


	void Loaded(void);
private:
	std::mutex mutex_;

	// ���[�h�����p�X���L�[�ɉ�����ׂ����̃t���O�Ƃ��ꂼ��̃n���h��������
	HandleMap imageMap_;
	HandleDivMap imageDivMap_;
	ScreenMap screenMap_;
	HandleMap soundMap_;
	ModelMap modelMap_;
	HandleMap shaderMap_;
	HandleMap effectMap_;

	std::list<std::function<void(void)>> loadedFunc_;
};

