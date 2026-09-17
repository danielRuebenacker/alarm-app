#pragma once

class Presenter {
public:
	virtual ~Presenter() = default;
	// called periodically by the router's clock tick
	virtual void onTick() {}
};
