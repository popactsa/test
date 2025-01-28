#include "Parameters.h"
#include <iterator>

Parameters::viscosity Parameters::interp_viscosity(std::string_view str) const
{
	using enum viscosity;
	std::unordered_map<std::string_view, viscosity> tbl
	{
		{"none", none},
		{"Neuman", Neuman},
		{"Latter", Latter},
		{"linear", linear},
		{"sum", sum}
	};
	auto found = tbl.find(str);
	if (found != tbl.end()) return found->second;
	else
	{
		std::cerr << "Passed incorrect value : " << str << std::endl;
		std::cerr << "Possible values : " << std::endl;
		for (auto it : tbl)
		{
			std::cerr << '\t' << it.first << std::endl;
		}
		throw std::invalid_argument("Incorrect viscosity value passed");
	}
}

Parameters::wall::w_type Parameters::interp_wall_type(std::string_view str) const
{
	using enum wall::w_type;
	std::unordered_map<std::string_view, wall::w_type> tbl
	{
		{"noslip", noslip},
		{"flux", flux}
	};
	auto found = tbl.find(str);
	if (found != tbl.end()) return found->second;
	else
	{
		std::cerr << "Passed incorrect value : " << str << std::endl;
		std::cerr << "Possible values : " << std::endl;
		for (auto it : tbl)
		{
			std::cerr << '\t' << it.first << std::endl;
		}
		throw std::invalid_argument("Incorrect ic_preset value passed");
	}
}

Parameters::ic_preset Parameters::interp_ic_preset(std::string_view str) const
{
	using enum ic_preset;
	std::unordered_map<std::string_view, ic_preset> tbl
	{
		{"test1", test1},
		{"test2", test2},
		{"test3", test3},
		{"test4", test4}
	};
	auto found = tbl.find(str);
	if (found != tbl.end()) return found->second;
	else
	{
		std::cerr << "Passed incorrect value : " << str << std::endl;
		std::cerr << "Possible values : " << std::endl;
		for (auto it : tbl)
		{
			std::cerr << '\t' << it.first << std::endl;
		}
		throw std::invalid_argument("Incorrect ic_preset value passed");
	}
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
	else if (!type.compare("bool"))
		*(static_cast<bool*>(ptr)) = static_cast<bool>(std::stoi(value));
	else if (!type.compare("string"))
		*(static_cast<std::string*>(ptr)) = value;
	else if (!type.compare("ic_preset"))
		*(static_cast<ic_preset*>(ptr)) = interp_ic_preset(value);
}

void Parameters::assign_read_wall_value(const std::string &value, std::string_view key, std::unordered_map<std::string_view, std::pair<std::string_view, void*>> w_table)
{
	// std::stoi, stod requires for std::string, so passing value as std::string_view is meaningless i think
	std::string_view type(w_table[key].first);
	void* ptr{w_table[key].second};
	if (!type.compare("double"))
		*(static_cast<double*>(ptr)) = std::stod(value);
	else if (!type.compare("type"))
		*(static_cast<wall::w_type*>(ptr)) = interp_wall_type(value);
}

void Parameters::set_wall_properties(std::ifstream& fin, const int n)
{
	std::unordered_map<std::string_view, std::pair<std::string_view, void*>> w_table
	{
		{"P", {"double", &(walls[n].p)}},
		{"v", {"double", &(walls[n].v)}},
		{"type", {"w_type", &(walls[n].type)}}
	};
	for (std::string read; std::getline(fin, read); )
	{
		if (read[0] == '#') continue;
		else if (read[0] == '\t')
		{
			const int number_of_properties = 2;
			std::array<std::string, number_of_properties> var_read_properties{}; // name value
			split_string<number_of_properties>(read, var_read_properties);
			var_read_properties[0].erase(0, 1); // pop aligning character

			auto found_name = w_table.find(var_read_properties[0]);
			expect<Error_action::throwing, std::invalid_argument>(
				[&, this]() {return found_name != w_table.end(); }, 
				"Can't find read variable name in w_table"
			);
			std::string value = var_read_properties[1];
			assign_read_wall_value(value, found_name->first, w_table);
		}
		else return;
	}
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
				if (found_name->first == "wall") 
				{
					expect<Error_action::throwing, std::invalid_argument>(
						[&var_read_properties, this]() {return std::stoi(var_read_properties[1]) <= static_cast<int>(walls.size()); }, 
						"Wall number doesn't fit in range"
					);
					set_wall_properties(fin, std::stoi(var_read_properties[1]));
					continue;
				}
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

