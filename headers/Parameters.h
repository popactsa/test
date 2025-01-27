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

	std::unordered_map<std::string_view, std::pair<std::string_view, void*>> var_table
	{
		{"dummy_parameter", {"double", &dummy_parameter}},
		{"steps", {"int", &steps}},
		{"sentence", {"string", &sentence}},
		{"enum_parameter", {"enum_test", &enum_parameter}}
	};

	Parameters(){};
	Parameters(std::ifstream);

	void assign_read_value(const std::string&, std::string_view);
	enum_test interp_enum_test(std::string_view) const;
};

template<const int size>
void split_string(const std::string& init, std::array<std::string, size>& result, const char = ' ');

#endif
