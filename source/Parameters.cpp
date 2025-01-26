#include "Parameters.h"
#include <iterator>
#include <typeinfo>
#include <stacktrace>
#include "io_auxiliary.h"

template<const int size>
void split_string(const std::string& init, std::array<std::string, size>& result, const char sep)
{
	std::stringstream ss(init);
	auto it = result.begin();
	while (std::getline(ss, *it++, sep))
	{
		expect<Error_action::throwing, std::length_error>(
			[&](){return (std::distance(it, const_cast<std::array<std::string, size>::iterator>(result.end())) >= 0); }, 
			(std::string("Size of init string is too big, must be = ") + std::to_string(size)).c_str()
		);
	}
	expect<Error_action::throwing, std::length_error>(
		[&](){return (std::distance(it, const_cast<std::array<std::string, size>::iterator>(result.end())) == -1); }, 
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
			{"steps", {"int", &steps}},
			{"sentence", {"string", &sentence}},
			{"enum_parameter", {"enum_test", &enum_parameter}}
		};
		const int number_of_properties = 2;
		auto str_to_enum_test = [](std::string read_value)
		{
			if (read_value == "red") return enum_test::red;
			else if (read_value == "blue") return enum_test::blue;
			else if (read_value == "green") return enum_test::green;
			else
			{
				expect<Error_action::terminating, std::invalid_argument>(
					[](){return false; },
					"incorrect str_to_enum read value"
				);
			}
		};

		for (std::string read; std::getline(fin, read); )
		{
			if (read[0] == '#') continue;
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
				else if (type.compare("string") == 0)
					*(static_cast<std::string*>(var)) = value;
				else if (type.compare("enum_test") == 0)
					*(static_cast<enum_test*>(var)) = str_to_enum_test(value);
				/*std::cout << var_read_properties[0] << " : " << value << std::endl;*/
			}
			/*catch (const std::length_error &err)*/
			catch (std::exception &err)
			{
				std::cerr << std::endl << "failure : " << err.what() << std::endl;
				std::cerr << "Read : " << read << std::endl << std::endl;
				std::terminate();
			}
		}
		std::cout << "===================" << std::endl;
	}
}

