#ifndef IO_AUXILIARY_H
#define IO_AUXILIARY_H

#include <iostream>
#include <string>
#include <filesystem>
#include <format>
#include <chrono>
#include <ctime>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <array>
#include <cmath>
#include <initializer_list>

#include "error_handling.h"

extern struct winsize w;

std::string time_to_string(const std::filesystem::file_time_type&) noexcept;

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

template<typename... Args>
inline std::string dynamic_print(std::string_view fmt_str, Args&&... args)
{
	return std::vformat(fmt_str, std::make_format_args(args...));
}

std::string get_format_by_left_side_impl(std::initializer_list<std::string_view>) noexcept;

template<typename... type>
std::string get_format_by_left_side(const type &... args) noexcept
{
	return get_format_by_left_side_impl({args...});
}

inline bool check_rmod(const std::filesystem::path& p)
{
	namespace fs = std::filesystem;
	if (!fs::exists(p))
	{
		std::cout << p << " doesn't exist" << std::endl;
		return false;
	}
	else if ((fs::status(p).permissions() & fs::perms::owner_read) != fs::perms::owner_read)
	{
		std::cout << p << " isn't readable" << std::endl;
		return false;
	}
	else if ((fs::is_empty(p)))
	{
		std::cout << p << " is empty" << std::endl;
		return false;
	}
	return true;
}



#endif
