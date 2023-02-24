#include <DxLib.h>
#include "BaseScene.h"
#include "Obj/Obj.h"
#include "SceneMng.h"


BaseScene::BaseScene()
{
	screenScrID_ = MakeScreen(lpSceneMng.scnArea_.x, lpSceneMng.scnArea_.y, 255);					// �X�N���[���̍쐬
	camera_ = std::make_shared<Camera>();
	//auto tmp = objList_.begin();
}


BaseScene::~BaseScene()
{
}


uniqueBaseScene BaseScene::Update(uniqueBaseScene own)
{
	return uniqueBaseScene();
}


void BaseScene::Draw()
{
	DrawGraph(0, 0, screenScrID_, true);
	//DrawGraph((rand() % 5) - 2, (rand() % 5) - 2, screenScrID_, true);					// ��ʗh��i�V�тŏ�������j
}


void BaseScene::DrawOwnScreen()
{
}


bool BaseScene::Init(void)
{
	return false;
}


ObjList::iterator BaseScene::AddObjList(ObjPtr&& objUni)
{
	objList_.push_back(std::move(objUni));				// objList�ɗv�f��ǉ�				������I�u�W�F�N�g�̏����W�񂵂Ă���
	auto itr = objList_.end();							// objList��end()		itr ... iterator
	--itr;												// objList��end()�̈�O�̗v�f�i�v�f�I�Ɍ����΍Ō�j	// �O���Z
	return itr;											// return --objList_.end();		���2�s�𓝍������ꍇ�̕�
}


bool BaseScene::ObjUpdate(void)
{
	bool flag = false;						// |= ���g�������̂ŏ����l���O�ifalse�j�ɂ���K�v������

	// �͈�for��
	//------------
	for (auto& data : objList_)
	{
		flag |= data->Update(objList_);				// |= ... ���true����������ォ��false�����Ă�true�̂܂܂ɂȂ�		//flag = data->Update(); ... true�������Ă�����false�������true�̏�񂪂Ȃ��Ȃ�̂ō���͎g���Ȃ�

		// �ʉ�
		//-------
		//if (data->Update())				// Obj::Update�̕Ԃ�l��true���������flag��true����
		//{
		//	flag = true;
		//}
	}
	// itr���̂��ق����ꍇ�͂�������g�p
	//------------------------------------
	//for (auto itr = objList_.begin(); itr != objList_.end(); ++itr)			// obj�̃��X�g�̒��g����for��
	//{
	//	flag |= (*itr)->Update();									// itr�̒��̂��̒��ɓ���			// �ʉ� itr->get()->Update();
	//}

	if (camera_)							// �J�����̃G���v�e�B�`�F�b�N(������Γ���)
	{
		camera_->Update();
	}
	return flag;
}


bool BaseScene::ObjDraw(void)
{
	// �͈�for��
	//------------
	for (auto& data : objList_)
	{
		data->Draw();
	}
	return true;
}
