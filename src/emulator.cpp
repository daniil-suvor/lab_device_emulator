#include "emulator.h"

pump :: pump() {
    speed = 0;
}
void pump :: print_speed() {
    std :: cout << "speed pump is " << speed << '\n';
}
void pump :: start(double new_speed) {
    speed = new_speed;
}
void pump ::redo_speed() {
    if (speed != 0) {
        std :: srand(std :: time(nullptr));
        int random_variable = std::rand() % 1001;
        speed += (random_variable % 2 == 0) ? random_variable / 100.0 : -random_variable / 100.0;
        speed = (speed < 0) ? 0 : speed;
    }
}

pressure_sensor :: pressure_sensor() {
    pressure = 0;
}
void pressure_sensor :: set_pressure(double new_pressure) {
    pressure = new_pressure;
}
void pressure_sensor :: print_pressure() {
    std :: cout << "pressure is " << pressure << '\n';
}
void pressure_sensor :: redo_pressure() {
    std :: srand(std :: time(nullptr));
    int random_variable = std::rand() % 1001;
    pressure += (random_variable % 2 == 0) ? random_variable / 100.0 : -random_variable / 100.0;
    pressure = (pressure < 0) ? 0 : pressure;
}

emulator :: emulator() {
    pump_1  = pump();
    sensors = {pressure_sensor(), pressure_sensor()};
}
void emulator :: print_state() {
    pump_1.print_speed();
    std :: for_each(sensors.begin(), sensors.end(), [](auto sens){sens.print_pressure();});
}
void emulator :: start(double new_speed) {
    pump_1.start(new_speed);
}
void emulator :: set_pressure(double new_pressure, int number) {
    sensors[number].set_pressure(new_pressure);
}
void emulator :: redo_parametr() {
    pump_1.redo_speed();
    std :: for_each(sensors.begin(), sensors.end(), [](auto& sens){sens.redo_pressure();});
}
