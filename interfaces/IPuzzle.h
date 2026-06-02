#pragma once

class IPuzzle {
	public:
		virtual ~IPuzzle() = default;
		virtual void display() = 0;
		virtual bool verifySolution();
};
