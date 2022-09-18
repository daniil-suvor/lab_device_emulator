#ifndef _EMULATOR_H_
#define _EMULATOR_H_

#include <iostream>
#include <vector>
#include <algorithm>

#include <cstdlib>
#include <ctime>

class pump {
    public:
        pump();
        void start(double new_sped);
        void print_speed();
        void redo_speed();
    private:
        double speed;
};
class pressure_sensor {
    public:
        pressure_sensor();
        void set_pressure(double new_pressure);
        void print_pressure();
        void redo_pressure();
    private:
        double pressure;
};
class emulator {
    public:
        emulator();
        void print_state();
        void start(double new_speed);
        void set_pressure(double new_pressure, int number);
        void redo_parametr();
    private:
        pump pump_1;
        std :: vector <pressure_sensor> sensors;
};
#endif  // _EMULATOR_H_
