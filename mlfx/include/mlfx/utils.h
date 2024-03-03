#pragma once

// https://gcc.gnu.org/onlinedocs/gcc-4.8.5/cpp/Stringification.html
#define XSTR(s) STR(s)
#define STR(s) #s

#include <ctime>
#include <string>

#include <chrono>
using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::steady_clock::time_point;
using Duration = std::chrono::steady_clock::rep;


#define HERE() std::cerr << ">> " << __FUNCTION__ << ":" << __LINE__ << "\n";
#define HERE_P(str) \
  std::cerr << ">> " << __FUNCTION__ << ":" << __LINE__ << " " << str << "\n";
#define assertm(exp, msg) assert(((void)msg, exp))

const char* HelloMLFX();

float rnd01();

float rnd(float min, float max);

