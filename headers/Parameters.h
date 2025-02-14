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
#include <set>
#include <algorithm>

#include <charconv>

#include <concepts>
#include <iterator>
#include <type_traits>

#include "io_auxiliary.h"
#include "error_handling.h"
#include "custom_concepts.h"

// std::unordered_map w/ string_hash for heterogenious search
template<typename key, typename value>
using ums_w_hs = std::unordered_map<key, value, string_hash, std::equal_to<>>;

class Parameters
{
	public:
		// General variables
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

		int nt_write;
		std::string write_file;

		// Wall-related variables
		struct wall
		{
			enum class w_type
			{
				noslip,
				flux
			} type;
			int n_fict = 1; // set implicitly depending on type(will make it later, depending on solver)
			double P, v;
		};
		
		static constexpr int number_of_walls = 2;
		std::array<wall, number_of_walls> walls;

		Parameters(){};
		Parameters(std::ifstream);
	
	protected:
		viscosity interp_viscosity(std::string_view str) const;
		ic_preset interp_ic_preset(std::string_view str) const;
		wall::w_type interp_wall_type(std::string_view str) const;
	
		void assign_read_value(std::string_view, std::string_view);
		void assign_read_wall_value(std::string_view, std::string_view, const ums_w_hs<std::string, std::pair<std::string, void*>>&, const int);
		std::string set_wall_properties(std::ifstream&, const int);

	private:
		ums_w_hs<std::string, std::pair<std::string, void*>> var_table
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
			{"initial_conditions", {"ic_preset", &ic}},
			{"is_conservative", {"bool", &is_conservative}}
		};

		std::set<void*> initialized_variables
		{
			&mu0 // adding default-initialized variables
		};

		bool does_initialized_values_contain_all_w_vars(const int, const ums_w_hs<std::string, std::pair<std::string, void*>>&) const noexcept;
		bool are_all_general_variables_initialized() const noexcept;	
		bool are_all_wall_variables_initialized(const std::vector<int>&) const noexcept;
};


#endif
