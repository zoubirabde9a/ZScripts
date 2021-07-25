#include "Scanner.h"

float toFloat(const std::string& string) {
	float output = 0.0f;
	size_t i;
	if (string.size() > 1 && (string[0] == '-' || string[0] == '+')) {
		if (string[1] == '.') {
			if (string.size() > 2) {
				float multiplier = 0.1f;
				for (i = 2; i < string.size(); i++) {
					output += (string[i] - '0') * multiplier;
					multiplier *= 0.1f;
				}
				if (string.size() > 0) {
					output *= -1 * (string[0] == '-') + (string[0] != '-');
				}
				return output;
			}
			else {
				i = 2;
			}
		}
		else {
			output += string[1] - '0';
			i = 2;
		}
	}
	else if (string.size() > 1 && string[0] == '.') {
		float multiplier = 0.1f;
		for (i = 1; i < string.size(); i++) {
			output += (string[i] - '0') * multiplier;
			multiplier *= 0.1f;
		}
		if (string.size() > 0) {
			output *= -1 * (string[0] == '-') + (string[0] != '-');
		}
		return output;
	}
	else if (string.size() > 0) {
		output += string[0] - '0';
		i = 1;
	}
	else {
		return output;
	}

	for (; i < string.size() && string[i] != '.'; i++) {
		output *= 10;
		output += string[i] - '0';
	}
	if (i < string.size() && string[i] == '.')i++;
	float multiplier = 0.1f;
	for (; i < string.size(); i++) {
		output += (string[i] - '0') * multiplier;
		multiplier *= 0.1f;
	}
	if (string.size() > 0) {
		output *= -1 * (string[0] == '-') + (string[0] != '-');
	}
	return output;
}

namespace ZScript {
	std::unordered_map < std::string, TokenType > Scanner::m_keyWords = {
		{ "and", AND },
		{ "class", CLASS },
		{ "else", ELSE },
		{ "false", FALSE },
		{ "for", FOR },
		{ "fun", FUN },
		{ "if", IF },
		{ "nil", NIL },
		{ "or", OR },
		{ "print", PRINT },
		{ "return", RETURN },
		{ "super", SUPER },
		{ "this", THIS },
		{ "true", TRUE },
		{ "var", VAR },
		{ "while", WHILE }
	};

	Scanner::Scanner(const std::string &source) : m_source(source) {
		
	}
	const std::vector < Token >& Scanner::scanTokens() {
		size_t tokenCount = 1;
		m_start = 0;
		m_current = 0;
		while (!isAtEnd()) {
			m_start = m_current;
			tokenCount+= countToken();
		}
		m_tokens.reserve(tokenCount);
		m_start = 0;
		m_current = 0;
		while (!isAtEnd()) {
			m_start = m_current;
			scanToken();
		}

		m_tokens.emplace_back(ENOF, "", m_line);
		return m_tokens;
	}
	bool Scanner::countToken() {
		char c = advance();
		switch (c) {
		case '(': return true; break;
		case ')': return true; break;
		case '{': return true; break;
		case '}': return true; break;
		case ',': return true; break;
		case '.': return true; break;
		case '-': return true; break;
		case '+': return true; break;
		case ';': return true; break;
		case '*': return true; break;
		case '!': return true; break;
		case '=': return true; break;
		case '<': return true; break;
		case '>': return true; break;
		case ' ':
		case '\r':
		case '\t':
			// Ignore whitespace.
			break;
		case '\n':
			m_line++;
			return true;
			break;
		case '/':
			if (match('/')) {
				// A comment goes until the end of the line.
				while (peek() != '\n' && !isAtEnd()) advance();
			}
			else {
				return true;
			}
			break;
		case char(39):
		case '"': stringCount(); return true; break;
		default:
			if (isDigit(c)) {
				numberCount();
				return true;
			}
			else if (isAlpha(c)) {
				identifierCount();
				return true;
			}
			else {
				error("Unexpected character.");
			}
			break;
		}
		return false;
	}
	void Scanner::scanToken() {
		char c = advance();
		switch (c) {
		case '(': addToken(LEFT_PAREN); break;
		case ')': addToken(RIGHT_PAREN); break;
		case '{': addToken(LEFT_BRACE); break;
		case '}': addToken(RIGHT_BRACE); break;
		case ',': addToken(COMMA); break;
		case '.': addToken(DOT); break;
		case '-': addToken(MINUS); break;
		case '+': addToken(PLUS); break;
		case ';': addToken(SEMICOLON); break;
		case '*': addToken(STAR); break;
		case '!': addToken(match('=') ? BANG_EQUAL : BANG); break;
		case '=': addToken(match('=') ? EQUAL_EQUAL : EQUAL); break;
		case '<': addToken(match('=') ? LESS_EQUAL : LESS); break;
		case '>': addToken(match('=') ? GREATER_EQUAL : GREATER); break;
		case ' ':
		case '\r':
		case '\t':
			// Ignore whitespace.
			break;
		case '\n':
			addToken(NEW_LINE);
			m_line++;
			break;
		case '/':
			if (match('/')) {
				// A comment goes until the end of the line.
				while (peek() != '\n' && !isAtEnd()) advance();
			}
			else {
				addToken(SLASH);
			}
			break;
		case char(39):
		case '"': string(); break;
		default:
			if (isDigit(c)) {
				number();
			}
			else if (isAlpha(c)) {
				identifier();
			}
			else {
				error("Unexpected character.");
			}
			break;
		}
	}





	char Scanner::advance() {
		m_current++;
		return m_source[m_current - 1];
	}
	void Scanner::addToken(TokenType type) {
		std::string lexeme = m_source.substr(m_start, m_current - m_start);
		m_tokens.emplace_back(type, lexeme, m_line);
	}
	void Scanner::addToken(TokenType type, const std::string &lexeme) {
		m_tokens.emplace_back(type, lexeme, m_line);
	}
	void Scanner::addToken(double number, const std::string &lexeme) {
		m_tokens.emplace_back(NUMBER, lexeme, number, m_line);
	}
	void Scanner::addToken(const char* str, const std::string &lexeme) {
		m_tokens.emplace_back(STRING, lexeme, str, m_line);
	}
	void Scanner::addToken(TokenType type, bool b, const std::string &lexeme) {
		m_tokens.emplace_back(type, lexeme, b, m_line);
	}
	bool Scanner::match(char expected) {
		if (isAtEnd()) return false;
		if (m_source[m_current] != expected) return false;

		m_current++;
		return true;
	}
	char Scanner::peek() const {
		if (isAtEnd()) return '\0';
		return m_source[m_current];
	}
	char Scanner::peekNext() const {
		if (m_current + 1 >= m_source.size()) return '\0';
		return m_source[m_current + 1];
	}
	void Scanner::string() {
		while ((peek() != '"' && peek() != char(39)) && !isAtEnd()) {
			if (peek() == '\n') m_line++;
			advance();
		}

		// Unterminated string.
		if (isAtEnd()) {
			error("Unterminated string.");
			return;
		}

		// The closing ".
		advance();

		// Trim the surrounding quotes.
		std::string value = m_source.substr(m_start + 1, m_current - m_start - 2);
		addToken(value.c_str(), m_source.substr(m_start, m_current - m_start));
	}
	void Scanner::stringCount() {
		while ((peek() != '"' && peek() != char(39)) && !isAtEnd()) {
			if (peek() == '\n') m_line++;
			advance();
		}

		if (isAtEnd()) {
			error("Unterminated string.");
			return;
		}

		advance();

	}
	bool Scanner::isDigit(char c) const {
		return c >= '0' && c <= '9';
	}
	void Scanner::number() {
		while (isDigit(peek())) advance();

		// Look for a fractional part.
		if (peek() == '.' && isDigit(peekNext())) {
			// Consume the "."
			advance();

			while (isDigit(peek())) advance();
		}
		std::string lexeme = m_source.substr(m_start, m_current - m_start);
		addToken(toFloat(lexeme), lexeme);
	}
	void Scanner::numberCount() {
		while (isDigit(peek())) advance();

		if (peek() == '.' && isDigit(peekNext())) {
			advance();

			while (isDigit(peek())) advance();
		}
	}
	void Scanner::identifier() {
		while (isAlphaNumeric(peek())) advance();

		std::string text = m_source.substr(m_start, m_current - m_start);
		TokenType type;
		auto &it = m_keyWords.find(text);
		if (it != m_keyWords.end()) {
			type = it->second;
		}
		else {
			type = IDENTIFIER;
		}
		addToken(type, text);
	}
	void Scanner::identifierCount() {
		while (isAlphaNumeric(peek())) advance();
	}
	bool Scanner::isAlpha(char c) const {
		return (c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			c == '_';
	}

	bool Scanner::isAlphaNumeric(char c) const {
		return isAlpha(c) || isDigit(c);
	}
	void Scanner::error(const std::string &err) {
		printf("error line : %u %s\n", m_line, err.c_str());
		m_hasError = true;
	}
	bool Scanner::isAtEnd() const {
		return m_current >= m_source.size();
	}

}
