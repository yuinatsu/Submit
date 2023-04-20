#pragma once
#ifdef _DEBUG
#include <Windows.h>	// OutPutDebugStringn�����鏊
#include <iostream>		// �R���\�[���ɕW���o��
#include <sstream>		// string�̏�ʔŕ�����X�g���[��
#include <array>
#include <memory>		// ���j�[�N�|�C���^�Ƃ���
#include <cassert>

// �N���G�C�g�֐����Ăԃ}�N���֐�
#define DebugStart(timeClass) (DebugClass::Create(timeClass))

// �f�o�b�O�̃Z�b�g�A�b�v������
#define DebugSetUp() (DebugClass::GetInstance().SetUp())

// �f�o�b�O�R���\�[���ɏo�͂���
#define DebugLog(...) (DebugClass::GetInstance().Log(__VA_ARGS__))

// visualstudio�̏o�̗͂��ɏo�͂���
#define DebugOutput(...) (DebugClass::GetInstance().Out(__VA_ARGS__))

// �f�o�b�O���̕`��
#define DebugInfoDraw() (DebugClass::GetInstance().DebugDraw())

// �����assert�݂����Ȃ�
#define Assert(val,mess) ( (val) || (_wassert(_CRT_WIDE(mess), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)),0))

class TimeClass;

class DebugClass
{
public:

	// �N���G�C�g�֐�
	static void Create(TimeClass& timeClass);

	// ���̂��擾����
	static DebugClass& GetInstance(void);

	void SetUp(const int& targetSc = 0xfffffffe);

	// �R���\�[���ɏo�͂���
	template<class T>
	void Log(const T& t);

	template<class T, class... A>
	void Log(const T& t, const A&... a);


	// VisualStudio�̏o�̗͂��ɏo�͂���
	template<class T>
	void Out(const T& t);

	template<class T, class... A>
	void Out(const T& t, const A&... a);


	// �f�o�b�O�p�̃X�N���[��
	void DebugDraw(void);


private:
	// �R���X�g���N�^
	DebugClass(TimeClass& timeClass);

	// �f�X�g���N�^
	~DebugClass();

	// �f���[�^�[
	struct Deleter
	{
		void operator()(DebugClass* p)
		{
			delete p;
		}
	};

	// ���̂����j�[�N�|�C���^�Ŏ���
	static std::unique_ptr<DebugClass, Deleter> instance_;


	// �t�@�C���|�C���^
	FILE* conFp_;

	// �������Ƃ̃X�N���[�����w��ł���悤��
	int targetSc_;

	// �f�o�b�O�p�̃X�N���[���̃n���h��
	int screen_;

	// �X�N���[���̍����ƕ�
	int scW, scH;

	// �f�o�b�O���̕\���t���O
	bool infoDrawFlag_;

	// ��~�t���O
	bool stopFlag_;

	std::array<char, 256> key_;
	std::array<char, 256> keyOld_;

	TimeClass& time_;

};

template<class T>
inline void DebugClass::Log(const T& t)
{
	std::cout << t << std::endl;
}

template<class T, class ...A>
inline void DebugClass::Log(const T& t, const A & ...a)
{
	// �f�o�b�O���[�h�̎�����
	std::cout << t;
	Log(a...);
}

template<class T>
inline void DebugClass::Out(const T& t)
{
	// �f�o�b�O���[�h�̎�����
#ifdef UNICODE
	// ���j�R�[�h�̎�������
	std::wstringstream ss;
	ss << t << std::endl;
	OutputDebugString(ss.str().c_str());
#else
	std::stringstream ss(t);
	OutputDebugString(ss.str().c_str());
#endif
}

template<class T, class ...A>
inline void DebugClass::Out(const T& t, const A & ...a)
{
#ifdef UNICODE
	// ���j�R�[�h�̎�������
	std::wstringstream ss;
	ss << t;
	OutputDebugString(ss.str().c_str());
	Out(a...);
#else
	std::stringstream ss(t);
	OutputDebugString(ss.str().c_str());
	Out(a...);
#endif
}





#else

#define DebugStart() 

#define DebugSetUp() 

#define DebugLog(...)

#define DebugOutput(...) 

#define DebugInfoDraw(delta) 

#define Assert(val,mess) (val)

#endif







