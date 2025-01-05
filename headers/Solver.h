#ifndef SOLVER_H
#define SOLVER_H

#include "Parameters.h"
#include <vector>

class Solver
{
	public:
		Solver(const Parameters& _pars):
			pars(_pars),
			pressure(pars.steps, pars.dummy_parameter)
		{}
		void start();
		void calc_smth(std::vector<double>&);
	private:
		const Parameters pars;
		std::vector<double> pressure;
};

#endif
