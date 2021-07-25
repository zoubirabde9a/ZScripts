#pragma once
#include "Interpreter.h"
namespace ZScript {
	/*template < typename R >
	void subscribeFunction(Interpreter* interpreter, const std::string& name, R(*fn)()) {
		interpreter->subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {		
			return Literal(fn());
		}, 0);
	}
	void subscribeFunction(Interpreter* interpreter, const std::string& name, void(*fn)()) {
		interpreter->subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
			fn();
			return Literal();
		}, 0);
	}

	template < typename R, typename Arg1 >
	void subscribeFunction(Interpreter* interpreter, const std::string& name, R(*fn)(Arg1)) {
		interpreter->subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
			return fn(arguments[0].cast < Arg1 >());
		}, 1);
	}
	template < typename Arg1 >
	void subscribeFunction(Interpreter* interpreter, const std::string& name, void(*fn)(Arg1)) {
		interpreter->subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
			fn(arguments[0].cast < Arg1 >());
			return Literal();
		}, 1);
	}

	template < typename R, typename Arg1, typename Arg2 >
	void subscribeFunction(Interpreter* interpreter, const std::string& name, R(*fn)(Arg1, Arg2)) {
		interpreter->subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
			return fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >());
		}, 2);
	}
	template < typename Arg1, typename Arg2  >
	void subscribeFunction(Interpreter* interpreter, const std::string& name, void(*fn)(Arg1, Arg2)) {
		interpreter->subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
			fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >());
			return Literal();
		}, 2);
	}

	template < typename R, typename Arg1, typename Arg2, typename Arg3 >
	void subscribeFunction(Interpreter* interpreter, const std::string& name, R(*fn)(Arg1, Arg2, Arg3)) {
		interpreter->subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
			return fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >());
		}, 3);
	}
	template < typename Arg1, typename Arg2, typename Arg3 >
	void subscribeFunction(Interpreter* interpreter, const std::string& name, void(*fn)(Arg1, Arg2, Arg3)) {
		interpreter->subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
			fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >());
				return Literal();
		}, 3);
	}

	template < typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4 >
	void subscribeFunction(Interpreter* interpreter, const std::string& name, R(*fn)(Arg1, Arg2, Arg3, Arg4)) {
		interpreter->subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
			return fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >(), arguments[3].cast < Arg4 >());
		}, 4);
	}
	template < typename Arg1, typename Arg2, typename Arg3, typename Arg4 >
	void subscribeFunction(Interpreter* interpreter, const std::string& name, void(*fn)(Arg1, Arg2, Arg3, Arg4)) {
		interpreter->subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
			fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >(), arguments[3].cast < Arg4 >());
			return Literal();
		}, 4);
	}

	template < typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5 >
	void subscribeFunction(Interpreter* interpreter, const std::string& name, R(*fn)(Arg1, Arg2, Arg3, Arg4, Arg5)) {
		interpreter->subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
			return fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >(), arguments[3].cast < Arg4 >(), arguments[4].cast < Arg5 >());
		}, 5);
	}
	template < typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5 >
	void subscribeFunction(Interpreter* interpreter, const std::string& name, void(*fn)(Arg1, Arg2, Arg3, Arg4, Arg5)) {
		interpreter->subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
			fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >(), arguments[3].cast < Arg4 >(), arguments[4].cast < Arg5 >());
			return Literal();
		}, 5);
	}

	template < typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6 >
	void subscribeFunction(Interpreter* interpreter, const std::string& name, R(*fn)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)) {
		interpreter->subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
			return fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >(), arguments[3].cast < Arg4 >(), arguments[4].cast < Arg5 >(), , arguments[5].cast < Arg6 >());
		}, 6);
	}
	template < typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6 >
	void subscribeFunction(Interpreter* interpreter, const std::string& name, void(*fn)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)) {
		interpreter->subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
			fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >(), arguments[3].cast < Arg4 >(), arguments[4].cast < Arg5 >(), arguments[5].cast < Arg6 >());
			return Literal();
		}, 6);
	}
	template < typename T , typename R >
	LoxCallable MemberFunctionBuilder(R(T::*fn)()) {
		return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
			return ((T*)(arguments[0].lightUserData)->*fn)();
		}, 0);
	}
	template < typename T>
	LoxCallable MemberFunctionBuilder(void(T::*fn)()) {
		return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
			((T*)((arguments[0].lightUserData))->*fn)();
			return Literal();
		}, 0);
	}
	template < typename T, typename R, typename Arg1 >
	LoxCallable MemberFunctionBuilder(R(T::*fn)(Arg1)) {
		return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
			return ((T*)(arguments[0].lightUserData)->*fn)(arguments[1].cast < Arg1 > ());
		}, 1);
	}
	template < typename T, typename Arg1 >
	LoxCallable MemberFunctionBuilder(void(T::*fn)(Arg1)) {
		return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
			((T*)(arguments[0].lightUserData)->*fn)(arguments[1].cast < Arg1 >());
			return Literal();
		}, 1);
	}
	template < typename T >
	class ClassSubscriber {
	public:
		ClassSubscriber(CClass* cclass): m_class(cclass){}
		template < typename FN >
		ClassSubscriber& addFunction(const std::string& name, FN const fp) {
			m_class->addFunction(name, MemberFunctionBuilder < T >(fp));
			return *this;
		}
	private:
		CClass* m_class;
	};
	template < typename T >
	ClassSubscriber < T > subscribeClass(Interpreter* interpreter, const std::string& name) {
		return ClassSubscriber < T >(interpreter->addCClass < T >(name));
	}*/
}
