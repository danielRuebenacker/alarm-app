class AlarmContext;

class IAppState {
	public:
		virtual ~IAppState() = default;
		virtual void enter(AlarmContext* context) = 0;
		virtual void update(AlarmContext* context) = 0;
		virtual void exit(AlarmContext* context) = 0;
};
