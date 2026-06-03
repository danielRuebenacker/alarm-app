#pragma once
#include <map>
#include <string>
#include <memory>

#include "../interfaces/IScreen.h"

class UIManager {
	private:
		std::map<std::string, std::shared_ptr<IScreen>> screens;
		std::shared_ptr<IScreen> currentScreen;
		std::shared_ptr<IScreen> getScreen(std::string screenID);
	public:
		void registerScreen(std::string screenID, std::shared_ptr<IScreen> screen);
		void loadScreen(std::string screenID);
		void update();
};
