#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <exception>
#include <string>
#include <type_traits>
#include <cassert>

////////Tour of C++ 2022 B.Stroustrup p.49/////////

namespace custom_exceptions
{
	class multiple_read_definitions : public std::exception
	{
		private:
			std::string what_message;
		public:
			explicit multiple_read_definitions(const std::string& msg) : what_message(msg) {}
			const char* what() const noexcept override
			{
				return what_message.c_str();
			}
	};
}


enum class Error_action
{
	ignore,
	throwing,
	terminating,
	logging
};

constexpr Error_action default_Error_action = Error_action::throwing;

template<Error_action action, class exc, class C>
constexpr void expect(const C cond, const std::string& msg)
{
	if constexpr (action == Error_action::throwing)
		if (!cond()) throw exc(msg);
	if constexpr (action == Error_action::terminating)
		if (!cond())
		{
			std::cerr << msg << std::endl;
			std::terminate();
		}
	if constexpr (action == Error_action::logging)
		if (!cond()) std::cerr << "expect() failure: " << msg << std::endl;
	// ignore --> nothing happens
}

#endif
