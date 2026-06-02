#include "../interfaces/IAppState.h"

class IdleState : public IAppState {
	public:
		void enter(AlarmContext* context) override {

		}

		void update(AlarmContext* context) override {
			// regular, don't need to check for alarms
			context->update();
		}

		void exit(AlarmContext* context) override {

		}
};
