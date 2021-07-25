#include "Environment.h"
#include "RuntimeError.h"


namespace ZScript {
	Environment::Environment(Environment* enclosing) : m_enclosing(enclosing) {}
	void Environment::define(const std::string& name, const Literal& value) {
		m_values[name] = value.copy();
	}
	Environment::~Environment() {
		for (Environment* it = m_front; it != nullptr;) {
			Environment* next = it->getNext();
			delete it;
			it = next;
		}
		for (auto &it : m_values) {
			it.second.free();
		}
		m_values.clear();
	}
	Literal Environment::get(const Token &name) const {
		auto &it = m_values.find(name.getLexeme());
		if (it != m_values.end()) {
			return it->second;
		}
		if (m_enclosing != nullptr) return m_enclosing->get(name);
		throw RuntimeError(name,
			"Undefined variable '" + name.getLexeme() + "'.");
	}
	Literal Environment::get(const std::string& name) const {
		auto &it = m_values.find(name);
		if (it != m_values.end()) {
			return it->second;
		}
		if (m_enclosing != nullptr) return m_enclosing->get(name);
		return Literal();
	}
	void Environment::assign(const Token& name, const Literal& value) {
		auto it = m_values.find(name.getLexeme());
		if (it != m_values.end()) {
			m_values[name.getLexeme()] = value.copy();
			return;
		}
		if (m_enclosing != nullptr) {
			m_enclosing->assign(name, value);
			return;
		}
		throw RuntimeError(name,
			"Undefined variable '" + name.getLexeme() + "'.");
	}


	Environment* Environment::add() {
		Environment* environement = new Environment(this);
		if (m_back) {
			m_back->setNext(environement);
			environement->setPrevious(m_back);
			m_back = environement;
		}
		else {
			m_front = environement;
			m_back = environement;
		}
		m_listSize++;
		return environement;
	}
	void Environment::remove(Environment* environement) {
		if (environement->getEnclosing() == this) {
			Environment* next = environement->getNext();
			Environment* previous = environement->getPrevious();
			if (next && previous) {
				next->setPrevious(previous);
				previous->setNext(next);
			}
			else if (next) {
				m_front = next;
				next->setPrevious(nullptr);
			}
			else if (previous) {
				previous->setNext(nullptr);
			}
			else {
				m_front = nullptr;
				m_back = nullptr;
			}
			delete environement;
			m_listSize--;
		}
	}
	void Environment::setPrevious(Environment* environement) {
		m_previous = environement;
	}
	void Environment::setNext(Environment* environement) {
		m_next = environement;
	}
}
