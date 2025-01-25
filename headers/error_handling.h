#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <exception>
#include <string>
#include <type_traits>
#include <cassert>

////////Tour of C++ 2022 B.Stroustrup p.49/////////

enum class Error_action
{
	ignore,
	throwing,
	terminating,
	logging
};

constexpr Error_action default_Error_action = Error_action::throwing;

template<Error_action action, class exc, class C>
constexpr void expect(const C cond, const char *msg)
{
	static_assert((void("failure : Provided exception isn't derived from std::exception"), std::is_base_of<std::exception, exc>::value == true));
	if constexpr (action == Error_action::throwing)
		if (!cond())
		{
			std::cerr << "failure :" << msg << std::endl;
			throw exc(msg);
		}
	if constexpr (action == Error_action::terminating)
		if (!cond())
		{
			std::cerr << msg << std::endl;
			std::terminate();
		}
	if constexpr (action == Error_action::logging)
		if (!cond()) std::cerr << "expect() failure: " << ' ' << exc(msg).what() << std::endl;
	// ignore --> nothing happens
}

#endif
