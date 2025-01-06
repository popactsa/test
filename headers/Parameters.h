#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <exception>
#include <array>
#include <utility>

struct Parameters
{
	double dummy_parameter;
	int steps;

	Parameters(){};
	Parameters(Parameters&&) = default;
	Parameters(const Parameters&) = default;
	Parameters(std::ifstream);
};

template<const int size>
inline void split_string(const std::string& init, std::array<std::string, size>& result, const char = ' ');

#endif
