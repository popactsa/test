#include "io_auxiliary.h"

std::string time_to_string(const std::filesystem::file_time_type& ftime)
{
	std::time_t cftime = std::chrono::system_clock::to_time_t(
		std::chrono::file_clock::to_sys(ftime)
	);
	std::string str = std::asctime(std::localtime(&cftime));
	str.pop_back(); // rm the trailing '\n' put by `asctime`
	return str;
}

std::filesystem::path get_path_to_file_in_dir(const std::filesystem::path &dir, int pos)
{
	int cnt{0};
	for (auto const& dir_entry : std::filesystem::directory_iterator{dir, std::filesystem::directory_options::skip_permission_denied})
	{
		using enum std::filesystem::perms;
		auto file_perms = std::filesystem::status(dir_entry).permissions();
		if ((file_perms & owner_read) == owner_read)
		{
			if (cnt < pos) ++cnt;
			if (cnt == pos) return dir_entry.path();
		}
	}
	throw std::range_error("end of directory reached");
}

template<typename... Args>
inline std::string dynamic_print(std::string_view fmt_str, Args&&... args)
{
	return std::vformat(fmt_str, std::make_format_args(args...));
}

int print_filenames(const std::filesystem::path &dir)
{
	int cnt{0};
	for (auto const& dir_entry : std::filesystem::directory_iterator{dir, std::filesystem::directory_options::skip_permission_denied})
	{
		using enum std::filesystem::perms;
		auto file_perms = std::filesystem::status(dir_entry).permissions();
		if ((file_perms & owner_read) == owner_read)
		{
			++cnt;
			const std::string rp_str{static_cast<std::string>(std::filesystem::relative(dir_entry.path(), dir))};
			const std::string time_str{time_to_string(dir_entry.last_write_time())};
			const int number_of_chars_on_the_left{4 + std::floor(1 + std::log10(cnt)) + 3 + static_cast<int>(rp_str.size())}; // 4 spaces
			std::string fmt("    {} : {}{:.>" + std::to_string(w.ws_col - number_of_chars_on_the_left) + "}");
			std::cout << dynamic_print(fmt, cnt, rp_str, time_str) << std::endl;
		}
	}
	return cnt;
}

int choose_in_range(const int min, const int max)
{
	if (min == max) return min;
	std::string read;
	while (std::getline(std::cin, read))
	{
		try
		{
			int choose = std::stoi(read);
			if (choose >= min && choose <= max) 
			{
				return choose;
				break;
			}
			else std::cout << "Please, choose value in range [" << min << ", " << max << "]" << std::endl;
		}
		catch (std::invalid_argument &err) 
		{
			if (!read.empty()) std::cout << "Enter only integers" << std::endl;
			else std::cout << "~Empty input~" << std::endl;
			continue;
		}
	}
}
