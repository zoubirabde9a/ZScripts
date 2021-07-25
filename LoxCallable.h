#pragma once
#include "Token.h"
#include "Stmt.h"
#include <functional>
#include <list>
#include "Environment.h"
namespace ZScript {
	typedef std::function < Literal(Interpreter*, const std::vector < Literal >&) > Function;
	class LoxCallable {
	public:
		LoxCallable(): m_arity(0), m_call(nullptr) {}
		LoxCallable(const Function& callFn, size_t arity);
		virtual Literal call(Interpreter* interpreter, const std::vector < Literal >& parameters) const {
			if (m_call) {
				return m_call(interpreter, parameters);
			}
			else return Literal();
		}
		LoxCallable* copy() const { return new LoxCallable(*this); }
		virtual size_t arity() const { return m_arity; }
	private:
		size_t m_arity;
		Function m_call;
	};
	class LoxFunction : public LoxCallable {
	public:
		LoxFunction(const std::list < Token >& parameters, const std::list < StmtPtr >& body, const std::string& name, Environment* closure);
		LoxFunction(const std::vector < Token >& parameters, const std::vector < StmtPtr >& body, const std::string& name, Environment* closure);

		Literal call(Interpreter* interpreter, const std::vector < Literal >& arguments) const override;
		size_t arity() const override {
			return m_parameters.size();
		}
		LoxFunction* copy()const;
		const std::string& getName() const { return m_name; }
	private:
		Environment* m_closure;
		std::string m_name;
		std::vector < Token > m_parameters;
		std::vector < StmtPtr > m_body;
	};
}
