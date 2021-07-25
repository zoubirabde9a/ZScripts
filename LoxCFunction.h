#pragma once
#include "Token.h"


namespace ZScript {
	typedef Literal(*CFunction)(Interpreter* interpreter, const std::vector < Literal >& parameters);
	class LoxCFunction {
	public:
		LoxCFunction() {};
		LoxCFunction(CFunction callFn)
			: m_call(callFn) {}
		Literal call(Interpreter* interpreter, const std::vector < Literal >& parameters) {
			if (m_call) {
				return m_call(interpreter, parameters);
			}
			else return Literal();
		}
	private:
		CFunction m_call = nullptr;
	};
}