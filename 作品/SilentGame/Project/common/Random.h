#pragma once
#include <random>


// ๐ตคNX
class Random
{
public:
	Random();

	/// <summary> Cำฬmฆลtrue๐ิท </summary>
	/// <param name="p"> mฆ </param>
	/// <returns> trueฉfalse </returns>
	bool GetRandom(const double& p);

	/// <summary> ๊่mฆลCำฬออฬฎ๐ิท </summary>
	/// <param name="minVal"> ลฌl </param>
	/// <param name="maxVal"> ลๅl </param>
	/// <returns> Cำฬออฬฎ </returns>
	int GetRandom(const int& minVal, const int& maxVal);

	/// <summary> ๊่mฆลCำฬออฬฎฌ_(float)๐ิท </summary>
	/// <param name="minVal"> ลฌl </param>
	/// <param name="maxVal"> ลๅl </param>
	/// <returns> Cำฬออฬฎฌ_(float) </returns>
	float GetRandom(const float& minVal, const float& maxVal);

	/// <summary> ๊่mฆลCำฬออฬฎฌ_(double)๐ิท </summary>
		/// <param name="minVal"> ลฌl </param>
		/// <param name="maxVal"> ลๅl </param>
		/// <returns> Cำฬออฬฎฌ_(double) </returns>
	double GetRandom(const double& minVal, const double& maxVal);
private:
	std::mt19937_64 mt_;
};

