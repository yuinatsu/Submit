#pragma once

class Controller;

class OptionBase
{
public:
	OptionBase(Controller& controller) : controller_{ controller }, time_{0.0f}, isDrawInput_{ true }  {}

	virtual ~OptionBase() {}

	virtual bool Update(float delta) = 0;
	virtual void Draw(void) = 0;
	/// <summary> �����`�悷�邩? </summary>
	/// <param name=""> ����Ƃ�true���Ȃ��Ƃ�false </param>
	/// <returns></returns>
	bool IsDrawInput(void)
	{
		return isDrawInput_;
	}

protected:
	// �����`�悷�邩?
	bool isDrawInput_;
	Controller& controller_;
	float time_;
};

