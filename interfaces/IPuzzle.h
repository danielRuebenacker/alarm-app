#pragma once

class IPuzzle {
	public:
		virtual ~IPuzzle() = default;
		// draw to the screen or print on terminal
		virtual void display() = 0;
		// get UI touch or terminal input
		virtual bool verifySolution();
};
