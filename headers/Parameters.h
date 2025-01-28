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
#include <set>

#include "io_auxiliary.h"
#include "error_handling.h"

struct Parameters
{
	int nx_all; // calculated implicitly
	double dx;

	double x_start, x_end;
	int nx;
	double CFL;
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

	struct wall
	{
		enum class w_type
		{
			noslip,
			flux
		} type;
		int n_fict = 1; // set implicitly depending on type
		double p, v;
	};
	
	std::array<wall, 2> walls;
	int nt_write;
	std::string write_file;

	std::unordered_map<std::string, std::pair<const std::string, void*>> var_table
	{
		{"x_start", {"double", &x_start}},
		{"x_end", {"double", &x_end}},
		{"nx", {"int", &nx}},
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

	std::set<void*> initialized_variables
	{
		&mu0, // adding default-initialized variables
	};

	Parameters(){};
	Parameters(std::ifstream);

	void assign_read_value(const std::string&, const std::string&);
	viscosity interp_viscosity(std::string_view str) const;
	ic_preset interp_ic_preset(std::string_view str) const;
	wall::w_type interp_wall_type(std::string_view str) const;
	void assign_read_wall_value(const std::string&, const std::string&, std::unordered_map<std::string, std::pair<std::string, void*>>, const int);
	std::string set_wall_properties(std::ifstream&, const int);
	bool is_all_initialized() const;	
};


#endif
