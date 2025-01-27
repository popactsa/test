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
	std::string_view postfix = ".sc";
	if (check_rmod(scenario_dir))
	{
		fs::path scenario_file;
		std::cout << "Choose scenario from " << scenario_dir << " : " << std::endl;
		int n_items = print_filenames(scenario_dir, postfix);
		int choose_item = choose_in_range(1, n_items);
		scenario_file = get_path_to_file_in_dir(scenario_dir, choose_item, postfix);

		const Parameters pars(std::ifstream{scenario_file});
		Solver task(pars);
		task.start();
	}
	return 0;
}
