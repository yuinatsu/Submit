#pragma once
#ifdef _DEBUG
#include <Windows.h>	// OutPutDebugStringn�����鏊
#include <iostream>		// �R���\�[���ɕW���o��
#include <sstream>		// string�̏�ʔŕ�����X�g���[��
#include <array>
#include <deque>
#include <functional>
#include <memory>		// ���j�[�N�|�C���^�Ƃ���
#include <thread>
#include <cassert>
#include <iomanip>
#include <mutex>
#include "Vector3.h"

// �N���G�C�g�֐����Ăԃ}�N���֐�
#define DebugStart(timeClass) (DebugClass::Create(timeClass))

// �f�o�b�O�̃Z�b�g�A�b�v������
#define DebugSetUp() (DebugClass::GetInstance().SetUp())

// �f�o�b�O�R���\�[���ɏo�͂���
#define DebugLog(...) (DebugClass::GetInstance().Log(true,"",__VA_ARGS__))

// visualstudio�̏o�̗͂��ɏo�͂���
#define DebugOutput(...) (DebugClass::GetInstance().Out(__VA_ARGS__))

// �G���[�Ƃ��ăR���\�[���ɏo�͂���
#define DebugErrorLog(...)(DebugClass::GetInstance().ErrorLog(true,"",__VA_ARGS__))

// ��ʂɕ����Ƃ��ĕ`�悷��
#define DebugDrawString(...)(DebugClass::GetInstance().DrawStr(__VA_ARGS__));

// �f�o�b�O���̕`��
#define DebugInfoDraw() (DebugClass::GetInstance().DebugDraw())

// ���݂̃X�N���[���Ƀf�o�b�O�p3D�̂��̂�`�悷��
#define DebugDraw3DScreen() (DebugClass::GetInstance().DebugDraw3D())

// �f�o�b�O�悤�ɃJ�v�Z����`�悷��
#define DebugDrawCapsule(top,btm,r,color) (DebugClass::GetInstance().DebugDraw3D(top,btm,r,color))

// �f�o�b�O�悤�ɋ��̂�`�悷��
#define DebugDrawSphere(pos, r, color) (DebugClass::GetInstance().DebugDraw3D(pos,r,color))

// �f�o�b�O�悤�ɐ���`�悷��
#define DebugDrawLine(start, end, color) (DebugClass::GetInstance().DebugDraw3D(start,end,color))

#define DebugDrawCube(pos1, pos2, color) (DebugClass::GetInstance().DebugDraw3DCube(pos1, pos2, color));

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

	

	// ������
	template<class T, class... A>
	void Log(bool flag,const T& t,const A&... a);

	
	// �G���[�p�̃��O
	template<class T, class... A>
	void ErrorLog(bool flag,const T& t, const A&... a);

	// VisualStudio�̏o�̗͂��ɏo�͂���
	template<class T>
	void Out(const T& t);

	template<class T, class... A>
	void Out(const T& t, const A&... a);
	

	// �f�o�b�O�p�̃X�N���[��
	void DebugDraw(void);

	// 3D�n�̃f�o�b�O�p�̕`������s����
	void DebugDraw3D(void);

	// ���̂̃f�o�b�O�p�`��
	void DebugDraw3D(const Vector3& pos, float r, int color);

	// �J�v�Z���̃f�o�b�O�p�`��
	void DebugDraw3D(const Vector3& top, const Vector3& btm, float r, int color);

	// ���̃f�o�b�O�p�`��
	void DebugDraw3D(const Vector3& start, const Vector3& end, int color);

	// box�̃f�o�b�O�`��
	void DebugDraw3DCube(const Vector3& pos1, const Vector3& pos2, int color);

	// ��ʂɕ������\������
	template<class T>
	void DrawStr(const T& t)
	{
		ss_ << t << std::endl;
	}

	template<class T, class... A>
	void DrawStr(const T& t, const A&... a)
	{
		ss_ << t;
		DrawStr(a...);
	}

private:

	// ���O�̃^�C�v
	enum class LogType
	{
		ALL,
		Log,
		Error
	};

	// �R���X�g���N�^
	DebugClass(TimeClass& timeClass);

	// �f�X�g���N�^
	~DebugClass();

	// ���O�̏����o�͂���
	void PrintLogInfo(void);

	// �G���[���O�̏����o�͂���
	void PrintErrorLogInfor(void);

	// �R���\�[���ɏo�͂���
	template<class T>
	void Log(bool flag, const T& t);

	// �G���[�p
	template<class T>
	void ErrorLog(bool flag,const T& t);

	// �f���[�^�[
	struct Deleter
	{
		void operator()(DebugClass* p)
		{
			delete p;
		}
	};

	

	// �W���o�͂��邽�߂̊֐��I�u�W�F�N�g
	struct LogPrinter
	{
		template<class T>
		void operator()(const T& t)
		{
			std::cout << t << std::endl;
		}

		template<class T, class... A>
		void operator()(const T& t, const A&... a)
		{
			std::count << t;
			LogPrinter()(a...);
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

	// �f�o�b�O�p�`��̕\���t���O
	bool debugDrawFlag_;

	std::array<char, 256> key_;
	std::array<char, 256> keyOld_;

	// ���O�o�͂̃^�C�v
	LogType logType_;

	TimeClass& time_;

	std::chrono::system_clock::time_point start_;

	// �~���[�e�b�N�X
	std::mutex mutex_;

	// 3D�n�f�o�b�O�����s���邽�߂̃L���[
	std::deque<std::function<void(void)>> draw3DQueue_;

	// ���݂�3D�n�f�o�b�O�`��̐�
	size_t nowDraw3D_;

	// ������`��p�̃L���[
	std::deque<std::function<void(std::stringstream&)>> drawStringQueue_;

#ifdef UNICODE
	std::wstringstream ss_;
#else
	std::stringstream ss_;
#endif

};

template<class T>
inline void DebugClass::Log(bool flag, const T& t)
{
	std::cout << (t) << std::endl;
}

template<class T, class ...A>
inline void DebugClass::Log(bool flag, const T& t, const A& ...a)
{
	// �f�o�b�O���[�h�̎�����
	if (logType_ == LogType::Error)
	{
		return;
	}
	if (flag)
	{
		// true�̎�(�ŏ��ɌĂ΂ꂽ��)���b�N���čċA�I��Log���Ă�
		mutex_.lock();
		PrintLogInfo();
		std::cout << t;
		Log(false, a...);

		// �ċA�Ăяo�����畜�A�����̂Ń��b�N����
		mutex_.unlock();
	}
	else
	{
		// false�̎�(2��ڈڍs)�o�͂���Log��
		std::cout << t;
		Log(false, a...);
	}
}

template<class T>
inline void DebugClass::ErrorLog(bool flag,const T& t)
{
	std::cout << (t) << std::endl;
}

template<class T, class ...A>
inline void DebugClass::ErrorLog(bool flag,const T& t, const A & ...a)
{
	if (logType_ == LogType::Log)
	{
		return;
	}
	if (flag)
	{
		mutex_.lock();
		PrintErrorLogInfor();
		std::cout << t;
		ErrorLog(false, a...);
		mutex_.unlock();
	}
	else
	{
		std::cout << t;
		ErrorLog(false, a...);
	}
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

#define DebugStart(timeClass) 

#define DebugSetUp() 

#define DebugLog(...)

#define DebugErrorLog(...)

#define DebugOutput(...) 

#define DebugInfoDraw() 

// �f�o�b�O�悤�ɃJ�v�Z����`�悷��
#define DebugDrawCapsule(top,btm,r,color)

// �f�o�b�O�悤�ɋ��̂�`�悷��
#define DebugDrawSphere(pos, r, color)

// �f�o�b�O�悤�ɐ���`�悷��
#define DebugDrawLine(start, end, color)

#define DebugDrawCube(pos1, pos2, color)

#define DebugDrawString(...)

#define DebugDraw3DScreen()

#define Assert(val,mess) (val)

#endif







