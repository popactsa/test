#ifndef IO_AUXILIARY_H
#define IO_AUXILIARY_H

#include <iostream>
#include <string>
#include <filesystem>
#include <format>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <array>
#include <cmath>
#include <initializer_list>
#include <unordered_map>

#include "error_handling.h"

extern struct winsize w;
extern const std::time_t start_time;
extern const std::chrono::time_point<std::chrono::system_clock> start_tick_time;

namespace io_constants
{
	extern const int asc_time_len;
	constexpr int max_file_name_size = 35;
	constexpr int max_solver_name_size = 15;
}

enum class solver_types
{
	unknown,
	Lagrange_1D
};

static std::unordered_map<std::string, solver_types> solver_types_table
{
	{"unknown", solver_types::unknown},
	{"Lagrange 1D", solver_types::Lagrange_1D}
};

std::string time_to_string(const std::filesystem::file_time_type&) noexcept;

template<typename timeformat>
inline int count_time_between(std::chrono::time_point<std::chrono::system_clock>& from, const std::chrono::time_point<std::chrono::system_clock>& to)
{
	int result = std::chrono::duration_cast<timeformat>(to - from).count();
	from = to;
	return result;
}

template<typename timeformat>
inline int count_time_between_const(const std::chrono::time_point<std::chrono::system_clock>& from, const std::chrono::time_point<std::chrono::system_clock>& to)
{
	int result = std::chrono::duration_cast<timeformat>(to - from).count();
	return result;
}

std::filesystem::path get_path_to_file_in_dir(const std::filesystem::path&, int, std::string_view);

inline std::filesystem::path get_path_to_file_in_dir(const std::filesystem::path &dir, int pos)
{
	return get_path_to_file_in_dir(dir, pos, "");
}

int print_filenames(const std::filesystem::path&, std::string_view) noexcept;

template<const int size>
void split_string(const std::string& init, std::array<std::string, size>& result, const char sep)
{
	std::stringstream ss(init);
	auto it = result.begin();
	while (std::getline(ss, *it++, sep))
	{
		expect<Error_action::throwing, std::length_error>(
			[&](){return (std::distance(it, const_cast<std::array<std::string, size>::iterator>(result.end())) >= 0); }, 
			(std::string("Size of init string is too big, must be = ") + std::to_string(size)).c_str()
		);
	}
	expect<Error_action::throwing, std::length_error>(
		[&](){return (std::distance(it, const_cast<std::array<std::string, size>::iterator>(result.end())) == -1); }, 
		(std::string("Size of init string is too small, must be = ") + std::to_string(size)).c_str()
	);
}

template<const int size>
inline void split_string(const std::string& init, std::array<std::string, size>& result)
{
	return split_string<size>(init, result, ' ');
}

inline int print_filenames(const std::filesystem::path& dir) noexcept
{
	return print_filenames(dir, "");
}

int choose_in_range(const int, const int);

bool check_rmod(const std::filesystem::path&) noexcept;

void fmt_add_align(std::string& fmt, const std::string& align, const std::vector<int>& sizes) noexcept;

inline void fmt_add_align(std::string& fmt, const std::string& align, const std::vector<int>& sizes) noexcept
{
	for (auto it : sizes)
	{
		fmt += "{:";
		fmt += align;
		fmt += std::to_string(it);
		fmt += "}";
	}
}

inline void fmt_add_align(std::string& fmt, const std::vector<std::pair<std::string, int>>& align_n_sizes) noexcept
{
	for (auto [align, size] : align_n_sizes) fmt_add_align(fmt, align, {size});
}

#endif
