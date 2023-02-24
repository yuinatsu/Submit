#pragma once
#include "OptionBase.h"
#include <memory>


template<class T>
class MenuUICtrl;

class Controller;

template<class T>
class OptionWindow :
	public OptionBase
{
public:
	OptionWindow(Controller& controller) : OptionBase{ controller }{}

	virtual ~OptionWindow(){}

	
protected:

	
	std::unique_ptr<MenuUICtrl<T>> menuUICtrl_;
};


