#include "ZScriptFunctions.h"

#include <stdlib.h>


using namespace ZScript;

void executeScript(const std::string& text)
{
	std::vector < StmtPtr > stmt;
	parseBuffer(stmt, text);

	ZScript::Interpreter interpreter;
	interpreter.interpret(stmt);
}

int myFunction(int a, int b)
{
	return a + b - 1;
}

void interpret(Interpreter &ptr, const std::string &script)
{
	std::vector < StmtPtr > stmt;
	parseBuffer(stmt, script);
	ptr.interpret(stmt);
}

class MyClass
{
public:

	MyClass(int a, int b) : a(a), b(b) {}
	int a;
	int b;

	void doSomething() { printf("i did something a=%d, b=%d\n", a, b); }
};

int main()
{
	printf("---Test 1---- Simple Scripts \n");
	// Simple Scripts
	executeScript("print 1");
	executeScript("var a = 10; print a");
	executeScript("var b = 5; var c = 3; print b + c");

	printf("---Test 2---- Register C++ Function to Intepreter\n");
	// Register C++ Function to Intepreter
	ZScript::Interpreter interpreter;
	interpreter.subscribeFunction("myFunction", myFunction);
	interpret(interpreter, "print myFunction(10, 20)");

	printf("---Test 3---- Register C++ Class To Interpreter\n");
	// Register C++ Class To Interpreter
	interpreter.subscribeClass<MyClass>("MyClass")
		.addFunction("doSomething", &MyClass::doSomething);

	
	MyClass foo(2, 5);
	interpreter.getGlobalEnvironement()->define("foo", &foo);

	interpret(interpreter, "foo.doSomething()");
	getchar();
}