#include <iostream>
#include <fstream>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#include "Parameters.h"
#include "Solver.h"
#include "io_auxiliary.h"

struct winsize w;

int main()
{
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	namespace fs = std::filesystem;
	const fs::path scenario_dir{"scenarios"};
	try
	{
		check_rmod(scenario_dir);
	}
	catch(fs::filesystem_error const& err)
	{
		std::terminate();
	}
	fs::path scenario_file;
	std::cout << "Choose scenario from " << scenario_dir << " : " << std::endl;
	int cnt = print_filenames(scenario_dir);
	int choose_scenario = choose_in_range(1, cnt);
	scenario_file = get_path_to_file_in_dir(scenario_dir, choose_scenario);

	const Parameters pars(std::ifstream{scenario_file});
	std::cout << pars.dummy_parameter << std::endl;
	Solver task(pars);
	task.start();
	return 0;
}
