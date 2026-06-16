// interface for sound functions: ring, stop
class ISound {
	public:
		virtual ~ISound() = default;
		virtual void ring() = 0;
		virtual void stopRinging() = 0;
};
