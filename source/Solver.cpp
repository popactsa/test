#include "Solver.h"

void Solver::start()
{
	calc_smth(pressure);
	std::vector temp_pressure(pars.steps, 2 * pars.dummy_parameter);
	calc_smth(temp_pressure);
	std::cout << "sentence is : " << pars.sentence << std::endl;
	std::cout << "enum parameter is : " << static_cast<int>(pars.enum_parameter) << std::endl;
}

void Solver::calc_smth(std::vector<double>& p)
{
	for (auto const& it : p)
	{
		std::cout << it << " ";
	}
	std::cout << std::endl;
}

