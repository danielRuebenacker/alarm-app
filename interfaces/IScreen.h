class IScreen {
	public:
		virtual ~IScreen() = default;
		virtual void load() = 0;	// when screen initialised
		virtual void update() = 0;	// called periodically
		virtual void exit() = 0;	// cleaning up
};
