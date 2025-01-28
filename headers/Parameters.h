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
#include <iterator>

#include "io_auxiliary.h"
#include "error_handling.h"

struct Parameters
{
	double x_start, x_end;
	int nx, nx_fict;
	double dx, CFL;
	int nt;

	double gamma;
	double mu0 = 2;
	bool is_conservative;

	enum class viscosity
	{
		none,
		Neuman,
		Latter,
		linear,
		sum
	} visc;

	enum class ic_preset
	{
		test1,
		test2,
		test3,
		test4
	} ic;

	int nt_write;
	std::string write_file;

	std::unordered_map<std::string_view, std::pair<std::string_view, void*>> var_table
	{
		{"x_start", {"double", &x_start}},
		{"x_end", {"double", &x_end}},
		{"nx", {"int", &nx}},
		{"nx_fict", {"int", &nx_fict}},
		{"dx", {"double", &dx}},
		{"CFL", {"double", &CFL}},
		{"nt", {"int", &nt}},
		{"nt_write", {"int", &nt_write}},
		{"write_file", {"string", &write_file}},
		{"gamma", {"double", &gamma}},
		{"mu0", {"double", &mu0}},
		{"viscosity", {"viscosity", &visc}},
		{"ic", {"ic_preset", &ic}},
		{"is_conservative", {"bool", &is_conservative}}
	};

	Parameters(){};
	Parameters(std::ifstream);

	void assign_read_value(const std::string&, std::string_view);
	viscosity interp_viscosity(std::string_view str) const;
	ic_preset interp_ic_preset(std::string_view str) const;
};


#endif
