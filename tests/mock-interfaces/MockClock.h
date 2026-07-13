#include "src/interfaces/IClock.h"

class MockClock : public IClock {
private:
    TimePoint now_ = { 0, 0 };
    Days::Day currentDay;
    int daysSince1970;

public:
    void setTime(int hour, int min) {
        now_ = { hour, min };
    }

    void setCurrentDay(const Days::Day& day) {
        currentDay = day;
    }

    void setDaysSince1970(int days) {
        daysSince1970 = days;
    }

    TimePoint now() const override { return now_; }

    Days::Day getCurrentDay() const override { return currentDay; }

    int getDaysSince1970() const override { return daysSince1970; }
};
