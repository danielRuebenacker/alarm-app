#include "../interfaces/IAppState.h"

class IdleState : public IAppState {
	public:
		void update(AlarmContext* context) override {
			// regular, don't need to check for alarms
		}
};
