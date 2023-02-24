#pragma once

class Controller;

class OptionBase
{
public:
	OptionBase(Controller& controller) : controller_{ controller }, time_{0.0f}, isDrawInput_{ true }  {}

	virtual ~OptionBase() {}

	virtual bool Update(float delta) = 0;
	virtual void Draw(void) = 0;
	/// <summary> ‘€ì‚ğ•`‰æ‚·‚é‚©? </summary>
	/// <param name=""> ‚·‚é‚Æ‚«true‚µ‚È‚¢‚Æ‚«false </param>
	/// <returns></returns>
	bool IsDrawInput(void)
	{
		return isDrawInput_;
	}

protected:
	// ‘€ì‚ğ•`‰æ‚·‚é‚©?
	bool isDrawInput_;
	Controller& controller_;
	float time_;
};

