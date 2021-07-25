#include "Token.h"
#include "LoxCallable.h"


namespace ZScript {
	Literal::Literal() : type(VAR_NIL) {}
	Literal::Literal(bool b) : type(VAR_BOOLEAN), boolean(b) {}
	Literal::Literal(double number) : type(VAR_NUMBER), number(number) {}
	Literal::Literal(float number) : type(VAR_NUMBER), number(number) {}
	Literal::Literal(size_t number) : type(VAR_NUMBER), number(number) {}
	Literal::Literal(int number) : type(VAR_NUMBER), number(number) {}
	Literal::Literal(const char* str) : type(VAR_STRING){
		size_t s = 0;
		for (s = 0; str[s] != 0; s++);
		size = s;
		string = new char[s + 1];
		for (size_t i = 0; i < s; i++) {
			string[i] = str[i];
		}
		string[size] = 0;
	}
	Literal::Literal(char* str) : type(VAR_STRING) {
		size_t s = 0;
		for (s = 0; str[s] != 0; s++);
		size = s;
		string = new char[s + 1];
		for (size_t i = 0; i < s; i++) {
			string[i] = str[i];
		}
		string[size] = 0;
	}
	Literal::Literal(const std::string& str) : type(VAR_STRING) {
		size_t s = 0;
		for (s = 0; str[s] != 0; s++);
		size = s;
		string = new char[s + 1];
		for (size_t i = 0; i < s; i++) {
			string[i] = str[i];
		}
		string[size] = 0;
	}

	void Literal::free() {
		if (type == VAR_CALLABLE && callable) {
			delete callable;
		}
		else if (type == VAR_FUNCTION && function) {
			delete function;
		}
		else if (type == VAR_CFUNCTION && cFunction) {
			delete cFunction;
		}
		else if (type == VAR_STRING && string) {
			delete[] string;
		}
		else if (type == VAR_USERDATA) {
			shared_release(userData);
		}
	}
	Literal Literal::copy() const {
		Literal newLiteral;
		if (type == VAR_CALLABLE && callable) {
			newLiteral.callable = callable->copy();
			newLiteral.type = VAR_CALLABLE;
			return newLiteral;
		}
		else if (type == VAR_FUNCTION && function) {
			newLiteral.function = function->copy();
			newLiteral.type = VAR_FUNCTION;
			return newLiteral;
		}
		else if (type == VAR_STRING && string) {
			newLiteral.string = new char[size + 1];
			for (size_t i = 0; i <= size; i++) {
				newLiteral.string[i] = string[i];
			}
			newLiteral.type = VAR_STRING;
			newLiteral.size = size;
			return newLiteral;
		}
		else if (isUserData() && userData) {
			shared_ref(userData);
		}
		newLiteral = *this;
		return newLiteral;
	}
	std::string Literal::toString() const {
		if (type == VAR_NUMBER) {
			return std::to_string(number);
		}
		if (type == VAR_STRING) {
			return std::string(string);
		}
		if (type == VAR_BOOLEAN) {
			if (boolean)return "true";
			return "false";
		}
		if (type == VAR_CALLABLE) {
			return " <callable>";
		}
		if (type == VAR_FUNCTION) {
			return "< fn " + function->getName() +" >";
		}
		return "nil";
	}
	bool Literal::isBoolean() const {
		return type == VAR_BOOLEAN;
	}
	bool Literal::isNumber() const {
		return type == VAR_NUMBER;
	}
	bool Literal::isString() const {
		return type == VAR_STRING;
	}
	bool Literal::isCallable() const {
		return type == VAR_CALLABLE;
	}
	bool Literal::isCFunction() const {
		return type == VAR_CFUNCTION;
	}
	bool Literal::isFunction() const {
		return type == VAR_FUNCTION;
	}
	bool Literal::isLightUserData() const {
		return type == VAR_LIGHTUSERDATA;
	}
	bool Literal::isUserData() const {
		return type == VAR_USERDATA;
	}
	bool Literal::isNil() const {
		return type == VAR_NIL;
	}
	Token::Token(TokenType type, const std::string& lexeme, int line)
		: m_type(type), m_lexeme(lexeme), m_line(line) {

	}
	Token::Token(TokenType type, const std::string& lexeme, double number, int line)
		: m_type(type), m_lexeme(lexeme), m_line(line) {
		m_literal = Literal(number);
	}
	Token::Token(TokenType type, const std::string& lexeme, const char* str, int line)
		: m_type(type), m_lexeme(lexeme), m_line(line) {
		m_literal = Literal(str);
	}
	Token::Token(TokenType type, const std::string& lexeme, bool b, int line)
		: m_type(type), m_lexeme(lexeme), m_line(line) {
		m_literal = Literal(b);
	}
	void Token::free() {
		m_literal.free();
	}
	Token Token::copy() const {
		Token newToken;
		newToken.m_type = m_type;
		newToken.m_lexeme = m_lexeme;
		newToken.m_literal = m_literal.copy();
		newToken.m_line = m_line;
		return newToken;
	}
	void Token::setLiteral(const Literal& lit) {
		m_literal = lit;
	}
	void Token::setLexeme(const std::string& lexeme) {
		m_lexeme = lexeme;
	}
	void Token::setType(TokenType type) {
		m_type = type;
	}
}