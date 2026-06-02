#pragma once
#include <map>
#include <string>
#include <memory>

#include "../interfaces/IScreen.h"

class UIManager {
	private:
		std::map<std::string, std::shared_ptr<IScreen>> screens;
		std::unique_ptr<IScreen> currentScreen;
	public:
		void registerScreen(std::string screenID, std::shared_ptr<IScreen> screen);
		std::shared_ptr<IScreen> getScreen(std::string screenID);
		void update();
};
