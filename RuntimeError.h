#pragma once
#include <exception>
#include "Token.h"

namespace ZScript {
	class RuntimeError : public std::exception {
	public:
		RuntimeError(const Token& token, const std::string& message) : exception(message.c_str()) {
			m_token = token;
		}
	private:
		Token m_token;
	};
}
