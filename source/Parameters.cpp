#include "Parameters.h"
#include <iterator>
#include <typeinfo>
#include <stacktrace>

template<const int size>
void split_string(const std::string& init, std::array<std::string, size>& result, const char sep)
{
	std::stringstream ss(init);
	auto it = result.begin();
	long dist;
	while (std::getline(ss, *it++, sep))
	{
		dist = std::distance(it, const_cast<std::array<std::string, size>::iterator>(result.end()));
		// Strange behaviout : have to use dist variable, if not, then std::distance returns in expect() *true value* - 24;
		expect<Error_action::terminating, std::length_error>(
			/*[=](){return (std::distance(it, const_cast<std::array<std::string, size>::iterator>(result.end())) <= 0); }, */
			[=](){return dist >= 0; }, 
			(std::string("Size of init string is too big, must be = ") + std::to_string(size)).c_str()
		);
	}
	dist = std::distance(it, const_cast<std::array<std::string, size>::iterator>(result.end()));
	expect<Error_action::terminating, std::length_error>(
		[=](){return dist == -1; }, 
		(std::string("Size of init string is too small, must be = ") + std::to_string(size)).c_str()
	);
}

Parameters::Parameters(std::ifstream fin)
{
	if (!fin.is_open()) std::cout << "failed to open" << std::endl;
	else
	{
		std::unordered_map<std::string, std::pair<std::string, void*>> var_table
		{
			{"dummy_parameter", {"double", &dummy_parameter}},
			{"steps", {"int", &steps}}
		};
		const int number_of_properties = 2;
		for (std::string read; std::getline(fin, read); )
		{
			std::array<std::string, number_of_properties> var_read_properties{}; // name value
			try
			{
				split_string<number_of_properties>(read, var_read_properties);
				expect<Error_action::throwing, std::invalid_argument>(
					[=](){return var_table.find(var_read_properties[0]) != var_table.end(); }, 
					"Incorrect value read"
				);
				std::string type{var_table[var_read_properties[0]].first};

				void* var{var_table[var_read_properties[0]].second};
				std::string value = var_read_properties[1];
				if (type.compare("double") == 0)
					*(static_cast<double*>(var)) = std::stod(value);
				else if (type.compare("int") == 0)
					*(static_cast<int*>(var)) = std::stoi(value);
				std::cout << var_read_properties[0] << " : " << value << std::endl;
			}
			catch (const std::invalid_argument &err)
			{
				std::cerr << std::endl << "failure : " << err.what() << std::endl << std::endl;
			}
			catch (const std::length_error &err)
			{
				std::cerr << std::endl << "failure : " << err.what() << std::endl << std::endl;
			}
		}
	}
}

