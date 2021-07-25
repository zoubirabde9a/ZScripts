#pragma once
#include <string>
#include <list>
#include <unordered_map>
#include "Token.h"

namespace ZScript {
	class Scanner
	{
	public:
		Scanner(const std::string &source);

		const std::vector < Token >& scanTokens();

		bool hasError() const { return m_hasError; }
	private:
		bool countToken();
		void scanToken();
		char advance();
		void addToken(TokenType type);
		void addToken(TokenType type, const std::string &lexeme);
		void addToken(double number, const std::string &lexeme);
		void addToken(const char* str, const std::string &lexeme);
		void addToken(TokenType type, bool b, const std::string &lexeme);
		bool match(char expected);
		char peek() const;
		char peekNext() const;
		void string();
		void stringCount();
		bool isDigit(char c) const;
		void number();
		void numberCount();
		void identifier();
		void identifierCount();
		bool isAlpha(char c) const;
		bool isAlphaNumeric(char c) const;
		void error(const std::string &err);
		bool isAtEnd() const;
	private:
		std::vector < Token > m_tokens;
		std::string m_source;
		size_t m_start = 0;
		size_t m_current = 0;
		size_t m_line = 1;
		bool m_hasError = false;
		static std::unordered_map < std::string, TokenType > m_keyWords;
	};
}
