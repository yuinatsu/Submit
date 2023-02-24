#pragma once
#include <memory>
#include <list>
#include "Camera.h"
#include "SceneID.h"
//#include "Obj/Obj.h"			//class Obj;

class BaseScene;
using uniqueBaseScene = std::unique_ptr<BaseScene>;					// std::shared_ptr<BaseScene>

class Obj;
using ObjPtr = std::shared_ptr<Obj>;				// Obj�̃��j�[�N�|�C���^
using ObjList = std::list<ObjPtr>;					// ObjList

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual uniqueBaseScene Update(uniqueBaseScene own) = 0;					// own ... �������g�Ƃ����Ӗ�
	virtual SceneID GetSceneID() = 0;
	virtual void Draw();
	//
	virtual void DrawOwnScreen() = 0;					// �������z
private:
protected:
	virtual bool Init(void) = 0;
	ObjList::iterator AddObjList(ObjPtr&& objUni);
	bool ObjUpdate(void);
	bool ObjDraw(void);

	ObjList objList_;								// Obj���Ǘ�����List			//std::list<Obj*>		Obj�ɓo�^����̂͊ȒP�����폜����̂��߂�ǂ�����
	int screenScrID_;					// �p�������eScene�݂̂Ŏg�p����O���t�B�b�N�n���h��
	std::shared_ptr<Camera> camera_;

	friend class MapCtl;
};
