#pragma once
#include "Expr.h"
#include "Stmt.h"
#include "Environment.h"
#include "RuntimeError.h"
#include "LoxCallable.h"
#include "LoxCFunction.h"
#include "CClass.h"
//#include "..\ZTime.hpp"
namespace ZScript {
	class Interpreter;
	class Session {
	public:
		Session(const std::vector < StmtPtr >& statements, Environment* environement) : m_statements(statements), m_environement(environement){}

		void setStatements(const std::vector < StmtPtr > &statements);

		void update(Interpreter* interpreter);
		void wait(float time);
		void wait();
		void resume();
		inline Environment* getEnvironement() { return m_environement; }
		inline bool isFinished() const { return m_cs == m_statements.size(); }
	private:
		Environment* m_environement;
		std::vector < StmtPtr > m_statements;
		size_t m_cs = 0;
		float m_waitTime = 0.0f;
		bool m_wait = false;;
		//ZEngine::Timer m_timer;
	};

	class Interpreter : public Expr::Visitor < Literal >, Stmt::Visitor < void >
	{
	public:
		static bool isTruthy(const Literal& object);
	public:
		Interpreter();
		~Interpreter();
		void updateSessions();
		Session* createSession(const std::vector < StmtPtr >& statements);
		Session* createSession(const std::vector < StmtPtr >& statements, Environment* environment);
		void interpret(const std::vector < StmtPtr >& statements);
		void interpret(const std::vector < StmtPtr >& statements, Environment* environment);
		Environment* interpretVariables(const std::vector < StmtPtr > &statements);
		void subscribeCFunction(const std::string& name, CFunction fn);
		void subscribeFunction(const std::string&name, const Function& fn, size_t arity);
		Literal visitLiteralExpr(LiteralExpr* expr) override;
		Literal visitLogicalExpr(Logical* expr) override;
		Literal visitGroupingExpr(Grouping* expr) override;
		Literal visitGetExpr(Get* expr) override;
		Literal visitUnaryExpr(Unary* expr)override;
		Literal visitVariableExpr(Variable* expr) override;
		Literal visitBinaryExpr(Binary* expr) override;
		Literal visitCallExpr(Call* expr) override;
		Literal visitFunctionExpr(FunctionExpr* expr) override;
		Literal visitAssignExpr(Assign* expr) override;
		void visitExpressionStmt(Expression* stmt) override;
		void visitFunctionStmt(FunctionStmt* stmt) override;
		void visitIfStmt(If* stmt)override;
		void visitPrintStmt(Print* stmt) override;
		void visitReturnStmt(Return* stmt) override;
		void visitVarStmt(Var* stmt) override;
		void visitWhileStmt(While* stmt) override;
		void visitBlockStmt(Block* stmt) override;
		void executeBlock(const std::list <StmtPtr>& statements, Environment* environment);
		void executeBlock(const std::vector <StmtPtr>& statements, Environment* environment);




		template < typename R >
		void subscribeFunction(const std::string& name, R(*fn)()) {
			subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				return Literal(fn());
			}, 0);
		}
		void subscribeFunction(const std::string& name, void(*fn)()) {
			subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				fn();
				return Literal();
			}, 0);
		}

		template < typename R, typename Arg1 >
		void subscribeFunction(const std::string& name, R(*fn)(Arg1)) {
			subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				return fn(arguments[0].cast < Arg1 >());
			}, 1);
		}
		template < typename Arg1 >
		void subscribeFunction(const std::string& name, void(*fn)(Arg1)) {
			subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				fn(arguments[0].cast < Arg1 >());
				return Literal();
			}, 1);
		}
		template < typename R, typename Arg1, typename Arg2 >
		void subscribeFunction(const std::string& name, R(*fn)(Arg1, Arg2)) {
			subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				return fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >());
			}, 2);
		}
		template < typename Arg1, typename Arg2  >
		void subscribeFunction(const std::string& name, void(*fn)(Arg1, Arg2)) {
			subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >());
				return Literal();
			}, 2);
		}

		template < typename R, typename Arg1, typename Arg2, typename Arg3 >
		void subscribeFunction(const std::string& name, R(*fn)(Arg1, Arg2, Arg3)) {
			subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				return fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >());
			}, 3);
		}
		template < typename Arg1, typename Arg2, typename Arg3 >
		void subscribeFunction(const std::string& name, void(*fn)(Arg1, Arg2, Arg3)) {
			subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >());
				return Literal();
			}, 3);
		}

		template < typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4 >
		void subscribeFunction(const std::string& name, R(*fn)(Arg1, Arg2, Arg3, Arg4)) {
			subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				return fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >(), arguments[3].cast < Arg4 >());
			}, 4);
		}
		template < typename Arg1, typename Arg2, typename Arg3, typename Arg4 >
		void subscribeFunction(const std::string& name, void(*fn)(Arg1, Arg2, Arg3, Arg4)) {
			subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >(), arguments[3].cast < Arg4 >());
				return Literal();
			}, 4);
		}

		template < typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5 >
		void subscribeFunction(const std::string& name, R(*fn)(Arg1, Arg2, Arg3, Arg4, Arg5)) {
			subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				return fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >(), arguments[3].cast < Arg4 >(), arguments[4].cast < Arg5 >());
			}, 5);
		}
		template < typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5 >
		void subscribeFunction(const std::string& name, void(*fn)(Arg1, Arg2, Arg3, Arg4, Arg5)) {
			subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >(), arguments[3].cast < Arg4 >(), arguments[4].cast < Arg5 >());
				return Literal();
			}, 5);
		}

		template < typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6 >
		void subscribeFunction(const std::string& name, R(*fn)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)) {
			subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				return fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >(), arguments[3].cast < Arg4 >(), arguments[4].cast < Arg5 >(), , arguments[5].cast < Arg6 >());
			}, 6);
		}
		template < typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6 >
		void subscribeFunction(const std::string& name, void(*fn)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)) {
			subscribeFunction(name, [=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				fn(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >(), arguments[3].cast < Arg4 >(), arguments[4].cast < Arg5 >(), arguments[5].cast < Arg6 >());
				return Literal();
			}, 6);
		}

		private:

		template < typename T, typename R >
		static LoxCallable MemberFunctionBuilder(R(T::*fn)()) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				return ((T*)(arguments[0].userData)->*fn)();
			}, 0);
		}
		template < typename T, typename R >
		static LoxCallable MemberFunctionBuilder(R(T::*fn)() const) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				return ((T*)(arguments[0].userData)->*fn)();
			}, 0);
		}
		template < typename T>
		static  LoxCallable MemberFunctionBuilder(void(T::*fn)()) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				((T*)((arguments[0].userData))->*fn)();
				return Literal();
			}, 0);
		}
		template < typename T>
		static LoxCallable MemberFunctionBuilder(void(T::*fn)() const) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				((T*)((arguments[0].userData))->*fn)();
				return Literal();
			}, 0);
		}



		template < typename T, typename R, typename Arg1 >
		static LoxCallable MemberFunctionBuilder(R(T::*fn)(Arg1)) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				return ((T*)(arguments[1].userData)->*fn)(arguments[0].cast < Arg1 >());
			}, 1);
		}
		template < typename T, typename R, typename Arg1 >
		static LoxCallable MemberFunctionBuilder(R(T::*fn)(Arg1) const) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				return ((T*)(arguments[1].userData)->*fn)(arguments[0].cast < Arg1 >());
			}, 1);
		}
		template < typename T, typename Arg1 >
		static LoxCallable MemberFunctionBuilder(void(T::*fn)(Arg1)) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				((T*)(arguments[1].userData)->*fn)(arguments[0].cast < Arg1 >());
				return Literal();
			}, 1);
		}
		template < typename T, typename Arg1 >
		static LoxCallable MemberFunctionBuilder(void(T::*fn)(Arg1) const) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				((T*)(arguments[1].userData)->*fn)(arguments[0].cast < Arg1 >());
				return Literal();
			}, 1);
		}



		template < typename T, typename R, typename Arg1, typename Arg2 >
		static LoxCallable MemberFunctionBuilder(R(T::*fn)(Arg1, Arg2)) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				return ((T*)(arguments[2].userData)->*fn)(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >());
			}, 2);
		}
		template < typename T, typename R, typename Arg1, typename Arg2 >
		static LoxCallable MemberFunctionBuilder(R(T::*fn)(Arg1, Arg2) const) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				return ((T*)(arguments[2].userData)->*fn)(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >());
			}, 2);
		}
		template < typename T, typename Arg1, typename Arg2 >
		static LoxCallable MemberFunctionBuilder(void(T::*fn)(Arg1, Arg2)) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				((T*)(arguments[2].userData)->*fn)(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >());
				return Literal();
			}, 2);
		}
		template < typename T, typename Arg1, typename Arg2 >
		static LoxCallable MemberFunctionBuilder(void(T::*fn)(Arg1, Arg2) const) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				((T*)(arguments[2].userData)->*fn)(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >());
				return Literal();
			}, 2);
		}



		template < typename T, typename R, typename Arg1, typename Arg2, typename Arg3 >
		static LoxCallable MemberFunctionBuilder(R(T::*fn)(Arg1, Arg2, Arg3)) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				return ((T*)(arguments[3].userData)->*fn)(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >());
			}, 3);
		}
		template < typename T, typename R, typename Arg1, typename Arg2, typename Arg3 >
		static LoxCallable MemberFunctionBuilder(R(T::*fn)(Arg1, Arg2, Arg3) const) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				return ((T*)(arguments[3].userData)->*fn)(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >());
			}, 3);
		}
		template < typename T, typename Arg1, typename Arg2, typename Arg3 >
		static LoxCallable MemberFunctionBuilder(void(T::*fn)(Arg1, Arg2, Arg3)) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				((T*)(arguments[3].userData)->*fn)(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >());
				return Literal();
			}, 3);
		}
		template < typename T, typename Arg1, typename Arg2, typename Arg3 >
		static LoxCallable MemberFunctionBuilder(void(T::*fn)(Arg1, Arg2, Arg3) const) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				((T*)(arguments[3].userData)->*fn)(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >());
				return Literal();
			}, 3);
		}




		template < typename T, typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4 >
		static LoxCallable MemberFunctionBuilder(R(T::*fn)(Arg1, Arg2, Arg3, Arg4)) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				return ((T*)(arguments[4].userData)->*fn)(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >(), arguments[3].cast < Arg4 >());
			}, 4);
		}
		template < typename T, typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4 >
		static LoxCallable MemberFunctionBuilder(R(T::*fn)(Arg1, Arg2, Arg3, Arg4) const) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				return ((T*)(arguments[4].userData)->*fn)(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >(), arguments[3].cast < Arg4 >());
			}, 4);
		}
		template < typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4 >
		static LoxCallable MemberFunctionBuilder(void(T::*fn)(Arg1, Arg2, Arg3, Arg4)) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				((T*)(arguments[4].userData)->*fn)(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >(), arguments[3].cast < Arg4 >());
				return Literal();
			}, 4);
		}
		template < typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4 >
		static LoxCallable MemberFunctionBuilder(void(T::*fn)(Arg1, Arg2, Arg3, Arg4) const) {
			return LoxCallable([=](Interpreter* interpreter, const std::vector < Literal >& arguments) -> Literal {
				((T*)(arguments[4].userData)->*fn)(arguments[0].cast < Arg1 >(), arguments[1].cast < Arg2 >(), arguments[2].cast < Arg3 >(), arguments[3].cast < Arg4 >());
				return Literal();
			}, 4);
		}



		public:

		template < typename T >
		CClass* addCClass(CClass* parent, const std::string& name) {
			return &m_cclasses.insert(std::make_pair(typeid(T).hash_code(), CClass(parent, name))).first->second;
		}

		CClass* getCClass(size_t id) {
			auto it = m_cclasses.find(id);
			if (it != m_cclasses.end()) {
				return &it->second;
			}
			else return nullptr;
		}

		private:

		template < typename T >
		class ClassSubscriber {
		public:
			ClassSubscriber(Interpreter* interpreter, CClass* cclass) : m_interpreter(interpreter), m_class(cclass) {}
			template < typename FN >
			ClassSubscriber& addFunction(const std::string& name, FN const fp) {
				m_class->addFunction(name, MemberFunctionBuilder < T >(fp));
				return *this;
			}
			template < typename C >
			ClassSubscriber < C > deriveClass(const std::string& name) {
				return ClassSubscriber < C >(m_interpreter, m_interpreter->addCClass < C >(m_class, name));
			}
		private:
			CClass* m_class;
			Interpreter* m_interpreter;
		};

		public:

		template < typename T >
		ClassSubscriber < T > subscribeClass(const std::string& name) {
			return ClassSubscriber < T >(this, addCClass < T >(nullptr, name));
		}


		Literal evaluateExpressionStmt(Expression* exprStmt);
		Literal evaluateExpressionStmt(Expression* exprStmt, Environment* environment);

		inline Environment* getGlobalEnvironement() { return m_globals; }
		inline Environment* getRegestryEnvironment() { return m_regestry; }
		inline Environment* getEnvironment() { return m_environment; }
		inline const std::unordered_map < size_t, CClass > &getCClasses() const { return m_cclasses; }
	private:
		friend class Session;
		void execute(StmtPtr stmt);
		void execute(StmtPtr stmt, Environment* environement);
		Literal evaluate(Expr* expr);
		bool isEqual(const Literal& a, const Literal& b);
		void checkNumberOperand(const Token &op, const Literal& operand);
		void checkNumberOperands(const Token &op, const Literal& left, const Literal& right);
		std::string stringify(const Literal& object);
		void error(RuntimeError& err);
	private:
		Environment* m_environment;
		Environment* m_globals;
		Environment* m_regestry;
		std::list < Session > m_sessions;
		std::unordered_map < size_t, CClass > m_cclasses;
	};
}
