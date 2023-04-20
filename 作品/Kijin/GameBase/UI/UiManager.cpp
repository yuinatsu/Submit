#include <Dxlib.h>
#include "UiManager.h"
#include "../Object/ObjectManager.h"
#include "LoadIndicatorUi.h"
#include "../Component/Behavior/PlayerBehavior.h"
#include "ComboUi.h"
#include "Gage.h"
#include "Image.h"
#include "Button.h"
#include "Cursor.h"
#include "../SceneManager.h"
#include "../Common/ResourceMng.h"
#include "Slider.h"
#include "InputSetButton.h"
#include "../Scene/OptionScene.h"
#include "../Scene/PauseScene.h"
#include "../Scene/DialogScene.h"
#include "../Scene/SelectScene.h"
#include "TutorialMessage.h"
#include "../Common/Utility.h"
#include "../Common/ThreadPool.h"
#include "SwitchButton.h"
#include "../Scene/ResultScene.h"
#include "MiniMap.h"

#include "../Common/Debug.h"



constexpr auto screenHalf{ SceneManager::screenSize_<float> / 2.0f };
constexpr auto menuSizeY{ 71.0f };

constexpr char sig[] = { 'u','i','d','a' };

constexpr float versions{ 0.0f };

struct UiDataHeader
{
	char sig[4];
	float version;
	int ckData;
};

// Ui�̎��
enum class UiType
{
	Image,				// �摜
	ImageArray,			// DivGraph�̔z���
	Button,				// �{�^��
	Slider,				// �X���C�_�[
	InputSet,			// ���̓Z�b�g
	Gage,				// �Q�[�W
	Combo,				// �R���{
	LoadIndicator,		// ���[�h���̕\��
	SwitchButton,		// �I���I�t�ł���{�^��
	MinMap
};

// 
enum class ButtonFuncID
{
	Tutorial,       // �`���[�g���A��
	Play,           // �Q�[���J�n
	SelectOption,   // �Z���N�g�V�[������I�v�V����
	SelectDialog,   // �Z���N�g�V�[������Q�[���I���_�C�A���O
	PauseBack,      // �|�[�Y�V�[���߂�
	PauseOption,    // �|�[�Y�V�[������I�v�V����
	PauseDialog,    // �|�[�Y�V�[������I���_�C�A���O
	OptionBack,     // �I�v�V�����߂�
	OptionOk,       // �I�v�V����OK
	DialogOk,       // �_�C�A���OOK
	DialogCancel    // �_�C�A���O�L�����Z��
};


UiManager::UiManager(const std::filesystem::path& path, bool isAsync, bool isTutorial, bool isCreateCursor)
{
	isLoaded_.store(false);
	if (isTutorial)
	{
		uiList_.emplace_back(std::make_unique<TutorialMessage>());
	}
	if (!isAsync)
	{
		InitUi(path, isCreateCursor);
		isLoaded_.store(true);
		return;
	}
	// �X���b�h�v�[���N���X�g���ă��[�h����
	Thread.Add([this, path, isCreateCursor]() { InitUi(path, isCreateCursor); isLoaded_.store(true); });
	
}


bool UiManager::InitUi(const std::filesystem::path& path, bool isCreateCursor)
{
	std::ifstream file{ path , std::ios::binary};

	if (!file)
	{
		DebugLog("�t�@�C�����J���܂���ł���");
		return false;
	}

	UiDataHeader header{};
	file.read(reinterpret_cast<char*>(&header), sizeof(header));
	
	if (versions != header.version)
	{
		// �o�[�W��������v���Ă��Ȃ��Ƃ�
		DebugLog("�t�@�C���̃o�[�W��������v���Ă��܂���");
		return false;
	}

	for (int i = 0; i < 4; i++)
	{
		if (sig[i] != header.sig[i])
		{
			// �V�O�l�`������v���ĂȂ��Ƃ�
			DebugLog("�t�@�C���̃V�O�l�`��������������܂���");
			return false;
		}
	}

	// �����p�֐��̃}�b�v
	const std::map<UiType, void(UiManager::*)(std::ifstream&, int)> createFuncs
	{
		{UiType::Button, &UiManager::CreateButton},
		{UiType::Image, &UiManager::CreateImage},
		{UiType::ImageArray, &UiManager::CreateImageArray},
		{UiType::Slider, &UiManager::CreateSlider},
		{UiType::InputSet, &UiManager::CreateInputSet},
		{UiType::LoadIndicator, &UiManager::CreateLoadIndicator},
		{UiType::Gage, &UiManager::CreateGage},
		{UiType::Combo, &UiManager::CreateCombo},
		{UiType::SwitchButton, &UiManager::CreateSwitchButton},
		{UiType::MinMap, &UiManager::CreateMinMap}
	};

	while (true)
	{
		// Ui�̎�ނ�ǂݎ��
		UiType type;
		file.read(reinterpret_cast<char*>(&type), sizeof(type));
		
		if (file.eof())
		{
			// �t�@�C���̖��[�܂œǂݏI����Ă�����
			break;
		}

		// ���̎�ނ�UI�̐����擾
		int num{ 0 };
		file.read(reinterpret_cast<char*>(&num), sizeof(num));

		
		// ��������
		if (createFuncs.contains(type))
		{
			(this->*createFuncs.at(type))(file, num);
		}

	}

	if (isCreateCursor)
	{
		// �J�[�\����������Ƃ�
		uiList_.emplace_back(std::make_unique<Cursor>());
	}
	return true;
}

bool UiManager::IsLoaded(void)
{
	return isLoaded_.load();
}


void UiManager::CreateButton(std::ifstream& file, int num)
{
	// �{�^���̏���
	std::vector<std::function<void(BaseScene&)>> funcs{
		[](BaseScene& scene) { static_cast<SelectScene&>(scene).Tutorial(); },
		[](BaseScene& scene) { static_cast<SelectScene&>(scene).Play(); },
		[](BaseScene& scene) { static_cast<SelectScene&>(scene).Option(); },
		[](BaseScene& scene) { static_cast<SelectScene&>(scene).Dialog(); },
		[](BaseScene& scene) {  static_cast<PauseScene&>(scene).BackGame(); },
		[](BaseScene& scene) { static_cast<PauseScene&>(scene).Option(); },
		[](BaseScene& scene) { static_cast<PauseScene&>(scene).Dialog(); },
		[](BaseScene& scene) {  static_cast<OptionScene&>(scene).Close(); },
		[](BaseScene& scene) { static_cast<OptionScene&>(scene).Save(); static_cast<OptionScene&>(scene).Close(); },
		[](BaseScene& scene) { static_cast<DialogScene&>(scene).Ok(); },
		[](BaseScene& scene) {static_cast<DialogScene&>(scene).Cancel(); },
		[](BaseScene& scene) { scene.ChangeSceneID(SceneID::Game); },
		[](BaseScene& scene) { scene.ChangeSceneID(SceneID::Select); },
		[](BaseScene& scene) { scene.ChangeSceneID(SceneID::Dialog); },
		[](BaseScene& scene) { static_cast<OptionScene&>(scene).Default(); },
		[](BaseScene& scene) { static_cast<ResultScene&>(scene).NextStage(); }
	};

	// ������ID
	int funcID{0};

	// �摜��
	std::string imgName;

	// �g�摜��
	std::string frameName;

	// ���W
	Vector2 pos;
	for (int i = 0; i < num; i++)
	{
		// ������ID��ǂݎ��
		file.read(reinterpret_cast<char*>(&funcID), sizeof(funcID));

		// �摜����ǂݎ��
		imgName = ReadString(file);

		// �g�摜����ǂݎ��
		frameName = ReadString(file);

		// ���W��ǂݎ��
		file.read(reinterpret_cast<char*>(&pos), sizeof(pos));

		// �{�^������
		uiList_.emplace_back(std::make_unique<Button>(
			funcs[funcID],
			imgName,
			frameName,
			pos
		));
	}
}

void UiManager::CreateImage(std::ifstream& file, int num)
{
	// ���W
	Vector2 pos;

	// �摜��
	std::string imgName;

	// �\���̎��
	ViewType viewType{ 0 };
	for (int i = 0; i < num; i++)
	{
		// ���W��ǂݎ��
		file.read(reinterpret_cast<char*>(&pos), sizeof(pos));

		// �摜����ǂݎ��
		imgName = ReadString(file);

		// �\���̎�ނ�ǂݎ��
		file.read(reinterpret_cast<char*>(&viewType), sizeof(viewType));

		// �摜�𐶐�
		uiList_.emplace_back(
			std::make_unique<Image>(imgName,pos, viewType)
		);
	}
}

void UiManager::CreateSlider(std::ifstream& file, int num)
{
	std::vector< std::pair<std::function<void(const float)>, float>> funcs
	{
		{[](const float v) { lpSooundPross.SetSEVolumeUserSet(v); },lpSooundPross.GetSEVolumeUserSet()},
		{[](const float v) { lpSooundPross.SetBGMVolumeUserSet(v); }, lpSooundPross.GetBGMVolumeUserSet()},
		{[](const float v) { lpConfigMng.SetCameraSpeed(v); },lpConfigMng.GetCameraSpeed()} 
	};

	// ������ID
	int funcID{ 0 };

	// �o�[�̉摜��
	std::string barImgName;

	// �^�u�̉摜��
	std::string tabImgName;

	std::string valueImgName;

	// �J�n���̒l
	float startValue{ 0.0f };

	// ���W
	Vector2 pos;
	for (int i = 0; i < num; i++)
	{
		// �����̎�ނ�ǂݎ��
		file.read(reinterpret_cast<char*>(&funcID), sizeof(funcID));

		// �o�[�̉摜����ǂݎ��
		barImgName = ReadString(file);

		// �^�u�̉摜����ǂݎ��
		tabImgName = ReadString(file);

		// �l�̕����̉摜����ǂݎ��
		valueImgName = ReadString(file);

		// ���W��ǂݎ��
		file.read(reinterpret_cast<char*>(&pos), sizeof(pos));

		// �X���C�_�[����
		uiList_.emplace_back(std::make_unique<Slider>(
			funcs[funcID].first,
			barImgName,
			tabImgName,
			valueImgName,
			funcs[funcID].second, pos
		));
	}
}

void UiManager::CreateInputSet(std::ifstream& file, int num)
{
	// ���͂�ID
	InputID id;

	// ���W
	Vector2 pos;
	for (int i = 0; i < num; i++)
	{
		// ���͂̎�ނ�ǂݎ��
		file.read(reinterpret_cast<char*>(&id), sizeof(id));

		// ���W��ǂݎ��
		file.read(reinterpret_cast<char*>(&pos), sizeof(pos));

		// ���͐ݒ��UI�𐶐�
		uiList_.emplace_back(std::make_unique<InputSetButton>(id,pos));
	}
}

void UiManager::CreateLoadIndicator(std::ifstream& file, int num)
{
	// �摜��1
	std::string imgName1;

	// �摜��2
	std::string imgName2;

	// ���W
	Vector2 pos;
	for (int i = 0; i < num; i++)
	{
		// �摜��1��ǂݎ��
		imgName1 = ReadString(file);

		// �摜��2��ǂݎ��
		imgName2 = ReadString(file);

		// ���W��ǂݎ��
		file.read(reinterpret_cast<char*>(&pos), sizeof(pos));

		// ���[�h�̕\���𐶐�
		uiList_.emplace_back(std::make_unique<LoadIndicatorUi>(imgName1,pos,imgName2));
	}
}

void UiManager::CreateGage(std::ifstream& file, int num)
{
	std::vector< std::function<float(ObjectManager&)>> funcs
	{
		[](ObjectManager& objMng) {
				auto player = objMng.GetComponent<PlayerBehavior>(objMng.GetPlayerID());
				auto [value, minmax] = player->GetGaugeValue(UiID::Hp);
				return value;
			},
		[](ObjectManager& objMng) {
				auto player = objMng.GetComponent<PlayerBehavior>(objMng.GetPlayerID());
				auto [value, minmax] = player->GetGaugeValue(UiID::Skill);
				return value;
			},
		[](ObjectManager& objMng) {
			auto player = objMng.GetComponent<PlayerBehavior>(objMng.GetPlayerID());
			auto [value, minmax] = player->GetGaugeValue(UiID::Stamina);
			return value;
		}
	};

	// �����̎��
	int funcID{ 0 };

	// �摜��
	std::string imgName;

	// �g�摜��
	std::string frameName;

	// �J�n���̒l
	float startValue{ 0.0f };

	// ���W
	Vector2 pos;
	for (int i = 0; i < num; i++)
	{
		// �����̎��
		file.read(reinterpret_cast<char*>(&funcID), sizeof(funcID));

		// �摜����ǂݎ��
		imgName = ReadString(file);

		// �g�摜����ǂݎ��
		frameName = ReadString(file);

		// �J�n���̒l��ǂݎ��
		file.read(reinterpret_cast<char*>(&startValue), sizeof(startValue));

		// ���W��ǂݎ��
		file.read(reinterpret_cast<char*>(&pos), sizeof(pos));

		// �Q�[�W�𐶐�
		uiList_.emplace_back(
			std::make_unique<Gage>(
				funcID, funcs[funcID],imgName,frameName,startValue, pos
				));
	}
}

void UiManager::CreateImageArray(std::ifstream& file, int num)
{
	// �摜��
	std::string imageName;

	// ������
	Vector2I div;

	// �����T�C�Y
	Vector2I size;

	// ���W
	Vector2 pos;

	// �C���^�[�o���l
	float interval{0.0f};
	for (int i = 0; i < num; i++)
	{
		// �摜����ǂݎ��
		imageName = ReadString(file);

		// ��������ǂݎ��
		file.read(reinterpret_cast<char*>(&div), sizeof(div));

		// �����T�C�Y��ǂݎ��
		file.read(reinterpret_cast<char*>(&size), sizeof(size));

		// ���W��ǂݎ��
		file.read(reinterpret_cast<char*>(&pos), sizeof(pos));

		// �C���^�[�o����ǂݎ��
		file.read(reinterpret_cast<char*>(&interval), sizeof(interval));

		// DivGraph���g�����摜�𐶐�
		uiList_.emplace_back(std::make_unique<Image>(
				imageName,div,size,pos,Vector2{0.0f, interval}
		));
	}
}

void UiManager::CreateCombo(std::ifstream& file, int num)
{
	// ���l�摜��
	std::string numImgName;

	// ������
	Vector2I div;

	// �����T�C�Y
	Vector2I size;

	// �摜��
	std::string imgName;

	// �g�摜��
	std::string frameName;

	// �J�n���̒l
	float startValue{0.0f};

	// ���W
	Vector2 pos;

	// �C���^�[�o��
	Vector2 inerval;
	for (int i = 0; i < num; i++)
	{
		// ���l�摜����ǂݎ��
		numImgName = ReadString(file);

		// ��������ǂݎ��
		file.read(reinterpret_cast<char*>(&div), sizeof(div));

		// �T�C�Y��ǂݎ��
		file.read(reinterpret_cast<char*>(&size), sizeof(size));

		// �摜����ǂݎ��
		imgName = ReadString(file);

		// �g�摜����ǂݎ��
		frameName = ReadString(file);

		// �J�n���̒l��ǂݎ��
		file.read(reinterpret_cast<char*>(&startValue), sizeof(startValue));

		// ���W��ǂݎ��
		file.read(reinterpret_cast<char*>(&pos), sizeof(pos));

		// �C���^�[�o����ǂݎ��
		file.read(reinterpret_cast<char*>(&inerval), sizeof(inerval));

		// �R���{�𐶐�
		uiList_.emplace_back(
			std::make_unique<ComboUi>(
				numImgName,
				div,
				size,
				imgName,
				pos,
				inerval
				));
	}
}

void UiManager::CreateSwitchButton(std::ifstream& file, int num)
{
	// �{�^���̏���
	std::vector<std::pair<SwitchButton::UpdateFunc,SwitchButton::PushFunc>> funcs{
		{[](BaseScene& scene) {  return static_cast<OptionScene&>(scene).IsPeMono();  },[](BaseScene& scene, bool isOn) { static_cast<OptionScene&>(scene).SetPeMono(isOn);  } },
		{[](BaseScene& scene) {  return static_cast<OptionScene&>(scene).IsPeFog();  },[](BaseScene& scene, bool isOn) { static_cast<OptionScene&>(scene).SetPeFog(isOn);  } }
	};

	// ������ID
	int funcID{ 0 };

	// �I���̎��̉摜��
	std::string onImgName;

	// �I�t�̎��̉摜��
	std::string offImgName;

	// �g�摜��
	std::string frameName;

	// ���W
	Vector2 pos;

	for (int i = 0; i < num; i++)
	{
		// ������ID��ǂݎ��
		file.read(reinterpret_cast<char*>(&funcID), sizeof(funcID));

		// �I���̎��̉摜����ǂݎ��
		onImgName = ReadString(file);

		// �I�t�̎��̉摜����ǂݎ��
		offImgName = ReadString(file);

		// �g�摜����ǂݎ��
		frameName = ReadString(file);

		// ���W��ǂݎ��
		file.read(reinterpret_cast<char*>(&pos), sizeof(pos));

		// �{�^������
		uiList_.emplace_back(std::make_unique<SwitchButton>(
			funcs[funcID].second,
			funcs[funcID].first,
			onImgName,
			offImgName,
			frameName,
			pos
			));
	}
}

void UiManager::CreateMinMap(std::ifstream& file, int num)
{
	for (int i = 0; i < num; i++)
	{
		uiList_.emplace_back(std::make_unique<MiniMap>(ReadString(file)));
	}
}


UiManager::~UiManager()
{
	uiList_.clear();
}


void UiManager::Begin(BaseScene& scene)
{
	for (auto& ui : uiList_)
	{
		ui->Loaded(scene);
	}
}

void UiManager::Update(float delta, BaseScene& scene, ObjectManager& obj, Controller& controller)
{
	if (scene.GetID() == SceneID::Game)
	{
		auto player = obj.GetComponent<PlayerBehavior>(obj.GetPlayerID());
		if (!player.IsActive())
		{
			uiList_.clear();
		}
	}

	for (auto& ui : uiList_)
	{
		ui->Update(delta,scene, obj, controller);
	}

	auto cursor = std::find_if(uiList_.begin(), uiList_.end(), [](std::unique_ptr<UiBase>& ui) { return ui->GetUIID() == UiID::Cursor; });
	if (cursor != uiList_.end())
	{
		static_cast<Cursor*>((*cursor).get())->Check(uiList_, scene,controller);
	}
}

void UiManager::Draw(int mainScr)
{
	for (auto& ui : uiList_)
	{
		ui->Draw(mainScr);
	}
}
