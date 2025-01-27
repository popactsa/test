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

enum_test Parameters::interp_enum_test(std::string_view str) const
{
	if (str == "red") return enum_test::red;
	else if (str == "blue") return enum_test::blue;
	else if (str == "green") return enum_test::green;
	else throw std::invalid_argument("Incorrect enum_test value passed");
}

void Parameters::assign_read_value(const std::string &value, std::string_view key)
{
	// std::stoi, stod requires for std::string, so passing value as std::string_view is meaningless i think
	std::string_view type(var_table[key].first);
	void* ptr{var_table[key].second};
	if (!type.compare("double"))
		*(static_cast<double*>(ptr)) = std::stod(value);
	else if (!type.compare("int"))
		*(static_cast<int*>(ptr)) = std::stoi(value);
	else if (!type.compare("string"))
		*(static_cast<std::string*>(ptr)) = value;
	else if (!type.compare("enum_test"))
		*(static_cast<enum_test*>(ptr)) = interp_enum_test(value);
}

Parameters::Parameters(std::ifstream fin)
{
	if (!fin.is_open()) std::cout << "failed to open" << std::endl;
	else
	{
		const int number_of_properties = 2;
		for (std::string read; std::getline(fin, read); )
		{
			if (read[0] == '#') continue;
			std::array<std::string, number_of_properties> var_read_properties{}; // name value
			try
			{
				split_string<number_of_properties>(read, var_read_properties);
				auto found_name = var_table.find(var_read_properties[0]);
				expect<Error_action::throwing, std::invalid_argument>(
					[found_name, this]() {return found_name != var_table.end(); }, 
					"Can't find read variable name in var_table"
				);
				std::string value = var_read_properties[1];
				assign_read_value(value, found_name->first);
			}
			catch (const std::invalid_argument& err)
			{
				std::cerr << std::endl << "failure : " << err.what() << std::endl;
				std::cerr << "Read : " << read << std::endl << std::endl;
				std::terminate();
			}
		}
		std::cout << "===================" << std::endl;
	}
}

