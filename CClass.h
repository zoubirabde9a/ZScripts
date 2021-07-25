#pragma once
#include <unordered_map>
#include "LoxCallable.h"
namespace ZScript {
	class CClass {
	public:
		CClass(CClass* parent, const std::string& name);

		void addFunction(const std::string& name, const Function& fn, size_t arity);
		void addFunction(const std::string& name, const LoxCallable& callable);
		const LoxCallable* get(const std::string& name);

	//	void getFunctionsNames(std::vector < ZEngine::Word > &names);

		inline const CClass* getParent() const { return m_parent; }
		inline CClass* getParent() { return m_parent; }
		inline const std::string& getName() const { return m_name; }
		inline const std::unordered_map < std::string, LoxCallable >& getFunctions() const { return m_functions; }
	private:
		std::string m_name;
		CClass* m_parent;
		std::unordered_map < std::string, LoxCallable > m_functions;
	};
}
