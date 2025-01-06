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
		std::unordered_map<std::string, std::pair<std::string, void*>> var_table
		{
			{"dummy_parameter", {"double", &dummy_parameter}},
			{"steps", {"int", &steps}}
		};
		const int number_of_properties = 2;
		for (std::string read; std::getline(fin, read); )
		{
			std::array<std::string, number_of_properties> var_read_properties{}; // name value
			split_string<number_of_properties>(read, var_read_properties);
			try
			{
				std::string type{var_table[var_read_properties[0]].first};
				void* var{var_table[var_read_properties[0]].second};
				std::string value = var_read_properties[1];
				if (type.compare("double") == 0)
					*(double*)var = std::stod(value);
				else if (type.compare("int") == 0)
					*(int*)var = std::stoi(value);
				else
					std::terminate();
			}
			catch(...)
			{

			}
		}
	}
}

