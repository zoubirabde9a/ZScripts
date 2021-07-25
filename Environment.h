#pragma once
#include <unordered_map>
#include "Token.h"
namespace ZScript {
	class Environment
	{
	public:
		Environment(Environment* enclosing);
		~Environment();
		void define(const std::string& name, const Literal& value);
		Literal get(const Token& name) const;
		Literal get(const std::string& name) const;
		void assign(const Token& name, const Literal& value);

		Environment* add();
		void remove(Environment* environement);
		
		inline Environment* getNext() { return m_next; }
		inline Environment* getPrevious() { return m_previous; }
		inline Environment* getEnclosing() { return m_enclosing; }
		inline const std::unordered_map< std::string, Literal >& getValues() { return m_values; }
	private:
		void setPrevious(Environment* environement);
		void setNext(Environment* environement);
	private:
		Environment* m_enclosing = nullptr;
		Environment* m_front = nullptr;
		Environment* m_back = nullptr;
		Environment* m_next = nullptr;
		Environment* m_previous = nullptr;
		size_t m_listSize = 0;
		std::unordered_map< std::string, Literal > m_values;
	};
}

