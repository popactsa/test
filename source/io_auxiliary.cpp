#include "io_auxiliary.h"

std::string time_to_string(const std::filesystem::file_time_type& ftime) noexcept
{
	std::time_t cftime = std::chrono::system_clock::to_time_t(
		std::chrono::file_clock::to_sys(ftime)
	);
	std::string str = std::asctime(std::localtime(&cftime));
	str.pop_back(); // rm the trailing '\n' put by `asctime`
	return str;
}

bool check_rmod(const std::filesystem::path& p) noexcept
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

std::filesystem::path get_path_to_file_in_dir(const std::filesystem::path &dir, int pos, std::string_view postfix)
{
	int cnt{0};
	for (auto const& dir_entry : std::filesystem::directory_iterator{dir, std::filesystem::directory_options::skip_permission_denied})
	{
		using enum std::filesystem::perms;
		auto file_perms = std::filesystem::status(dir_entry).permissions();
		if ((file_perms & owner_read) == owner_read && dir_entry.path().string().ends_with(postfix))
		{
			if (cnt < pos) ++cnt;
			if (cnt == pos) return dir_entry.path();
		}
	}
	throw std::range_error("end of directory reached");
}

std::string get_format_by_left_side_impl(std::initializer_list<std::string_view> args) noexcept
{
	std::string fmt;
	for (auto const& it : args) 
	{
		fmt += it;		
	}
	fmt += "{:.>" + std::to_string(w.ws_col - static_cast<int>(fmt.size())) + "}";
	return fmt;
}


int print_filenames(const std::filesystem::path &dir, std::string_view postfix) noexcept
{
	int cnt{0};

	std::string fmt = "      {} : ";
	const int init_size = fmt.size() - 1;
	const int max_file_name_size = 35;
	const int max_solver_name_size = 15;
	fmt_add_align(fmt, ".<",
			max_file_name_size, max_solver_name_size);
	fmt_add_align(fmt, ".>", w.ws_col - init_size - max_file_name_size - max_solver_name_size);
	std::string_view name = "Lagrange_1D";
	for (auto const& dir_entry : std::filesystem::directory_iterator{dir, std::filesystem::directory_options::skip_permission_denied})
	{
		auto file_perms = std::filesystem::status(dir_entry).permissions();
		using enum std::filesystem::perms;
		if ((file_perms & owner_read) == owner_read && dir_entry.path().string().ends_with(postfix))
		{
			++cnt;
			std::string rp_str{static_cast<std::string>(std::filesystem::relative(dir_entry.path(), dir))};
			std::string time_str{time_to_string(dir_entry.last_write_time())};
			std::cout << std::vformat(fmt, std::make_format_args(cnt, rp_str, name, time_str)) << std::endl;
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
			if (choose >= min && choose <= max) return choose;
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
