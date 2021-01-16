#pragma once

#include <iostream>
#include <algorithm>
#include <functional>
#include <memory>
#include <thread>
#include <utility>
#include <cmath>
#include <fstream>
#include <cstdio>

#include <string>
#include <array>
#include <vector>

#include <SDL.h>

extern int allocated;
extern int deleted;
#define USAGE std::cout << "usage " << allocated << " bytes\n";