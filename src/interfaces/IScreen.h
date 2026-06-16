#pragma once
#include "IUIWidget.h"

class IScreen {
	public:
		virtual ~IScreen() = default;
		virtual void load() = 0;	// when screen initialised
		virtual void update() = 0;	// called periodically
		virtual void exit() = 0;	// cleaning up
									//
		// returns a pointer to content area container if this screen supports nesting
		virtual UIWidget* getContentContainer() { return nullptr; }
};
