#pragma once
#include <array>
#include <vector>
#include <string>
#include <map>
#include <DxLib.h>

class SceneManager;

class TextScroll
{
public:

	static constexpr int IMAGE_DIV_X = 10;
	static constexpr int IMAGE_DIV_Y = 6;
	static constexpr int NUM_IMAGE = IMAGE_DIV_X * IMAGE_DIV_Y;
	static constexpr int IMAGE_SIZE_X = 32;
	static constexpr int IMAGE_SIZE_Y = 32;
	enum class TYPE
	{
		TITLE,	
	};
	struct MsgInfo
	{
		VECTOR pos;
		std::vector<int> message;
	};
	TextScroll(SceneManager* sceneManager);
	~TextScroll();
	void Update(void);
	void Draw(void);
	void DrawGrid(void);
	void Release(void);
	void Init(void);
private:
	SceneManager* sceneManager_;
	std::array<int, NUM_IMAGE> images_;
	std::map<TYPE, std::vector<MsgInfo>> textMap_;
	std::vector<MsgInfo> msgInfos_;
	TYPE type_;
	MsgInfo MakeMsgInfo(std::string msg, int mapCount);

};

