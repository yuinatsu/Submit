#pragma once
#include <memory>
#include <functional>
#include "Math.h"
#include "ScreenID.h"

// DxLib�̃n���h�������[�N���Ȃ��悤�ɋ��L���邽�߂̃N���X
class SharedHandle
{
public:
	SharedHandle(){}
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle"> �Ǘ���C����n���h�� </param>
	SharedHandle(const int handle);

	/// <summary>
	/// ���[�u�R���X�g���N�^
	/// </summary>
	/// <param name="sharedHandle"></param>
	/// <returns></returns>
	SharedHandle(SharedHandle&& sharedHandle) noexcept;

	/// <summary>
	/// �R�s�[�R���X�g���N�^
	/// </summary>
	/// <param name="sharedHandle"></param>
	/// <returns></returns>
	SharedHandle(const SharedHandle& sharedHandle) noexcept;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	/// <returns></returns>
	virtual ~SharedHandle() noexcept;


	SharedHandle& operator=(SharedHandle&& sharedHandle) noexcept;

	/// <summary>
	/// �n���h�����擾���邽��
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	virtual const int operator*(void) const noexcept;

	void SetPtr(std::shared_ptr<int>& ptr);

protected:

	// �n���h�����Ǘ����邽�߂�shared_ptr
	std::shared_ptr<int> ptr_;

};

// �O���t�B�b�N�p
class SharedGraphicHandle :
	public SharedHandle
{
public:
	SharedGraphicHandle(){}
	explicit SharedGraphicHandle(const int handle) ;
	SharedGraphicHandle(const SharedGraphicHandle& sharedHandle) noexcept;
	~SharedGraphicHandle();
	SharedGraphicHandle& operator=(const SharedGraphicHandle& sharedHandle) noexcept;
};

// �X�N���[���p
class SharedRenderTargetHandle :
	public SharedHandle
{
public:
	SharedRenderTargetHandle() : id_{} {}
	explicit SharedRenderTargetHandle(const int handle, const ScreenID id);
	SharedRenderTargetHandle(const SharedRenderTargetHandle& sharedHandle) noexcept;
	~SharedRenderTargetHandle();
	SharedRenderTargetHandle& operator=(const SharedRenderTargetHandle& sharedHandle) noexcept;
	void SetScreenID(ScreenID id) { id_ = id; }
private:
	ScreenID id_;
};

// MV1model�p
class SharedModelHandle :
	public SharedHandle
{
public:
	SharedModelHandle(){}
	~SharedModelHandle();
	SharedModelHandle& operator=(const SharedModelHandle& sharedHandle) noexcept;
	void CopyParent(void);
	void SetHandle(const int handle);
	void SetParent(std::shared_ptr<int>& parent);
	const int operator*(void) const noexcept final;
private:
	int handle_{-1};
};

// �V�F�[�_�p
class SharedShaderHandle :
	public SharedHandle
{
public:
	SharedShaderHandle(){}
	explicit SharedShaderHandle(const int handle);
	SharedShaderHandle(const SharedShaderHandle& sharedHandle) noexcept;
	~SharedShaderHandle();
	SharedShaderHandle& operator=(const SharedShaderHandle& sharedHandle) noexcept;
};

// �T�E���h�p
class SharedSoundHandle :
	public SharedHandle
{
public:
	SharedSoundHandle() {}
	~SharedSoundHandle();
	SharedSoundHandle& operator=(const SharedSoundHandle& sharedHandle) noexcept;
	void SetHandle(const int handle);
	void CopyParent(void);
	int handle_{-1};
};

// �t�H���g�p
class SharedFontHandle :
	public SharedHandle
{
public:
	SharedFontHandle() {}
	SharedFontHandle(const int handle);
	SharedFontHandle(const SharedFontHandle& sharedHandle) noexcept;
	~SharedFontHandle();
	SharedFontHandle& operator=(const SharedFontHandle& sharedHandle) noexcept;
};

class SharedEffectHandle :
	public SharedHandle
{
public:
	SharedEffectHandle() {}
	explicit SharedEffectHandle(const int handle);
	SharedEffectHandle(const SharedEffectHandle& sharedHandle) noexcept;
	~SharedEffectHandle();
	SharedEffectHandle& operator=(const SharedEffectHandle& sharedHandle) noexcept;
};