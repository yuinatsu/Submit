#pragma once
#include <memory>
#include <list>
#include "Camera.h"
#include "SceneID.h"
//#include "Obj/Obj.h"			//class Obj;

class BaseScene;
using uniqueBaseScene = std::unique_ptr<BaseScene>;					// std::shared_ptr<BaseScene>

class Obj;
using ObjPtr = std::shared_ptr<Obj>;				// Objのユニークポインタ
using ObjList = std::list<ObjPtr>;					// ObjList

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual uniqueBaseScene Update(uniqueBaseScene own) = 0;					// own ... 自分自身という意味
	virtual SceneID GetSceneID() = 0;
	virtual void Draw();
	//
	virtual void DrawOwnScreen() = 0;					// 純粋仮想
private:
protected:
	virtual bool Init(void) = 0;
	ObjList::iterator AddObjList(ObjPtr&& objUni);
	bool ObjUpdate(void);
	bool ObjDraw(void);

	ObjList objList_;								// Objを管理するList			//std::list<Obj*>		Objに登録するのは簡単だが削除するのがめんどくさい
	int screenScrID_;					// 継承される各Sceneのみで使用するグラフィックハンドル
	std::shared_ptr<Camera> camera_;

	friend class MapCtl;
};
