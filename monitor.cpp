#include "./monitor.h"
#include <thread>
#include <chrono>
#include <iostream>
using std::cout, std::flush, std::this_thread::sleep_for, std::chrono::seconds;

void alertBlink(const char* message) {
    cout << message << "\n";
    for (int i = 0; i < 6; i++) {
        cout << "\r* " << flush;
        sleep_for(seconds(1));
        cout << "\r *" << flush;
        sleep_for(seconds(1));
    }
}

struct VitalCheck {
    bool (*check)(float, float, float);
    const char* message;
};

bool tempOutOfRange(float temperature, float, float) {
    return temperature > 102 || temperature < 95;
}
bool pulseOutOfRange(float, float pulseRate, float) {
    return pulseRate < 60 || pulseRate > 100;
}
bool spo2OutOfRange(float, float, float spo2) {
    return spo2 < 90;
}

int vitalsOk(float temperature, float pulseRate, float spo2) {
    VitalCheck checks[] = {
        { tempOutOfRange, "Temperature is critical!" },
        { pulseOutOfRange, "Pulse Rate is out of range!" },
        { spo2OutOfRange, "Oxygen Saturation out of range!" }
    };

    for (const auto& check : checks) {
        if (check.check(temperature, pulseRate, spo2)) {
            alertBlink(check.message);
            return 0;
        }
    }

    return 1;
}
