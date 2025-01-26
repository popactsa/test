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

extern struct winsize w;

std::string time_to_string(const std::filesystem::file_time_type&);

std::filesystem::path get_path_to_file_in_dir(const std::filesystem::path&, int);

int print_filenames(const std::filesystem::path&);

int choose_in_range(const int, const int);

template<typename... Args>
inline std::string dynamic_print(std::string_view, Args&&...);

inline bool check_rmod(const std::filesystem::path& p)
{
	namespace fs = std::filesystem;
	if (!fs::exists(p))
	{
		std::cerr << p << " doesn't exist" << std::endl;
		throw fs::filesystem_error("Doesn't exist", std::error_code());
	}
	else if ((fs::status(p).permissions() & fs::perms::owner_read) != fs::perms::owner_read)
	{
		std::cerr << p << " isn't readable" << std::endl;
		throw fs::filesystem_error("Unreadable", std::error_code());
	}
	else if ((fs::is_empty(p)))
	{
		std::cerr << p << " is empty" << std::endl;
		throw fs::filesystem_error("Empty", std::error_code());
	}
	return true;
}

#endif
