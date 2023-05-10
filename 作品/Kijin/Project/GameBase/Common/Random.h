#pragma once
#include <random>


// �����������N���X
class Random
{
public:
	Random();

	/// <summary> �C�ӂ̊m����true��Ԃ� </summary>
	/// <param name="p"> �m�� </param>
	/// <returns> true��false </returns>
	bool GetRandom(const double& p);

	/// <summary> ���m���ŔC�ӂ͈̔͂̐�����Ԃ� </summary>
	/// <param name="minVal"> �ŏ��l </param>
	/// <param name="maxVal"> �ő�l </param>
	/// <returns> �C�ӂ͈̔͂̐��� </returns>
	int GetRandom(const int& minVal, const int& maxVal);

	/// <summary> ���m���ŔC�ӂ͈̔͂̕��������_��(float)��Ԃ� </summary>
	/// <param name="minVal"> �ŏ��l </param>
	/// <param name="maxVal"> �ő�l </param>
	/// <returns> �C�ӂ͈̔͂̕��������_��(float) </returns>
	float GetRandom(const float& minVal, const float& maxVal);

	/// <summary> ���m���ŔC�ӂ͈̔͂̕��������_��(double)��Ԃ� </summary>
		/// <param name="minVal"> �ŏ��l </param>
		/// <param name="maxVal"> �ő�l </param>
		/// <returns> �C�ӂ͈̔͂̕��������_��(double) </returns>
	double GetRandom(const double& minVal, const double& maxVal);
private:
	std::mt19937_64 mt_;
};

