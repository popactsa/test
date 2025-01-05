#include "Parameters.h"

template<const int size>
void split_string(const std::string& init, std::array<std::string, size>& result, const char sep)
{
	std::stringstream ss(init);
	for (auto it = result.begin(); std::getline(ss, *it++, sep););
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
			std::array<std::string, number_of_properties> var_read_properties{}; // type name value
			split_string<number_of_properties>(read, var_read_properties);
			if (var_read_properties[0].compare("double") == 0)
				*(double*)var_table[var_read_properties[1]] = std::stod(var_read_properties[2]);
			else if (var_read_properties[0].compare("int") == 0)
				*(int*)var_table[var_read_properties[1]] = std::stoi(var_read_properties[2]);
			else
				std::terminate();
		}
	}
}

