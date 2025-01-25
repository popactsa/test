#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <unordered_map>
#include <iomanip>
#include <exception>
#include <array>
#include <utility>

#include "error_handling.h"

enum class enum_test
{
	red,
	blue,
	green
};

struct Parameters
{
	double dummy_parameter;
	int steps;
	std::string sentence;
	enum_test enum_parameter;

	Parameters(){};
	Parameters(Parameters&&) = default;
	Parameters(const Parameters&) = default;
	Parameters(std::ifstream);
};

template<const int size>
void split_string(const std::string& init, std::array<std::string, size>& result, const char = ' ');

#endif
