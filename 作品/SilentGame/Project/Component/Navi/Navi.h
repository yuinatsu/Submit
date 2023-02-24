#pragma once
#include <memory>
#include <list>
#include <mutex>
#include <thread>
#include <stop_token>
#include <functional>
#include "../../common/Math.h"
#include "../Component.h"


class TmxObj;

// Navi�̏��
enum class NaviState
{
	Search,		// �T����
	Find,		// �ړI�n�����݂���Ƃ�
	Non,		// �������Ȃ��~�܂��Ƃ�
};

// �o�H�T���p�N���X
class Navi :
	public Component
{
	// ���W�̃��X�g
	using Vector2lVec = std::list<Math::Vector2I>;

	// ���W�̃��X�g�̃C�e���[�^�[
	using Vector2lVecItr = Vector2lVec::iterator;


	using FuncListItr = std::list<std::function<void(void)>>::const_iterator;

public:
	Navi(std::shared_ptr<TmxObj>& tmxObj,Obj& owner);
	~Navi();

	/// <summary> �X�^�[�g����S�[���܂ł̌o�H�̒T�� </summary>
	/// <param name="start"> �X�^�[�g�n�_�̍��W </param>
	/// <param name="goal"> �S�[���n�_�̏��� </param>
	void Search(const Math::Vector2& start, const Math::Vector2I& goal);

	void Search(const Math::Vector2& start, const Math::Vector2& goal);

	/// <summary> Navi�N���X�̏�Ԃ��擾���� </summary>
	/// <param name=""></param>
	/// <returns> Navi�N���X�̏�� </returns>
	NaviState GetNaviState(void);

	/// <summary> ���s���ׂ����W���擾 </summary>
	/// <param name=""></param>
	/// <returns> ���s���ׂ����W(Vector2) </returns>
	const Math::Vector2 GetNaviPos(void);
private:


	void Update(float delta) override;

	
	/// <summary> A*�@���g���Čo�H�T�����s�� </summary>
	/// <param name="start"> �J�n�}�X�� </param>
	/// <param name="goal"> �ړI�̃}�X�� </param>
	void AstarSearch2(std::stop_token token, const Math::Vector2I& start, const Math::Vector2I& goal);

	ComponentID GetID() const override;

	Math::Vector2I goal_;

	// ���݂̖ړI�n�̃}�X��
	Vector2lVecItr nowNaviPos_;


	// �T������
	Vector2lVec destRoute_;

	// �}�b�v�Ɋւ���Tmx�I�u�W�F�N�g
	std::shared_ptr<TmxObj> tmxObj_;

	// �T����Ƃ��s���X���b�h
	std::jthread workThread_;

	// �~���[�e�b�N�X
	std::mutex mutex_;

	// ���
	NaviState state_;
};

