#pragma once
#include <string>
#include <functional>
#include <list>
#include <exception>
#include "TypeDefs.h"
#include <memory>
#include <typeinfo>
#include "HelperFunctions.hpp"

namespace ZScript {
	class Interpreter;
	class LoxCallable;
	class LoxCFunction;
	class LoxFunction;

	class Literal {
	public:
		Literal();
		Literal(bool boolean);
		Literal(double number);
		Literal(float number);
		Literal(size_t number);
		Literal(int number);
		Literal(const std::string& str);
		Literal(const char* str);
		Literal(char* str);
		Literal(void* lightUserData, size_t classId) : type(VAR_LIGHTUSERDATA), userData(lightUserData), classId(classId) {}
		template < typename T >
		Literal(T* lightUserData) : type(VAR_LIGHTUSERDATA), userData(lightUserData), classId(typeid(T).hash_code()) {}
		template < typename T >
		Literal(const T& userData) : type(VAR_USERDATA), userData(shared_create < T > (userData)), classId(typeid(T).hash_code()) {}
		template <>
		Literal(const LoxCallable* callable) : type(VAR_CALLABLE), callable(callable), callPtr(nullptr), classId(0) {}
		template <>
		Literal(LoxCallable* callable) : type(VAR_CALLABLE), callable(callable), callPtr(nullptr), classId(0) {}
		Literal(const LoxCallable* callable, void* callPtr, size_t calssId) : type(VAR_CALLABLE), callable(callable), callPtr(callPtr), classId(classId) {}
		Literal(LoxCallable* callable, void* callPtr, size_t calssId) : type(VAR_CALLABLE), callable(callable), callPtr(callPtr), classId(classId) {}
		template <>
		Literal(const LoxFunction* function) : type(VAR_FUNCTION), function(function) {}
		template <>
		Literal(LoxCFunction* cFunction) : type(VAR_CFUNCTION), cFunction(cFunction) {}


		void free();
		Literal copy() const;

		std::string toString() const;

		bool isBoolean() const;
		bool isNumber() const;
		bool isString() const;
		bool isCallable() const;
		bool isCFunction() const;
		bool isFunction() const;
		bool isLightUserData() const;
		bool isUserData() const;
		bool isNil() const;

		union {
			// Boolean
			struct {
				bool boolean;
			};
			// Number
			struct {
				double number;
			};
			// String
			struct {
				char* string;
				size_t size;
			};
			// Callable
			struct {
				const LoxCallable* callable;
				void* callPtr;
				size_t classId;
			};
			struct {
				const LoxFunction* function;
			};
			struct {
				LoxCFunction* cFunction;
			};
			struct {
				void* userData;
				size_t classId;
			};
		};
		VarType type;


		template < typename T>	
		struct RemoveRef {
			typedef T type;
		};
		template < typename T>
		struct RemoveRef < typename T&>{
			typedef T type;
		};
		template < typename T>
		struct RemoveRef < typename T&&> {
			typedef T type;
		};
		template < typename T>
		struct RemoveRef < typename T const &> {
			typedef T type;
		};


		template < typename T>
		struct RefStr {
			static T ref(const Literal* lit) {
				return *(T*)(lit->userData);
			}
		};
		template < typename T>
		struct RefStr < typename T* >{
			static T* ref(const Literal* lit) {
				return (T*)(lit->userData);
			}
		};
		template <>
		struct RefStr  < std::string >{
			static std::string ref(const Literal* lit) {
				return lit->string;
			}
		};
		template <>
		struct RefStr  < float >{
			static float ref(const Literal* lit) {
				return lit->number;
			}
		};
		template <>
		struct RefStr < int >{
			static int ref(const Literal* lit) {
				return lit->number;
			}
		};
		template <>
		struct RefStr < double >{
			static double ref(const Literal* lit) {
				return lit->number;
			}
		};
		template <>
		struct RefStr < unsigned int > {
			static unsigned int ref(const Literal* lit) {
				return lit->number;
			}
		};
		template <>
		struct RefStr < const char* >{
			static const char* ref(const Literal* lit) {
				return lit->string;
			}
		};
		template <>
		struct RefStr < bool >{
			static bool ref(const Literal* lit) {
				return lit->boolean;
			}
		};

		template < typename T >
		auto cast() const {
			return RefStr < RemoveRef < T >::type >::ref(this);
		}
		/*template <>
		float cast() const {
			return number;
		}
		template <>
		char* cast() const {
			return string;
		}
		template <>
		const char* cast() const {
			return string;
		}*/
	};
	class ReturnException : public std::exception {
	public:
		ReturnException(const Literal& value) : std::exception() {
			m_value = value;
		}
		inline const Literal& getValue() const { return m_value; }
	private:
		Literal m_value;

	};
	class Token {
	public:
		Token() {};
		Token(TokenType type, const std::string& lexeme, int line);
		Token(TokenType type, const std::string& lexeme, double number, int line);
		Token(TokenType type, const std::string& lexeme, const char* str, int line);
		Token(TokenType type, const std::string& lexeme, bool b, int line);

		void free();
		Token copy() const;

		void setLiteral(const Literal& lit);
		void setLexeme(const std::string& lexeme);
		void setType(TokenType type);

		const Literal& getLiteral() const { return m_literal; }
		const std::string &getLexeme() const { return m_lexeme; }
		TokenType getType() const { return m_type; }
	private:
		TokenType m_type;
		std::string m_lexeme;
		Literal m_literal;
		size_t m_line;
	};
}
