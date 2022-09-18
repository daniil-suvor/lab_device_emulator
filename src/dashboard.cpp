#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <vector>
#include <regex>
#include <iostream>
#include "emulator.h"

#define STOP -1
#define SERVICE 0
#define STANDART 1
#define START 0
#define SET_PRESSURE 1
#define PARAMETRS 2
#define ERROR_COMMAND -1

std::condition_variable signal;
std::mutex signal_mutex;
int flag_ = 0;

boost::lockfree::queue <double> queue(100);
boost::lockfree::queue <double> service_queue(100);

std::vector<std::string> parsing_command(std :: string command) {
    std :: vector <std :: string> words;
    std::istringstream input;
    input.str(command);
    for (std::string word; std::getline(input, word, ' ');) {
        if (word[0] != '\0')
            words.push_back(word);
    }
    return words;
}
std::vector<double> check_command(std :: string command) {
    std :: vector <double> result;
    std :: vector <std :: string> tokens = parsing_command(command);
    if (std :: regex_match(command, std :: regex("stop"))) {
        result = {SERVICE, STOP};
    } else if (std :: regex_match(command, std :: regex("time [0-9]+"))) {
        result = {SERVICE, std :: atof(tokens[1].c_str())};
    } else if (std :: regex_match(command, std :: regex("stop pump"))) {
        result = {STANDART, START, 0};
    } else if (std :: regex_match(command, std :: regex("start [0-9]+(\\.[0-9]+){0,1}"))) {
        result = {STANDART, START, std :: atof(tokens[1].c_str())};
    } else if (std :: regex_match(command, std :: regex("reset [12]"))) {
        result = {STANDART, SET_PRESSURE, std :: atof(tokens[1].c_str()), 0};
    } else if (std :: regex_match(command, std :: regex("set [12] [0-9]+(\\.[0-9]+){0,1}"))) {
        result = {STANDART, SET_PRESSURE, std :: atof(tokens[1].c_str()), atof(tokens[2].c_str())};
    } else if (std :: regex_match(command, std :: regex("parametrs"))) {
        result = {STANDART, PARAMETRS};
    } else {
        result = {ERROR_COMMAND};
    }
    return result;
}

void command_processing(emulator* stend) {
    using namespace std :: chrono_literals;
    double value, speed, number, pressure;
    std::unique_lock<std::mutex> signal_wait(signal_mutex);
    auto dt = 1s;
    auto time = 10*dt;
    while (true) {
        if (signal.wait_for(signal_wait, time, []{return flag_ == 1;})) {
            flag_ = 0;
            service_queue.pop(value);
            service_queue.pop(value);
            if (value == STOP) {
                break;
            } else {
                time = (int)value*dt;
            }
        } else {
            if (queue.pop(value)) {
                queue.pop(value);
                switch ((int)value) {
                    case START:
                        queue.pop(speed);
                        stend->start(speed);
                        break;
                    case SET_PRESSURE:
                        queue.pop(number);
                        queue.pop(pressure);
                        stend->set_pressure(pressure, number - 1);
                        break;
                    case PARAMETRS:
                        stend->print_state();
                        break;
                }
            } else {
                stend->redo_parametr();
            }
            
        }
    }
}

int main() {
    emulator stend;
    std :: thread th(command_processing, &stend);
    std :: vector <double> message;
    std :: string command;
    while (true) {
        std::getline(std::cin, command);
        message = check_command(command);
        switch ((int)message[0]) {
            case SERVICE:
                std :: for_each(message.begin(), message.end(), [](double vl){service_queue.push(vl);});
                flag_ = 1;
                signal.notify_one();
                break;
            case STANDART:
                std :: for_each(message.begin(), message.end(), [](double vl){queue.push(vl);});
                break;
            default:
                std :: cout << "Erorr command!\n";
        }
        if ((message[0] == 0) && (message[1] == -1))
            break;
    }
    th.join();
    return 0;
}
