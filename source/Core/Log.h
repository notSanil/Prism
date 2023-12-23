#pragma once
#include <iostream>

#define LOG_ERROR(x) std::cout << "[ERROR] " << __FILE__ << " Line: " << __LINE__ << ' ' << x << "\n"
#define LOG_WARN(x)  std::cout << "[WARN] " << __FILE__ << " Line: " << __LINE__ << ' ' << x << "\n"
#define LOG_CORE(x)  std::cout << "[CORE] " << __FILE__ << " Line: " << __LINE__ << ' ' << x << "\n"
#define LOG_TRACE(x) std::cout << "[TRACE] " << __FILE__ << " Line: " << __LINE__ << ' ' << x << "\n"