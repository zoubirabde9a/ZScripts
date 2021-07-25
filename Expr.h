#pragma once
#include "Stmt.h"
#include <list>
namespace ZScript {
	class Assign;
	class Binary;
	class Call;
	class FunctionExpr;
	class Grouping;
	class Logical;
	class Get;
	class Unary;
	class Variable;
	class LiteralExpr;
	class Stmt;
	class Expr {
	public:
		virtual ~Expr() {}
		template < typename R >
		class Visitor {
		public:
			virtual R visitAssignExpr(Assign* expr) = 0;
			virtual R visitBinaryExpr(Binary* expr) = 0;
			virtual R visitCallExpr(Call* expr) = 0;
			virtual R visitFunctionExpr(FunctionExpr* expr) = 0;
			virtual R visitGroupingExpr(Grouping* expr) = 0;
			virtual R visitLiteralExpr(LiteralExpr* expr) = 0;
			virtual R visitLogicalExpr(Logical* expr) = 0;
			virtual R visitGetExpr(Get* expr) = 0;
			virtual R visitUnaryExpr(Unary* expr) = 0;
			virtual R visitVariableExpr(Variable* expr) = 0;
		};
	public:
		virtual Literal accept(Visitor <Literal>* visitor) = 0;
		virtual Expr* copy() const = 0;
	};

	class Assign : public Expr {
	public:
		Assign(const Token& name, Expr* value);
		~Assign() override;
		Expr* copy() const override;
		Literal accept(Expr::Visitor <Literal>* visitor) override;
		inline const Token& getName() const { return m_name; }
		inline Expr* getValue() const { return m_value; }
	private:
		Token m_name;
		Expr* m_value = nullptr;
	};

	class Binary : public Expr {
	public:
		Binary(Expr* left, const Token &op, Expr* right);
		~Binary() override;
		Expr* copy() const override;
		Literal accept(Expr::Visitor < Literal >* visitor) override;
		inline Expr* getLeft() const { return m_left; }
		inline const Token& getOperator() const { return m_operator; }
		inline Expr* getRight() const { return m_right; }
	private:
		Expr* m_left = nullptr;
		Token m_operator;
		Expr* m_right = nullptr;
	};
	class Call : public Expr {
	public:
		Call(Expr* callee, const Token &paren, const std::list < Expr* > &arguments);
		~Call() override;
		Expr* copy() const override;
		Literal accept(Expr::Visitor < Literal >* visitor) override;
		inline Expr* getCallee() const { return m_callee; }
		inline const Token &getParen() const { return m_paren; }
		inline const std::list < Expr* >& getArguments() const { return m_arguments; }
	private:
		Expr* m_callee = nullptr;
		Token m_paren;
		std::list < Expr* > m_arguments;
	};
	class FunctionExpr : public Expr {
	public:
		FunctionExpr(const std::list < Token > &parameters, const std::list < StmtPtr >& body);
		~FunctionExpr() override;
		Expr* copy() const override;
		Literal accept(Expr::Visitor < Literal >* visitor) override;
		inline const std::list < Token >& getParameters() const { return m_parameters; }
		inline const std::list < StmtPtr >& getBody() const { return m_body; }
	private:
		std::list < Token > m_parameters;
		std::list < StmtPtr > m_body;
	};
	class Grouping : public Expr {
	public:
		Grouping(Expr* expression);
		~Grouping() override;
		Expr* copy() const override;
		Literal accept(Expr::Visitor<Literal>* visitor);
		Expr* getExpression() const { return m_expression; }
	private:
		Expr* m_expression = nullptr;
	};
	class LiteralExpr : public Expr {
	public:
		LiteralExpr(const Literal& value);
		~LiteralExpr() override;
		Expr* copy() const;
		Literal accept(Expr::Visitor<Literal>* visitor);
		inline const Literal& getValue() const { return m_value; }
	private:
		Literal m_value;
	};

	class Logical : public Expr {
	public:
		Logical(Expr* left, const Token &op, Expr* right);
		~Logical() override;
		Expr* copy() const override;
		Literal accept(Expr::Visitor<Literal>* visitor) override;
		inline Expr* getLeft() const { return m_left; }
		inline const Token& getOperator() const { return m_operator; }
		inline Expr* getRight() const { return m_right; }
	private:
		Expr* m_left = nullptr;
		Token m_operator;
		Expr* m_right = nullptr;
	};
	class Get : public Expr {
	public:
		Get(Expr* expr, const Token &name);
		~Get() override;
		Expr* copy() const override;
		Literal accept(Expr::Visitor<Literal>* visitor) override;
		inline Expr* getExpr() const { return m_expr; }
		inline const Token& getName() const { return m_name; }
	private:
		Expr* m_expr = nullptr;
		Token m_name;
	};
	class Unary : public Expr {
	public:
		Unary(const Token &op, Expr* right);
		~Unary() override;
		Expr* copy() const override;
		Literal accept(Expr::Visitor<Literal>* visitor) override;
		inline const Token& getOperator() const { return m_operator; }
		inline Expr* getRight() const { return m_right; }
	private:
		Token m_operator;
		Expr* m_right = nullptr;
	};

	class Variable : public Expr {
	public:
		Variable(const Token &name);
		~Variable() override;
		Expr* copy() const override;
		Literal accept(Expr::Visitor<Literal>* visitor) override;
		inline const Token& getName() const { return m_name; }
	private:
		Token m_name;
	};

}

