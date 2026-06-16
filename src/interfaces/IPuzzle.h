#pragma once
#include "IScreen.h"

class IPuzzle {
	public:
		virtual ~IPuzzle() = default;
		// draw to the screen or print on terminal (within a wrapper object)
		virtual void display(UIWidget* parent) = 0;
		// get UI touch or terminal input
		virtual bool verifySolution();
};
