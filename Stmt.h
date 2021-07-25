#pragma once
#include <list>
#include "Token.h"
#include <memory>
namespace ZScript {
	class Block;
	class Expression;
	class FunctionStmt;
	class If;
	class Print;
	class Return;
	class Var;
	class While;
	class Expr;
	class Stmt {
	public:
		virtual ~Stmt() {}
		template < typename R >
		class Visitor {
		public:
			virtual R visitBlockStmt(Block* stmt) = 0;
			virtual R visitExpressionStmt(Expression* stmt) = 0;
			virtual R visitFunctionStmt(FunctionStmt* stmt) = 0;
			virtual R visitIfStmt(If* stmt) = 0;
			virtual R visitPrintStmt(Print* stmt) = 0;
			virtual R visitReturnStmt(Return* stmt) = 0;
			virtual R visitVarStmt(Var* stmt) = 0;
			virtual R visitWhileStmt(While* stmt) = 0;
		};
		//virtual Stmt* copy() const = 0;
		virtual void accept(Visitor <void>* visitor) = 0;
	};
	typedef std::shared_ptr < Stmt > StmtPtr;
	class Block : public Stmt {
	public:
		Block(const std::list < StmtPtr >& statements);
		~Block() override;
		//Stmt* copy() const override;
		void accept(Visitor <void>* visitor) override;
		inline const std::list < StmtPtr > &getStatements() const { return m_statements; }
	private:
		std::list < StmtPtr > m_statements;
	};

	class Expression : public Stmt {
	public:
		Expression(Expr* expression);
		~Expression() override;
		//Stmt* copy() const override;
		void accept(Visitor <void>* visitor) override;
		inline Expr* getExpression() const { return m_expression; }
	private:
		Expr* m_expression = nullptr;
	};
	class FunctionStmt : public Stmt {
	public:
		FunctionStmt(const Token& name, const std::list < Token > & parameters,
			const std::list < StmtPtr > &body);
		~FunctionStmt() override;
		//Stmt* copy() const override;
		void accept(Visitor < void >* visitor) override;

		inline const Token& getName() const { return m_name; }
		inline const std::list < Token >& getParameters() const { return m_parameters; }
		inline const std::list < StmtPtr >& getBody() const { return m_body; }
	private:
		Token m_name;
		std::list < Token > m_parameters;
		std::list < StmtPtr > m_body;
	};
	class If : public Stmt {
	public:
		If(Expr* condition, StmtPtr thenBranch, StmtPtr elseBranch);
		~If() override;
		//Stmt* copy() const override;
		void accept(Visitor <void>* visitor) override;
		inline Expr* getCondition() const { return m_condition; }
		inline StmtPtr getThenBranch() const { return m_thenBranch; }
		inline StmtPtr getElseBranch() const { return m_elseBranch; }
	private:
		Expr* m_condition = nullptr;
		StmtPtr m_thenBranch = nullptr;
		StmtPtr m_elseBranch = nullptr;
	};

	class Print : public Stmt {
	public:
		Print(Expr* expression);
		~Print() override;
		//Stmt* copy() const override;
		void accept(Visitor <void>* visitor) override;
		inline Expr* getExpression() const { return m_expression; }
	private:
		Expr* m_expression = nullptr;
	};
	class Return : public Stmt {
	public:
		Return(const Token& keyword, Expr* value);
		~Return() override;
		//Stmt* copy() const override;
		void accept(Visitor <void>* visitor) override;
		inline const Token& getKeyword() const { return m_keyword; }
		inline Expr* getValue() const { return m_value; }
	private:
		Token m_keyword;
		Expr* m_value = nullptr;
	};

	class Var : public Stmt {
	public:
		Var(const Token& name, Expr* initializer);
		~Var() override;
		//Stmt* copy() const override;
		void accept(Visitor<void>* visitor) override;
		inline const Token& getName() const { return m_name; }
		inline Expr* getInitializer() const { return m_initializer; }
	private:
		Token m_name;
		Expr* m_initializer = nullptr;
	};

	class While : public Stmt {
	public:
		While(Expr* condition, StmtPtr body);
		~While() override;
		//Stmt* copy() const override;
		void accept(Visitor<void>* visitor) override;
		inline Expr* getCondition() const { return m_condition; }
		inline StmtPtr getBody() const { return m_body; }
	private:
		Expr* m_condition = nullptr;
		StmtPtr m_body = nullptr;
	};
};


