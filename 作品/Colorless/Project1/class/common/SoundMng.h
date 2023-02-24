#pragma once

class SoundMng
{
public:
	SoundMng();
	~SoundMng();
	void VolUpdate(int vol);
private:
	int vol_;
};

