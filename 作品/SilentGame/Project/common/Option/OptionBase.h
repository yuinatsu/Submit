#pragma once

class Controller;

class OptionBase
{
public:
	OptionBase(Controller& controller) : controller_{ controller }, time_{0.0f}, isDrawInput_{ true }  {}

	virtual ~OptionBase() {}

	virtual bool Update(float delta) = 0;
	virtual void Draw(void) = 0;
	/// <summary> 操作を描画するか? </summary>
	/// <param name=""> するときtrueしないときfalse </param>
	/// <returns></returns>
	bool IsDrawInput(void)
	{
		return isDrawInput_;
	}

protected:
	// 操作を描画するか?
	bool isDrawInput_;
	Controller& controller_;
	float time_;
};

