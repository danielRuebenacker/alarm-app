#include "../interfaces/IAppState.h"

class PrimedState : public IAppState {
	public:
		void enter(AlarmContext* context) override {
			// show primed notification?
		}

		void update(AlarmContext* context) override {
			// regular, don't need to check for alarms
			context->getUI().update();
		}

		void exit(AlarmContext* context) override {
			// alarm disabled/canceled notification?
		}
};
