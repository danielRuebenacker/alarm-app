#include "UIManager.h"

std::shared_ptr<IScreen> UIManager::getScreen(std::string screenID) {
	auto it = screens.find(screenID);
	if (it != screens.end()) {
		// return value (screen)
		return it->second;
	}
	return nullptr;
}

void UIManager::registerScreen(std::string screenID, std::shared_ptr<IScreen> screen) {
	screens.insert({screenID, screen});
}

void UIManager::loadScreen(std::string screenID) {
	std::shared_ptr<IScreen> newScreen = getScreen(screenID);
	// nullptr check!!
	if (!newScreen) return;

	newScreen->load();
	currentScreen = newScreen;
}
