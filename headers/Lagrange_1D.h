#ifndef LAGRANGE_1D_H
#define LAGRANGE_1D_H

#include "Parameters.h"
#include "iSolver.h"
#include "error_handling.h"
#include <vector>

class Lagrange_1D: public iSolver {
	public:
		Lagrange_1D(const Parameters& _par): par{_par} {}
		void start() override;
		void apply_boundary_conditions() override;
		void solve_step() override;
		void set_initial_conditions() override;
		void get_time_step() override;
		void write_data() override;
		void check_parameters() override;
	private:
		const Parameters par;
		std::vector<double> p, rho, U, m;
		std::vector<double> v, x;
		std::vector<double> omega; //viscosity
		double t, dt;
		int step;
};
#endif
