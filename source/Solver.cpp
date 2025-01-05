#include "Solver.h"

void Solver::start()
{
	calc_smth(pressure);
	std::vector temp_pressure(pars.steps, 2 * pars.dummy_parameter);
	calc_smth(temp_pressure);
}

void Solver::calc_smth(std::vector<double>& p)
{
	for (auto it : p)
	{
		std::cout << it << " ";
	}
	std::cout << std::endl;
}

