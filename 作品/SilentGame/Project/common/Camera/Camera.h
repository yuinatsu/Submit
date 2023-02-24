#pragma once
#include <memory>
#include <functional>
#include "../Math.h"


// �J�����̐����p�\����
struct CameraRect 
{
	Math::Vector2 leftTop;		// ���Ə�
	Math::Vector2 rightBottom;	// �E�Ɖ�
};

// �J�����̃��[�h
enum class CamMode
{
	Trace,			// ����̍��W��ǐՂ���
	MoveEvent,			// �J�����̃^�[�Q�b�g����ʂ̃^�[�Q�b�g�֌�����
	StartEvent,			// �J�n���̉��o�p�̃J�����̓���
	Non,
};



class Obj;
class CameraModeBase;
class TmxObj;
class Controller;

class Camera
{
public:

	/// <summary> �J�����̏���������(�ǐՃ��[�h�ŊJ�n) </summary>
	/// <param name="isSmallMap"> �}�b�v����������? </param>
	/// <param name="tmxObj"></param>
	/// <param name="traceTarget"> �ǐՂ���I�u�W�F�N�g </param>
	/// <returns></returns>
	bool Init(bool isSmallMap, std::shared_ptr<TmxObj>& tmxObj, const std::weak_ptr<Obj>& traceTarget);

	/// <summary> �J�����̏�����(�J�n���̉��o�p) </summary>
	/// <param name="isSmallMap"> �}�b�v����������? </param>
	/// <param name="tmxObj"></param>
	/// <param name="start"> �X�^�[�g���̃I�u�W�F�N�g </param>
	/// <param name="end"> ���[�̃I�u�W�F�N�g </param>
	/// <returns></returns>
	bool Init(Controller& controller,bool isSmallMap, std::shared_ptr<TmxObj>& tmxObj, const std::weak_ptr<Obj>& start, const std::weak_ptr<Obj>& end);


	bool Update(float delta);
	//bool Draw(TmxObj& tmxObj);  // �������g��`��
	Math::Vector2I GetViewSize();


	/// <summary> �`��p�I�t�Z�b�g���擾���� </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Math::Vector2 GetOffset(void) const;

	/// <summary> �J�����̍��W���擾���� </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Math::Vector2 GetPos(void) const { return pos_; };

	/// <summary> �ǐՂ���J�������J�n���� </summary>
	/// <param name="traceTarget"> �ǐՂ������I�u�W�F�N�g </param>
	void StartTraceMode(const std::weak_ptr<Obj>& traceTarget);

	/// <summary>  </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="allTime"></param>
	/// <param name="waitTime"></param>
	void StartMoveEvent(const std::weak_ptr<Obj>& start, const std::weak_ptr<Obj>& end, float allTime,float waitTime);


	void StartEvent(Controller& controller,const std::weak_ptr<Obj>& start, const std::weak_ptr<Obj>& end);

	/// <summary> ���݂̃J�����̃��[�h���擾 </summary>
	/// <param name=""> ���݂̃J�����̃��[�h </param>
	/// <returns></returns>
	const CamMode GetMode(void) const { return nowMode_; }

	void Draw(void);

	/// <summary> �J�����̕\���T�C�Y�ړ��������������� </summary>
	/// <param name="isSmallMap"> �}�b�v����������? </param>
	/// <param name="tmxObj"></param>
	/// <returns></returns>
	bool Init(bool isSmallMap, std::shared_ptr<TmxObj>& tmxObj);

	void Loaded(void);
private:

	
	bool isSmallMap_ = false;

	// ���[�h���Ƃ̃J�����̓�������������N���X
	std::unique_ptr<CameraModeBase> camMode_;

	// �J�����̐���
	CameraRect rect_;

	// �J�����̏����ʒu
	Math::Vector2 defPos_;

	// �J�����̕\���T�C�Y
	Math::Vector2 viewsize_;

	// �J�����̍��W
	Math::Vector2 pos_;




	// ���݂̃J�����̃��[�h
	CamMode nowMode_ = CamMode::Non;


	// �J�����̌o�ߎ���
	float cameraTime_ = 0.0f;

	friend class TraceModeCamera;
	friend class MoveEventModeCamera;
	friend class StartEventMode;

};

