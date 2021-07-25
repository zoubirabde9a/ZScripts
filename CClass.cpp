#include "CClass.h"


namespace ZScript {
	CClass::CClass(CClass* parent, const std::string& name)
		: m_parent(parent), m_name(name){

	}
	void CClass::addFunction(const std::string& name, const Function& fn, size_t arity) {
		m_functions[name] = LoxCallable(fn, arity);
	}
	void CClass::addFunction(const std::string& name, const LoxCallable& callable) {
		m_functions[name] = callable;
	}
	const LoxCallable* CClass::get(const std::string& name) {
		auto it = m_functions.find(name);
		if (it != m_functions.end()) {
			return &it->second;
		}
		else if (m_parent) {
			return m_parent->get(name);
		}
		else return nullptr;
	}


	/*void CClass::getFunctionsNames(std::vector < ZEngine::Word > &names) {
		const ZScript::CClass* tmp = this;
		size_t i = 0;
		while (tmp != nullptr) {
			i += tmp->m_functions.size();
			tmp = tmp->getParent();
		}
		names.resize(i);
		tmp = this;
		i = 0;
		while (tmp != nullptr) {
			for (auto &it : tmp->m_functions) {
				names[i++] = { it.first, ZEngine::WordType::FUNCTION };
			}
			tmp = tmp->getParent();
		}
	}*/
}