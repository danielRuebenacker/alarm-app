#include "../interfaces/IAppState.h"

class IdleState : public IAppState {
	public:
		void enter(AlarmContext* context) override {
			// show home screen
			context->getUI().loadScreen("home");
		}

		void update(AlarmContext* context) override {
			// regular, don't need to check for alarms
			context->getUI().update();
		}

		void exit(AlarmContext* context) override {
			// 
		}
};
