#include "LoxCallable.h"
#include "Interpreter.h"

namespace ZScript {
	LoxCallable::LoxCallable(const Function& callFn, size_t arity)
		: m_call(callFn), m_arity(arity) {

	}
	LoxFunction::LoxFunction(const std::list < Token >& parameters, const std::list < StmtPtr >& body, const std::string& name, Environment* closure) :
		LoxCallable(nullptr, 0), m_closure(closure) {
		m_parameters.resize(parameters.size());
		m_body.resize(body.size());
		size_t i = 0;
		for (auto &it : parameters) {
			m_parameters[i++] = it;
		}
		i = 0;
		for (auto &it : body) {
			m_body[i++] = it;
		}
	}
	LoxFunction::LoxFunction(const std::vector < Token >& parameters, const std::vector < StmtPtr >& body, const std::string& name, Environment* closure) :
		LoxCallable(nullptr, 0), m_parameters(parameters), m_body(body), m_closure(closure) {
	}
	Literal LoxFunction::call(Interpreter* interpreter, const std::vector < Literal >& arguments) const {
		Environment environment = Environment(m_closure);
		auto it = m_parameters.begin();
		auto ij = arguments.begin();
		for (;it != m_parameters.end() && ij != arguments.end(); it++, ij++) {
			environment.define(it->getLexeme(), *ij);
		}

		try {
			interpreter->executeBlock(m_body, &environment);
		}
		catch (ReturnException& returnValue) {
			return returnValue.getValue();
		}
		return Literal();
	}

	LoxFunction* LoxFunction::copy() const {
		std::vector < Token > parameters(m_parameters.size());
		//std::vector < StmtPtr > body(m_body.size());
		for (auto &it : m_parameters) {
			parameters.push_back(it.copy());
		}
		/*for (auto &it : m_body) {
			if (it)body.push_back(it->copy());
		}*/
		return new LoxFunction(parameters, m_body, m_name, m_closure);
	}
}