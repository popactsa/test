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
#include <cmath>
#include <initializer_list>

extern struct winsize w;

std::string time_to_string(const std::filesystem::file_time_type&) noexcept;

std::filesystem::path get_path_to_file_in_dir(const std::filesystem::path&, int);

int print_filenames(const std::filesystem::path&) noexcept;

int choose_in_range(const int, const int);

template<typename... Args>
inline std::string dynamic_print(std::string_view fmt_str, Args&&... args)
{
	return std::vformat(fmt_str, std::make_format_args(args...));
}

std::string get_format_by_left_side_impl(std::initializer_list<std::string_view>) noexcept;

template<typename... type>
std::string get_format_by_left_side(const type &...) noexcept;

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
