#include "TextScroll.h"
#include "Quaternion.h"
#include "AsoUtility.h"
#include "SceneManager.h"


TextScroll::TextScroll(SceneManager* sceneManager):
	sceneManager_(sceneManager)
{
	type_ = TYPE::TITLE;
	images_.fill(-1);
}

TextScroll::~TextScroll() = default;

void TextScroll::Update(void)
{
}

void TextScroll::Draw(void)
{
	Quaternion qua;
	qua = Quaternion::LookRotation(VAdd(AsoUtility::AXIS_Z, AsoUtility::AXIS_X));
	qua = qua.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(30.0f), AsoUtility::AXIS_Z));
	for (auto& msg : textMap_[type_])
	{
		VECTOR pos = msg.pos;
		for (const auto& mes : msg.message)
		{
			DrawBillboard3D(pos,5.0f,5.0f, IMAGE_SIZE_X,0.0f, images_[mes],true);
			pos.x += IMAGE_SIZE_X;
		}
		msg.pos = VAdd(msg.pos, VScale(qua.ToEuler() , sceneManager_->GetDeltaTime() * 100.0f));
	}
}

void TextScroll::DrawGrid(void)
{
}

void TextScroll::Release(void)
{
}

void TextScroll::Init(void)
{
	LoadDivGraph("Image/Alphabet.png", NUM_IMAGE, IMAGE_DIV_X, IMAGE_DIV_Y, IMAGE_SIZE_X, IMAGE_SIZE_Y, &images_[0], true);
	msgInfos_.emplace_back(MakeMsgInfo("start to space", msgInfos_.size()));
	msgInfos_.emplace_back(MakeMsgInfo("AAAAAAAAAAAA", msgInfos_.size()));
	msgInfos_.emplace_back(MakeMsgInfo("ZZZZZZZZZZZZ", msgInfos_.size()));
	textMap_.try_emplace(TYPE::TITLE, msgInfos_);
}

TextScroll::MsgInfo TextScroll::MakeMsgInfo(std::string msg, int mapCount)
{
	MsgInfo reInfo;
	int ascii = 0;
	std::vector<int> message;
	for (const auto& ms:msg)
	{
		ascii = ms;
		if (ascii == 32)
		{
			ascii = 52;
		}
		if (ascii == 46)
		{
			ascii = 53;
		}
		if (ascii == 44)
		{
			ascii = 54;
		}
		if (ascii >= 65 && ascii <= 90)
		{
			ascii -= 65;
		}
		if (ascii >= 97)
		{
			ascii -= 71;
		}
		
		message.emplace_back(ascii);
	}
	reInfo.pos = { 0.0f,static_cast<float>(-IMAGE_SIZE_Y * mapCount) ,0.0f };
	reInfo.message = message;
	return reInfo;
}
