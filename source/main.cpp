#include <iostream>
#include <fstream>

#include "Parameters.h"
#include "Solver.h"

int main()
{
	const Parameters pars(std::ifstream("data/data.txt"));
	std::cout << pars.dummy_parameter << std::endl;
	Solver task(pars);
	task.start();
	return 0;
}
