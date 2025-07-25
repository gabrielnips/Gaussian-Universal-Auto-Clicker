#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <thread>
#include <chrono>
#include <atomic>
#include <random>
#include <functional>
#include <limits>
#include <memory>
#include <stdexcept>
#include <algorithm>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "nlohmann/json.hpp"

#include "globals.hpp"

#include "Interfaces/IConsoleService.hpp"
#include "Services/ConsoleService.hpp"