#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <exception>
#include <regex>

struct Parameters
{
	double dummy_parameter;
	int steps;

	Parameters(){};
	Parameters(Parameters&&) = default;
	Parameters(const Parameters&) = default;
	Parameters(std::ifstream);
};

template<const int size>
void dispense_to_strings(std::string strs[size], std::sregex_token_iterator it)
{
	std::sregex_token_iterator end;
	for (int i = 0; i < size && it != end; ++i)
	{
		strs[i] = *it++;
	}
}

Parameters::Parameters(std::ifstream fin)
{
	if (!fin.is_open()) std::cout << "failed to open" << std::endl;
	else
	{
		std::unordered_map<std::string, void*> var_table
		{
			{"dummy_parameter", &dummy_parameter},
			{"steps", &steps}
		};
		const int number_of_properties = 3;
		for (std::string read; std::getline(fin, read); )
		{
			/* Defining variable type(read from file) and then assigning a read value to its variable*/
			const std::regex del("\\s+"); // whitespace
			std::sregex_token_iterator it(read.begin(), read.end(), del, -1);
			std::string var_read_properties[number_of_properties]{}; // type name value
			dispense_to_strings<number_of_properties>(var_read_properties, it);
			if (var_read_properties[0].compare("double") == 0)
				*(double*)var_table[var_read_properties[1]] = std::stod(var_read_properties[2]);
			else if (var_read_properties[0].compare("int") == 0)
				*(int*)var_table[var_read_properties[1]] = std::stod(var_read_properties[2]);
			else
				std::terminate();
		}
	}
}

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

int main()
{
	const Parameters pars(std::ifstream("data/data.txt"));
	std::cout << pars.dummy_parameter << std::endl;
	Solver task(pars);
	task.start();
	return 0;
}
