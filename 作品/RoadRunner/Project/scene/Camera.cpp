#include <algorithm>
#include "Camera.h"
#include "Obj/Obj.h"
#include "MapCtl.h"
#include "SceneMng.h"
#include "../_debug/_DebugDispOut.h"


Camera::Camera()
{
	// pos_�̓[���N���̎��_�ŏ���������Ă�̂ŏ����K�v�Ȃ�
}


Camera::~Camera()
{
}


bool Camera::InitArea(void)
{
	Vector2 tmpPos = (lpMapCtl.viewArea_ / 2) * lpMapCtl.chipSize_;						// viewArea_�̔����̑傫���~chipSize_

	antiMoveRect_.left = tmpPos.x;					// worldArea_�̒��S(���[����tmpPos���E�ɂ��ꂽ��)�̍��_
	antiMoveRect_.top = tmpPos.y;					// �J�����̏㉺�̓����������Ȃ�
	antiMoveRect_.right = (lpMapCtl.worldArea_.x - ((lpMapCtl.viewArea_.x / 2) + lpMapCtl.viewArea_.x % 2)) * lpMapCtl.chipSize_.x;				// worldArea_�̒��S(worldArea_�̉E�[����tmpPos�����ɂ��ꂽ��)�̉E�_
	antiMoveRect_.bottom = antiMoveRect_.top;				// top�Ɠ����l������

	defPos_ = { antiMoveRect_.left,antiMoveRect_.top };					// �f�t�H���g���W��ݒ�

	return false;
}


bool Camera::SetTarget(std::weak_ptr<Obj> target)
{
	target_ = target;
	if (target_.expired())				// target_�̎������s���Ă��邩�𒲂ׂ�			expired ... �������s���Ă��邩�𒲂ׂ�i����ł�Ftrue, �����Ă�Ffalse�j
	{
		// �J����������ł���ꍇ
		return false;
	}
	// �J�����������Ă���ꍇ

	InitArea();

	pos_.x = target.lock()->GetPos().x;										//	�v���C���[��X���W���擾					weak_ptr��(*target)->�ȂǂŃA�N�Z�X�͂ł��Ȃ�
	pos_.x = std::clamp(pos_.x, static_cast<int>(antiMoveRect_.left), static_cast<int>(antiMoveRect_.right));						// X���W�̃}�X�ڂ͈̔̓`�F�b�N(�v���C���[�̍��W�𒼐ړ��ꂽ��worldArea_�O��\�����Ă��܂��\�������邽��)
	pos_.y = antiMoveRect_.top;

	return true;
}


void Camera::Update(void)
{
	if (!target_.expired())
	{
		pos_.x = target_.lock()->GetPos().x;								//	�v���C���[��X���W���擾
		pos_.x = std::clamp(pos_.x, static_cast<int>(antiMoveRect_.left), static_cast<int>(antiMoveRect_.right));						// X���W�̃}�X�ڂ͈̔̓`�F�b�N
	}
}


void Camera::Draw(void)
{
	auto offset = lpSceneMng.drawOffset_;							// �J�����̒��S�ʒu���I�t�Z�b�g�����炷
	_dbgDrawBox(pos_.x + offset.x, pos_.y + offset.y, pos_.x + lpMapCtl.chipSize_.x + offset.x, pos_.y + lpMapCtl.chipSize_.y + offset.y, 0x00ff00, true);						// (�J������X���W, �J������Y���W, �J������X���W + �`�b�v�T�C�Y, �J������Y���W + �`�b�v�T�C�Y, �ΐF�i�f�o�b�O�p�j, true) ... �l���_���ׂĂɃI�t�Z�b�g�𑫂�
}


void Camera::SetPos(const Vector2& pos)
{
	pos_.x = std::clamp(pos.x, static_cast<int>(antiMoveRect_.left), static_cast<int>(antiMoveRect_.right));
	pos_.y = std::clamp(pos.y, static_cast<int>(antiMoveRect_.top), static_cast<int>(antiMoveRect_.bottom));
}


const Vector2& Camera::GetPos(void)
{
	return pos_;
}


const Vector2 Camera::GetDrawOffset(void)
{
	return (defPos_ - pos_);
}