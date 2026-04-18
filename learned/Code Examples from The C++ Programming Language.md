# 你好，世界

```c++
// The Hello world program.

// pg 46, sec 3.2, Hello, World!

// No guarantees offered. Constructive comments to bs@research.att.com



#include <iostream>

int main()
{
	std::cout << "Hello, world!\n";
}

// note that "return 0;" isn't required in ISO C++
```



# 	厘米转换

```c++
// Simple inch<->cm conversion program illustrating basic I/O and computation.

// pg 50, sec 3.6, Input

// No guarantees offered. Constructive comments to bs@research.att.com



#include<iostream>

using namespace std;


int main()
{
	const float factor = 2.54;	// 1 inch equals 2.54 cm
	float x, in, cm;
	char ch = 0;

	cout << "enter length: ";

	cin >> x;		// read a floating-point number
	cin >> ch;		// read a suffix

	switch (ch) {
	case 'i':		// inch
		in = x;
		cm = x*factor;
		break;
	case 'c':		// cm
		in = x/factor;
		cm = x;
		break;
	default:
		in = cm = 0;
		break;
	}

	cout << in << " in = " << cm << " cm\n";

	return 0;	// redundant in ISO C++
}
```



# 页列表实例

```c++
// example illustrating the use of std::list.

// This is *not* an example of good style, just an illustration of facilities

// pp 54-55, sec 3.7.3

// No guarantees offered. Constructive comments to bs@research.att.com


#include<list>
#include<iostream>
#include<string>

using namespace std;


struct Entry {
	string name;
	int number;
	Entry(const string& n, int i) :name(n), number(i) { }
};

list<Entry> phone_book;

void print_entries()
/*
	this kind of function should use a parameter,
	rather then a global name
*/
{
	typedef list<Entry>::const_iterator LI;

	for (LI i = phone_book.begin(); i != phone_book.end(); ++i) {
		const Entry& e = *i;	// reference used as shorthand
			cout << '{' << e.name << ' ' << e.number << "}\n";
	}
}

void print_entry(const string& s)
/*
	Is this the right treatment of a string not found?
*/
{
	typedef list<Entry>::const_iterator LI;

	for (LI i = phone_book.begin(); i != phone_book.end(); ++i) {
		const Entry& e = *i;	// reference used as shorthand
		if (s == e.name) {
			cout << e.name << ' ' << e.number << '\n';
			return;
		}
	}
}

void f(const Entry& e, list<Entry>::iterator i, list<Entry>::iterator p)
/*
	just some nonsense code
*/
{
	phone_book.push_front(e);	// add at beginning
	phone_book.push_back(e);	// add at end
	phone_book.insert(i,e);		// add before the element referred to by `i'

	phone_book.erase(p);		// remove the element referred to by `p'
}

int main()
{

	phone_book.push_back(Entry("one",1));
	phone_book.push_back(Entry("two",2));
	phone_book.push_back(Entry("three",3));
	phone_book.push_back(Entry("four",4));
	phone_book.push_back(Entry("five",5));
	Entry six("six",6);
	print_entries();
	f(six,phone_book.begin(),phone_book.begin());
	print_entries();
	print_entry("four");
	print_entry("seven");
	print_entry("three");
}
```



# 计数字符串中字符的出现次数

```c++
// Count occurrences of character in string using find()
// to illustrate simple use of iterators and algorithms.

// pp 57-58, sec 3.8.1, Use of Iterators

// No guarantees offered. Constructive comments to bs@research.att.com


#include<iostream>
#include<algorithm>
#include<string>

using namespace std;


int count(const string& s, char c)	// count occurrences of c in s
{
	int n = 0;
	string::const_iterator i = find(s.begin(),s.end(),c);
	while (i != s.end()) {
		++n;
		i = find(i+1,s.end(),c);
	}
	return n;
}


void f()
{
	string m = "Mary had a little lamb";
	int a_count = count(m,'a');

	cout << "\"" << m << "\" contains " << a_count << " 'a's\n";
}

int main()
{
	f();

	return 0;	// redundant in ISO C++
}
```



# 各容器中的元素计数

```c++
// Count occurrences of elements in various containers using find()
// to illustrate generic algoriths and iterators.

// pp 58-59, sec 3.8.1, Use of Iterators

// No guarantees offered. Constructive comments to bs@research.att.com


#include<iostream>
#include<algorithm>
#include<string>
#include<complex>
#include<vector>
#include<list>

using namespace std;

typedef complex<double> dcomplex;	// In the book, I cheated slightly, sorry
					// the standard library complex is a template



template<class C, class T> int count(const C& v, T val)
{
	typename C::const_iterator i = find(v.begin(),v.end(),val);	// "typename;" see C.13.5
	int n = 0;
	while (i != v.end()) {
		++n;
		++i;	// skip past the element we just found
		i = find(i,v.end(),val);
	}
	return n;
}

void f(list<dcomplex>& lc, vector<string>& vc, string s)
{
	int i1 = count(lc,dcomplex(1,3));
	int i2 = count(vc,"Chrysippus");
	int i3 = count(s,'x');

	cout << "number of complex(1,3)s " << i1 << "\n";
	cout << "number of \"Chrysippus\"s " << i2 << "\n";
	cout << "number of 'x's in \"" << s << "\" : " << i3 << "\n";
}

int main()
{
	vector<string> philo;
	philo.push_back("Aristotle");
	philo.push_back("Plato");
	philo.push_back("Chrysippus");
	philo.push_back("Zeno");

	list<dcomplex> spline;
	spline.push_back(dcomplex(1,3));
	spline.push_back(dcomplex(1,7));
	spline.push_back(dcomplex(1,3));
	spline.push_back(dcomplex(7,3));

	string boat = "Exxon Valdez";

	f(spline,philo,boat);

	return 0;	// redundant in ISO C++
}
```



# 从文件中排序单词

```C++
// Count occurrences of elements in various containers using find()
// to illustrate generic algoriths and iterators.

// pp 58-59, sec 3.8.1, Use of Iterators

// No guarantees offered. Constructive comments to bs@research.att.com


#include<iostream>
#include<algorithm>
#include<string>
#include<complex>
#include<vector>
#include<list>

using namespace std;

typedef complex<double> dcomplex;	// In the book, I cheated slightly, sorry
					// the standard library complex is a template



template<class C, class T> int count(const C& v, T val)
{
	typename C::const_iterator i = find(v.begin(),v.end(),val);	// "typename;" see C.13.5
	int n = 0;
	while (i != v.end()) {
		++n;
		++i;	// skip past the element we just found
		i = find(i,v.end(),val);
	}
	return n;
}

void f(list<dcomplex>& lc, vector<string>& vc, string s)
{
	int i1 = count(lc,dcomplex(1,3));
	int i2 = count(vc,"Chrysippus");
	int i3 = count(s,'x');

	cout << "number of complex(1,3)s " << i1 << "\n";
	cout << "number of \"Chrysippus\"s " << i2 << "\n";
	cout << "number of 'x's in \"" << s << "\" : " << i3 << "\n";
}

int main()
{
	vector<string> philo;
	philo.push_back("Aristotle");
	philo.push_back("Plato");
	philo.push_back("Chrysippus");
	philo.push_back("Zeno");

	list<dcomplex> spline;
	spline.push_back(dcomplex(1,3));
	spline.push_back(dcomplex(1,7));
	spline.push_back(dcomplex(1,3));
	spline.push_back(dcomplex(7,3));

	string boat = "Exxon Valdez";

	f(spline,philo,boat);

	return 0;	// redundant in ISO C++
}
```



# 桌面计数器

```c++
// The desk calculator

// includes character-level input (sec6.1.3), but
// no command line input (sec6.1.7),
// no namespaces, and
// no exceptions

// pp 107-117, sec 6.1, A Desk calculator


// uses += rather than push_back() for string
// to work around standard library bug

// No guarantees offered. Constructive comments to bs@research.att.com


/*
    program:
	END			   // END is end-of-input
	expr_list END

    expr_list:
	expression PRINT	   // PRINT is semicolon
	expression PRINT expr_list

    expression:
	expression + term
	expression - term
	term

    term:
	term / primary
	term * primary
	primary

    primary:
	NUMBER
	NAME
	NAME = expression
	- primary
	( expression )
*/

#include <string>
#include <cctype>
#include<iostream>
#include<map>

using namespace std;

int no_of_errors;	// note: default initialized to 0

double error(const char* s)
{
    no_of_errors++;
    cerr << "error: " << s << '\n';
    return 1;
}

enum Token_value {
	NAME,		NUMBER,		END,
	PLUS='+',	MINUS='-',	MUL='*',	DIV='/',
	PRINT=';',	ASSIGN='=',	LP='(',		RP=')'
};

Token_value curr_tok = PRINT;
double number_value;
string string_value;

/* The simplest token reader

Token_value get_token()
{
	char ch = 0;
	cin>>ch;

	switch (ch) {
	case 0:
		return curr_tok=END;
	case ';':
	case '*':
	case '/':
	case '+':
	case '-':
	case '(':
	case ')':
	case '=':
		return curr_tok=Token_value(ch);
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	case '.':
		cin.putback(ch);
		cin >> number_value;
		return curr_tok=NUMBER;
	default:					// NAME, NAME =, or error
		if (isalpha(ch)) {
			cin.putback(ch);
			cin>>string_value;
			return curr_tok=NAME;
		}
		error("bad token");
		return curr_tok=PRINT;
	}
}
*/

Token_value get_token()
{
	char ch;

	do {	// skip whitespace except '\en'
		if(!cin.get(ch)) return curr_tok = END;
	} while (ch!='\n' && isspace(ch));

	switch (ch) {
	case ';':
	case '\n':
		return curr_tok=PRINT;
	case '*':
	case '/':
	case '+':
	case '-':
	case '(':
	case ')':
	case '=':
		return curr_tok=Token_value(ch);
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	case '.':
		cin.putback(ch);
		cin >> number_value;
		return curr_tok=NUMBER;
	default:			// NAME, NAME=, or error
		if (isalpha(ch)) {
			string_value = ch;
			while (cin.get(ch) && isalnum(ch))
				string_value += ch;	// string_value.push_back(ch);
							// to work around library bug
			cin.putback(ch);
			return curr_tok=NAME;
		}
		error("bad token");
		return curr_tok=PRINT;
	}
}

map<string,double> table;

double expr(bool);	// cannot do without


double prim(bool get)		// handle primaries
{
	if (get) get_token();

	switch (curr_tok) {
	case NUMBER:		// floating-point constant
	{	double v = number_value;
		get_token();
		return v;
	}
	case NAME:
	{	double& v = table[string_value];
		if (get_token() == ASSIGN) v = expr(true);
		return v;
	}
	case MINUS:		// unary minus
		return -prim(true);
	case LP:
	{	double e = expr(true);
		if (curr_tok != RP) return error(") expected");
		get_token();		// eat ')'
		return e;
	}
	default:
		return error("primary expected");
	}
}

double term(bool get)		// multiply and divide
{
	double left = prim(get);

	for (;;)
		switch (curr_tok) {
		case MUL:
			left *= prim(true);
			break;
		case DIV:
			if (double d = prim(true)) {
				left /= d;
				break;
			}
			return error("divide by 0");
		default:
			return left;
		}
}

double expr(bool get)		// add and subtract
{
	double left = term(get);

	for (;;)				// ``forever''
		switch (curr_tok) {
		case PLUS:
			left += term(true);
			break;
		case MINUS:
			left -= term(true);
			break;
		default:
			return left;
		}
}

int main()
{

	table["pi"] = 3.1415926535897932385;	// insert predefined names
	table["e"] = 2.7182818284590452354;

	while (cin) {
		get_token();
		if (curr_tok == END) break;
		if (curr_tok == PRINT) continue;
		cout << expr(false) << '\n';
	}

	return no_of_errors;	
}
```



# 桌面计数器处理命令输入

```c++
// The desk calulator

// includes character-level input (sec6.1.3) and
// command line input (sec6.1.7),
// but no namespaces and
// no exceptions

// pp 107-119, sec 6.1, A Desk Calculator


// uses += rather than push_back() for string
// to work around standard library bug

// uses istrstream from <strstream> rather than istringstream from <sstream>
// to work around standard library bug

// No guarantees offered. Constructive comments to bs@research.att.com


/*
    program:
	END			   // END is end-of-input
	expr_list END

    expr_list:
	expression PRINT	   // PRINT is semicolon
	expression PRINT expr_list

    expression:
	expression + term
	expression - term
	term

    term:
	term / primary
	term * primary
	primary

    primary:
	NUMBER
	NAME
	NAME = expression
	- primary
	( expression )
*/

#include <string>
#include <cctype>
#include<iostream>
#include<map>

//#include<sstream>	// string streams
#include<strstream>	// C-style string streams

using namespace std;

istream* input;	// pointer to input stream

int no_of_errors;	// note: default initialized to 0

double error(const char* s)
{
    no_of_errors++;
    cerr << "error: " << s << '\n';
    return 1;
}

enum Token_value {
	NAME,		NUMBER,		END,
	PLUS='+',	MINUS='-',	MUL='*',	DIV='/',
	PRINT=';',	ASSIGN='=',	LP='(',		RP=')'
};

Token_value curr_tok = PRINT;
double number_value;
string string_value;

Token_value get_token()
{
	char ch;

	do {	// skip whitespace except '\en'
		if(!input->get(ch)) return curr_tok = END;
	} while (ch!='\n' && isspace(ch));

	switch (ch) {
	case ';':
	case '\n':
		return curr_tok=PRINT;
	case '*':
	case '/':
	case '+':
	case '-':
	case '(':
	case ')':
	case '=':
		return curr_tok=Token_value(ch);
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	case '.':
		input->putback(ch);
		*input >> number_value;
		return curr_tok=NUMBER;
	default:			// NAME, NAME=, or error
		if (isalpha(ch)) {
			string_value = ch;
			while (input->get(ch) && isalnum(ch))
				string_value += ch;	// string_value.push_back(ch);
							// to work around library bug
			input->putback(ch);
			return curr_tok=NAME;
		}
		error("bad token");
		return curr_tok=PRINT;
	}
}

map<string,double> table;

double expr(bool);	// cannot do without


double prim(bool get)		// handle primaries
{
	if (get) get_token();

	switch (curr_tok) {
	case NUMBER:		// floating-point constant
	{	double v = number_value;
		get_token();
		return v;
	}
	case NAME:
	{	double& v = table[string_value];
		if (get_token() == ASSIGN) v = expr(true);
		return v;
	}
	case MINUS:		// unary minus
		return -prim(true);
	case LP:
	{	double e = expr(true);
		if (curr_tok != RP) return error(") expected");
		get_token();		// eat ')'
		return e;
	}
	default:
		return error("primary expected");
	}
}

double term(bool get)		// multiply and divide
{
	double left = prim(get);

	for (;;)
		switch (curr_tok) {
		case MUL:
			left *= prim(true);
			break;
		case DIV:
			if (double d = prim(true)) {
				left /= d;
				break;
			}
			return error("divide by 0");
		default:
			return left;
		}
}

double expr(bool get)		// add and subtract
{
	double left = term(get);

	for (;;)				// ``forever''
		switch (curr_tok) {
		case PLUS:
			left += term(true);
			break;
		case MINUS:
			left -= term(true);
			break;
		default:
			return left;
		}
}



int main(int argc, char* argv[])
{

	switch (argc) {
	case 1:					// read from standard input
		input = &cin;
		break;
	case 2: 				// read argument string
//		input = new istringstream(argv[1]);
		input = new istrstream(argv[1]);
		break;
	default:
		error("too many arguments");
		return 1;
	}

	table["pi"] = 3.1415926535897932385;	// insert predefined names
	table["e"] = 2.7182818284590452354;

	while (*input) {
		get_token();
		if (curr_tok == END) break;
		if (curr_tok == PRINT) continue;
		cout << expr(false) << '\n';
	}

	if (input != &cin) delete input;
	return no_of_errors;
}
```



# 使用命名空间和异常的桌面计算器

```c++
// The desk calulator

// reads from standard input or command line 
// uses namespaces and no exceptions

// pp 190-1921, sec 8.3.3, Exceptions in Calculator


// uses += rather than push_back() for string
// to work around standard library bug

// uses istrstream from <strstream> rather than istringstream from <sstream>
// to work around standard library bug

// No guarantees offered. Constructive comments to bs@research.att.com


#include <map>
#include<iostream>
#include<cctype>
#include<string>

using namespace std;

namespace Error {

	struct Zero_divide { };

	struct Syntax_error {
		const char* p;
		Syntax_error(const char* q) { p = q; }
	};
}

namespace Lexer {

	enum Token_value {
		NAME,		NUMBER,		END,
		PLUS='+',	MINUS='-',	MUL='*',	DIV='/',
		PRINT=';',	ASSIGN='=',	LP='(',		RP=')'
	};

	Token_value curr_tok;
	double number_value;
	string string_value;

	Token_value get_token();
}

namespace Parser {
	double prim(bool get);		// handle primaries
	double term(bool get);		// multiply and divide
	double expr(bool get);		// add and subtract

	using namespace Lexer;
	using namespace Error;
}

namespace Symbol_table {
	map<string,double> table;
}

namespace Driver {
	int no_of_errors;
	std::istream* input;

	void skip();
}

Lexer::Token_value Lexer::get_token()
{
	char ch;

	do {	// skip whitespace except '\n'
		if(!Driver::input->get(ch)) return curr_tok = END;
	} while (ch!='\n' && isspace(ch));

	switch (ch) {
	case 0:
		return END;
	case ';':
	case '\n':
		return curr_tok=PRINT;
	case '*':
	case '/':
	case '+':
	case '-':
	case '(':
	case ')':
	case '=':
		return curr_tok=Token_value(ch);
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	case '.':
		Driver::input->putback(ch);
		*Driver::input >> number_value;
		return curr_tok=NUMBER;
	default:			// NAME, NAME =, or error
		if (isalpha(ch)) {
			string_value = ch;
			while (Driver::input->get(ch) && isalnum(ch))
				string_value += ch;	// string_value.push_back(ch);
                                                        // to work around library bug
			Driver::input->putback(ch);
			return curr_tok=NAME;
		}
		throw Error::Syntax_error("bad token");
	}
}


double Parser::prim(bool get)		// handle primaries
{
	if (get) get_token();

	switch (curr_tok) {
	case Lexer::NUMBER:	// floating point constant
		get_token();
		return number_value;
	case Lexer::NAME:
	{	double& v = Symbol_table::table[string_value];
  		if (get_token() == ASSIGN) v = expr(1);
  		return v;
	}
	case Lexer::MINUS:		// unary minus
		return -prim(1);
	case Lexer::LP:
	{	double e = expr(1);
		if (curr_tok != RP) throw Error::Syntax_error("`)' expected");
		get_token();			  // eat ')'
		return e;
	}
	case Lexer::END:
		return 1;
	default:
		throw Error::Syntax_error("primary expected");
	}
}

double Parser::term(bool get)		// multiply and divide
{
	double left = prim(get);

	for (;;)			// ``forever''
		switch (curr_tok) {
		case Lexer::MUL:
			left *= prim(true);
			break;
		case Lexer::DIV:
			if (double d = prim(true)) {
				left /= d;
				break;
			}	
			throw Error::Zero_divide();
		default:
			return left;
		}
}

double Parser::expr(bool get)		// add and subtract
{
	double left = term(get);	

	for(;;)				// ``forever''
		switch (curr_tok) {
		case Lexer::PLUS:
			left += term(true);
			break;
		case Lexer::MINUS:
			left -= term(true);
			break;
		default:
			return left;
		}
}

void Driver::skip()
{
	no_of_errors++;

	while (*input) {	// discard characters until newline or semicolon
				// note: skip doesn't know the state of the parser
				//       so if the erro rwas caused by a newline
				//       or a semicolon, we need to look for
				//       yet another terminator
		char ch;
		input->get(ch);

		switch (ch) {
		case '\n':
		case ';':
			return;
		}
	}
}

#include <strstream.h>

int main(int argc, char* argv[])
{
	using namespace Driver;

	switch (argc) {
	case 1:			 // read from standard input
		input = &cin;
		break;
	case 2:			 // read argument string
		input = new istrstream(argv[1],strlen(argv[1]));
		break;
	default:
		cerr << "too many arguments\n";
		return 1;
	}

	// insert pre-defined names:
	Symbol_table::table["pi"] = 3.1415926535897932385;
	Symbol_table::table["e"]  = 2.7182818284590452354;

 	while (*input) {
		cout<<"new expression:\n";
		try {
			Lexer::get_token();
			if (Lexer::curr_tok == Lexer::END) break;
			if (Lexer::curr_tok == Lexer::PRINT) continue;
			cout << Parser::expr(false) << '\n';
		}
		catch(Error::Zero_divide) {
			cerr << "attempt to divide by zero\n";
			skip();
		}
		catch(Error::Syntax_error e) {
			cerr << "syntax error:" << e.p << "\n";
			skip();
		}
	}

	if (input != &std::cin) delete input;
	return no_of_errors;
}
```



#  使用命名空间的桌面计算器

```c++
// The desk calulator

// reads from std::cin 
// uses namespaces, but no exceptions

// pp 163-171, sec 8.2, Namespaces


// uses += rather than push_back() for string
// to work around standard library bug

// No guarantees offered. Constructive comments to bs@research.att.com

/*
	program:
		END			   // END is end-of-input
		expr_list END

	expr_list:
		expression PRINT	   // PRINT is ';'
		expression PRINT expr_list

	expression:
		expression + term
		expression - term
		term

	term:
		term / primary
		term * primary
		primary

	primary:
		NUMBER
		NAME
		NAME = expression
		- primary
		( expression )
*/

#include<iostream>
#include<map>
#include<string>
#include<cctype>

// note: no "using namespace std;"

namespace Error {

	int no_of_errors;

	double error(const char* s)
	{
			std::cerr << "error: " << s << '\n';
			no_of_errors++;
			return 1;
	}
}

namespace Lexer {

	enum Token_value {
		NAME,	NUMBER,		END,
		PLUS='+',	MINUS='-',	MUL='*',	DIV='/',
		PRINT=';',	ASSIGN='=',	LP='(',		RP=')'
	};

	Token_value curr_tok;
	double number_value;
	std::string string_value;

	Token_value get_token();
}


Lexer::Token_value Lexer::get_token()
{
	char ch = 0;
	std::cin>>ch;

	switch (ch) {
	case 0:
		return END;
	case ';':
		return curr_tok=PRINT;
	case '*':
	case '/':
	case '+':
	case '-':
	case '(':
	case ')':
	case '=':
		return curr_tok=Token_value(ch);
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	case '.':
		std::cin.putback(ch);
		std::cin >> number_value;
		return curr_tok=NUMBER;
	default:			// NAME, NAME =, or error
		if (isalpha(ch)) {	// warning isalpha is a macro
					// in some implementations
			std::cin.putback(ch);
			std::cin>>string_value;
			return curr_tok=NAME;
		}
		Error::error("bad token");
		return curr_tok=PRINT;
	}
}

std::map<std::string,double> table;	// global; see sec9.???

namespace Parser {
	double prim(bool get);		// handle primaries
	double term(bool get);		// multiply and divide
	double expr(bool get);		// add and subtract

	using Lexer::get_token;		// commonly used names
	using Lexer::curr_tok;
	using Error::error;
}

double Parser::prim(bool get)		// handle primaries
{
	if (get) get_token();

	switch (curr_tok) {
	case Lexer::NUMBER:	// floating point constant
		get_token();
		return Lexer::number_value;
	case Lexer::NAME:
	{	double& v = table[Lexer::string_value];
  		if (get_token() == Lexer::ASSIGN) v = expr(1);
  		return v;
	}
	case Lexer::MINUS:		// unary minus
		return -prim(1);
	case Lexer::LP:
	{	double e = expr(1);
		if (curr_tok != Lexer::RP) return error(") expected");
		get_token();			  // eat ')'
		return e;
	}
	case Lexer::END:
		return 1;
	default:
		return error("primary expected");
	}
}

double Parser::term(bool get)		// multiply and divide
{
	double left = prim(get);

	for (;;)
	switch (curr_tok) {
	case Lexer::MUL:
		left *= prim(true);
		break;
	case Lexer::DIV:
		if (double d = prim(true)) {
		left /= d;
			break;
		}
		return error("divide by 0");
	default:
		return left;
	}
}

double Parser::expr(bool get)		// add and subtract
{
	double left = term(get);	

	for(;;)			// ``forever''
	switch (curr_tok) {
	case Lexer::PLUS:
		left += term(true);
		break;
	case Lexer::MINUS:
		left -= term(true);
		break;
	default:
		return left;
		}
}

int main(int argc, char* argv[])
{
	// insert pre-defined names:
	table["pi"] = 3.1415926535897932385;
	table["e"]  = 2.7182818284590452354;

	while (std::cin) {
		Lexer::get_token();
		if (Lexer::curr_tok == Lexer::END) break;
		if (Lexer::curr_tok == Lexer::PRINT) continue;
		std::cout << Parser::expr(false) << '\n';
	}

	return Error::no_of_errors;	
}
```



#  使用例外的桌面计算器

```c++
// The desk calulator

// reads from standard input or command line 
// uses namespaces and no exceptions

// pp 190-1921, sec 8.3.3, Exceptions in Calculator


// uses += rather than push_back() for string
// to work around standard library bug

// uses istrstream from <strstream> rather than istringstream from <sstream>
// to work around standard library bug

// No guarantees offered. Constructive comments to bs@research.att.com


#include <map>
#include<iostream>
#include<cctype>
#include<string>

using namespace std;

namespace Error {

	struct Zero_divide { };

	struct Syntax_error {
		const char* p;
		Syntax_error(const char* q) { p = q; }
	};
}

namespace Lexer {

	enum Token_value {
		NAME,		NUMBER,		END,
		PLUS='+',	MINUS='-',	MUL='*',	DIV='/',
		PRINT=';',	ASSIGN='=',	LP='(',		RP=')'
	};

	Token_value curr_tok;
	double number_value;
	string string_value;

	Token_value get_token();
}

namespace Parser {
	double prim(bool get);		// handle primaries
	double term(bool get);		// multiply and divide
	double expr(bool get);		// add and subtract

	using namespace Lexer;
	using namespace Error;
}

namespace Symbol_table {
	map<string,double> table;
}

namespace Driver {
	int no_of_errors;
	std::istream* input;

	void skip();
}

Lexer::Token_value Lexer::get_token()
{
	char ch;

	do {	// skip whitespace except '\n'
		if(!Driver::input->get(ch)) return curr_tok = END;
	} while (ch!='\n' && isspace(ch));

	switch (ch) {
	case 0:
		return END;
	case ';':
	case '\n':
		return curr_tok=PRINT;
	case '*':
	case '/':
	case '+':
	case '-':
	case '(':
	case ')':
	case '=':
		return curr_tok=Token_value(ch);
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	case '.':
		Driver::input->putback(ch);
		*Driver::input >> number_value;
		return curr_tok=NUMBER;
	default:			// NAME, NAME =, or error
		if (isalpha(ch)) {
			string_value = ch;
			while (Driver::input->get(ch) && isalnum(ch))
				string_value += ch;	// string_value.push_back(ch);
                                                        // to work around library bug
			Driver::input->putback(ch);
			return curr_tok=NAME;
		}
		throw Error::Syntax_error("bad token");
	}
}


double Parser::prim(bool get)		// handle primaries
{
	if (get) get_token();

	switch (curr_tok) {
	case Lexer::NUMBER:	// floating point constant
		get_token();
		return number_value;
	case Lexer::NAME:
	{	double& v = Symbol_table::table[string_value];
  		if (get_token() == ASSIGN) v = expr(1);
  		return v;
	}
	case Lexer::MINUS:		// unary minus
		return -prim(1);
	case Lexer::LP:
	{	double e = expr(1);
		if (curr_tok != RP) throw Error::Syntax_error("`)' expected");
		get_token();			  // eat ')'
		return e;
	}
	case Lexer::END:
		return 1;
	default:
		throw Error::Syntax_error("primary expected");
	}
}

double Parser::term(bool get)		// multiply and divide
{
	double left = prim(get);

	for (;;)			// ``forever''
		switch (curr_tok) {
		case Lexer::MUL:
			left *= prim(true);
			break;
		case Lexer::DIV:
			if (double d = prim(true)) {
				left /= d;
				break;
			}	
			throw Error::Zero_divide();
		default:
			return left;
		}
}

double Parser::expr(bool get)		// add and subtract
{
	double left = term(get);	

	for(;;)				// ``forever''
		switch (curr_tok) {
		case Lexer::PLUS:
			left += term(true);
			break;
		case Lexer::MINUS:
			left -= term(true);
			break;
		default:
			return left;
		}
}

void Driver::skip()
{
	no_of_errors++;

	while (*input) {	// discard characters until newline or semicolon
				// note: skip doesn't know the state of the parser
				//       so if the erro rwas caused by a newline
				//       or a semicolon, we need to look for
				//       yet another terminator
		char ch;
		input->get(ch);

		switch (ch) {
		case '\n':
		case ';':
			return;
		}
	}
}

#include <strstream.h>

int main(int argc, char* argv[])
{
	using namespace Driver;

	switch (argc) {
	case 1:			 // read from standard input
		input = &cin;
		break;
	case 2:			 // read argument string
		input = new istrstream(argv[1],strlen(argv[1]));
		break;
	default:
		cerr << "too many arguments\n";
		return 1;
	}

	// insert pre-defined names:
	Symbol_table::table["pi"] = 3.1415926535897932385;
	Symbol_table::table["e"]  = 2.7182818284590452354;

 	while (*input) {
		cout<<"new expression:\n";
		try {
			Lexer::get_token();
			if (Lexer::curr_tok == Lexer::END) break;
			if (Lexer::curr_tok == Lexer::PRINT) continue;
			cout << Parser::expr(false) << '\n';
		}
		catch(Error::Zero_divide) {
			cerr << "attempt to divide by zero\n";
			skip();
		}
		catch(Error::Syntax_error e) {
			cerr << "syntax error:" << e.p << "\n";
			skip();
		}
	}

	if (input != &std::cin) delete input;
	return no_of_errors;
}
```



# 简单联想数组示例

```c++
// Simple associative array example
// to illustrate overloading of subscripting

// pp 286-287, sec 11.8 Subscripting

// No guarantees offered. Constructive comments to bs@research.att.com

#include<vector>
#include<string>
#include<iostream>
using namespace std;

class Assoc {
	struct Pair {
		string name;
		double val;
		Pair(string n ="", double v =0) :name(n), val(v) { }
	};
	vector<Pair> vec;

	Assoc(const Assoc&);			// private to prevent copying
	Assoc& operator=(const Assoc&);	// private to prevent copying
public:
	Assoc() {}
	const double& operator[](const string&);
	double& operator[](string&);
	void print_all() const;
};

const double& Assoc::operator[](const string& s)
	// search for s; return its value if found; otherwise, make a new Pair and return the default value 0
{
	for (vector<Pair>::const_iterator p = vec.begin(); p!=vec.end(); ++p)
		if (s == p->name) return p->val;

	vec.push_back(Pair(s,0));	// initial value: 0

	return vec.back().val;		// return last element (_org.element_)
}

double& Assoc::operator[](string& s)
	// search for s; return its value if found; otherwise, make a new Pair and return the default value 0
{
	for (vector<Pair>::iterator p = vec.begin(); p!=vec.end(); ++p)
		if (s == p->name) return p->val;

	vec.push_back(Pair(s,0));	// initial value: 0

	return vec.back().val;		// return last element (_org.element_)
}

void Assoc::print_all() const
{
	for (vector<Pair>::const_iterator p = vec.begin(); p!=vec.end(); ++p)
		cout << p->name << ": " << p->val << '\n';
}

int main()	// count the occurrences of each word on input
{
	string buf;
	Assoc vec;
	while (cin>>buf) vec[buf]++;
	vec.print_all();
}
```



#  简单字符串示例

```c++
// Define simple string class, and exercise it a bit

// pp pp 292-298, sec 11.12, A String Class

// No guarantees offered. Constructive comments to bs@research.att.com

// writing your own string class is a good exercise.
// Once you have learned from that, throw it away and use std::string

#include<iostream>
using namespace std;

class String {
	struct Srep;			// representation
	Srep *rep;

	class Cref;			// reference to char
public:

	class Range { };		// for exceptions
	
	String();			// x = ""
	String(const char*);		// x = "abc" 
	String(const String&);	// x = other_string
	String& operator=(const char *);
	String& operator=(const String&);
	~String();

	void check(int i) const;

	char read(int i) const;
	void write(int i, char c); 

	Cref operator[](int i);
	char operator[](int i) const;

	int size() const;

	String& operator+=(const String&);
	String& operator+=(const char*);

	friend ostream& operator<<(ostream&, const String&);
	friend istream& operator>>(istream&, String&);

	friend bool operator==(const String& x, const char* s);

	friend bool operator==(const String& x, const String& y);

	friend bool operator!=(const String& x, const char* s);

	friend bool operator!=(const String& x, const String& y);
};

String operator+(const String&, const String&);
String operator+(const String&, const char*);

struct String::Srep {
	char* s;	// pointer to elements
	int sz;		// number of characters
	int n;		// reference count

	Srep(int nsz, const char* p)
	{
		n = 1;
		sz = nsz;
		s = new char[sz+1];	// add space for terminator
		strcpy(s,p);
	}

	~Srep() { delete[] s; }

	Srep* get_own_copy()	// clone if necessary
	{
		if (n==1) return this;
		n--;	
		return new Srep(sz,s);
	}

	void assign(int nsz, const char* p)
	{
		if (sz != nsz) {
			delete[] s;
			sz = nsz;
			s = new char[sz+1];
		}
		strcpy(s,p);
	}

private:				// prevent copying:
	Srep(const Srep&);
	Srep& operator=(const Srep&);
};

class String::Cref {		// reference to s[i]
friend class String;
	String& s;
	int i;

	Cref(String& ss, int ii) : s(ss), i(ii) { }
	Cref(const Cref& r) : s(r.s), i(r.i) { }
	Cref();	// not defined, never used
public:
	operator char() const { s.check(i); return s.read(i); }	// yield value
	void operator=(char c) { s.write(i,c); }			// change value
};

String::String()		// the empty string is the default value 
{
	rep = new Srep(0,"");
}

String::String(const String& x)	// copy constructor
{
	x.rep->n++;
	rep = x.rep;	// share representation
}

String::~String()
{
	if (--rep->n == 0) delete rep;
}

String& String::operator=(const String& x)	// copy assignment
{
	x.rep->n++;				// protects against ``st = st''
	if (--rep->n == 0) delete rep;
	rep = x.rep;				// share representation
	return *this;
}

String::String(const char* s)
{
	rep = new Srep(strlen(s),s);
}

String& String::operator=(const char* s)
{
	if (rep->n == 1)			// recycle Srep
		rep->assign(strlen(s),s);
	else {					// use new Srep
		rep->n--;
		rep = new Srep(strlen(s),s);
	}
	return *this;
}

void String::check(int i) const
{
	if (i<0 || rep->sz<=i) throw Range();
}

char String::read(int i) const
{
	return rep->s[i];
}

void String::write(int i, char c)
{
	rep=rep->get_own_copy();
	rep->s[i]=c;
} 

String::Cref String::operator[](int i)
{
	check(i);
	return Cref(*this,i);
}

char String::operator[](int i) const
{
	check(i);
	return rep->s[i];
}

int String::size() const
{
	return rep->sz;
} 

ostream& operator<<(ostream& os, const String& s)
{
	return os << s.rep->s;
}

#include<string>

istream& operator>>(istream& is, String& s)
// this implementation is "almost cheating" :-)
{
	string ss;
	is >> ss;
	s = ss.c_str();
	return is;
}

bool operator==(const String& x, const char* s)
{
	return strcmp(x.rep->s, s) == 0;
}

bool operator==(const String& x, const String& y)
{
	return strcmp(x.rep->s, y.rep->s) == 0;
}

bool operator!=(const String& x, const char* s)
{
	return strcmp(x.rep->s, s) != 0;
}

bool operator!=(const String& x, const String& y)
{
	return strcmp(x.rep->s, y.rep->s) != 0;
}

void ff(String s, const String& r)
{
	char c1 = s[1];	// c1 = s.operator[](1).operator char()
	s[1] = 'c';		// s.operator[](1).operator=('c')

	char c2 = r[1];	// c2 = r.operator[](1)
//	r[1] = 'd';		// error: assignment to non-lvalue char, r.operator[](1) = 'd'
}

int hash(const String& s)
{
	int h = s.read(0);
	const int max = s.size();
	for (int i = 1; i<max; i++) h ^= s.read(i)>>1;	// unchecked access to s
	return h;
}

String f(String a, String b)
{
	a[2] = 'x';
	char c = b[3];
	cout << "in f: " << a << ' ' << b << ' ' << c << '\n';
	return b;
}


int main()
// very simple minded first test
{
	String x, y;
	cout << "Please enter two strings\n";
	cin >> x >> y;
	cout << "input: " << x << ' ' << y << '\n';

	String z = x;
	y = f(x,y);
	if (x != z) cout << "x corrupted!\n";
	x[0] = '!';
	if (x == z) cout << "write failed!\n";
	cout << "exit: " << x << ' ' << y << ' ' << z << '\n';
}
```



# mismatch（） 的简单用法

```c++
// Simple test program to demonstrate mismatch() and predicate

// pg 516 and pg 523

// No guarantees offered. Constructive comments to bs@research.att.com

#include<vector>
#include<list>
#include<algorithm>
#include<functional>
#include<iostream>
using namespace std;

void m1(vector<int>& vi, list<int>& li)
{
	typedef list<int>::iterator LI;
	typedef vector<int>::iterator VI;
	pair<VI,LI> p1 = mismatch(vi.begin(),vi.end(),li.begin(),less<int>());

	cout << "m1: " << *p1.first << " " << *p1.second << endl;
}

void m2(vector<int>& vi, list<int>& li)
{
	typedef list<int>::iterator LI;
	typedef vector<int>::iterator VI;
	pair<VI,LI> p1 = mismatch(vi.begin(),vi.end(),li.begin(),greater_equal<int>());

	cout << "m2: " << *p1.first << " " << *p1.second << endl;
}

void m3(vector<int>& vi, list<int>& li)
{
	typedef list<int>::iterator LI;
	typedef vector<int>::iterator VI;
	pair<LI,VI> p1 = mismatch(li.begin(),li.end(),vi.begin(),less_equal<int>());

	cout << "m3: " << *p1.second << " " << *p1.first << endl;
}

void m4(vector<int>& vi, list<int>& li)	// revised example from _algo.pred_
{

	typedef list<int>::iterator LI;
	typedef vector<int>::iterator VI;
	pair<VI,LI> p1 =  mismatch(vi.begin(),vi.end(),li.begin(),not2(less<int>()));
	cout << "m4: " << *p1.first << " " << *p1.second << endl;
}

int main()
{
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	v.push_back(5);

	list<int> lst;
	lst.push_back(1);
	lst.push_back(1);
	lst.push_back(3);
	lst.push_back(3);
	lst.push_back(5);


	m1(v,lst);
	m2(v,lst);
	m3(v,lst);
	m4(v,lst);
}
```





#  “俱乐部”示例



```c++
// Simple test program to exercise standard algorithms and function objects

// "Club" examples scattered throughout Chapter 18 

// No guarantees offered. Constructive comments to bs@research.att.com

#include<list>
#include<iostream>
#include<algorithm>
#include<functional>
#include<string>
#include<vector>
#include<iterator>
using namespace std;

namespace {
	string noname = "";
	string noaddr = "";
}

struct Person {
	string name;
	string addr;

	Person(const string& n, const string& a = noaddr) :name(n), addr(a) { }
};

struct Club {
	string name;
	list<Person*> members;
	list<Person*> officers;
	// ...
	string town;

	Club(const string& n) :name(n) {}
};

class Club_eq : public unary_function<Club,bool> {
	string s;
public:
	explicit Club_eq(const string& ss) : s(ss) { }
	bool operator()(const Club& c) const { return c.name==s; }
};


class Extract_officers {
	list<Person*>& lst;
public:
	explicit Extract_officers(list<Person*>& x) : lst(x) { }

	void operator() (const Club& c) const
	{ copy(c.officers.begin(),c.officers.end(),back_inserter(lst)); }
};

void extract(const list<Club>& lc, list<Person*>& off)
// place the officers from `lc' on `off'
{
	for_each(lc.begin(),lc.end(),Extract_officers(off));
}

class Print_name {
	ostream& os;
public:
	Print_name(ostream& s) :os(s) { }
	void operator()(Person* p) { os << p->name << endl; }
};


void extract_and_print(const list<Club>& lc)
{
	cout << "\nPrint officers:\n";

	list<Person*> off;
	extract(lc,off);
	for_each(off.begin(),off.end(),Print_name(cout));
}

string nameof(const Club& c) { return c.name; }	// extract name string


void f(list<Club>& lc)
{
	cout << "\nPrint names:\n";

	transform(lc.begin(),lc.end(),ostream_iterator<string>(cout,"\n"),nameof);
}

bool operator==(const Person& x, const Person& y)
// equality for object
{
	// compare x and y for equality
	return x.name == y.name;	// simple minded
}

bool operator<(const Person& x, const Person& y)
// less than for object
{
	// compare x and y for order
	return x.name < y.name;	// simple minded
}

bool Person_eq(const Person* x, const Person* y)
// equality through pointer
{
	return *x == *y;
}

bool Person_lt(const Person* x, const Person* y)
// less than through pointer
{
	return *x < *y;
}

void extract_and_print2(const list<Club>& lc)
{
	cout << "\nPrint officers in alphabetic order:\n";

	list<Person*> off;	// not list
	extract(lc,off);
	off.sort(Person_lt);
	//sort(off.begin(),off.end(),Person_lt);
	list<Person*>::iterator p = unique(off.begin(),off.end(),Person_eq);
	for_each(off.begin(),p,Print_name(cout));
}


class located_in : public unary_function<Club,bool> {
	string town;
public:
	located_in(const string& ss) :town(ss) { }
	bool operator()(const Club& c) const { return c.town == town; }
};

void ff(list<Club>& lc)
{
	cout << "\nPrint clubs located in Kbh:\n";

	remove_copy_if(lc.begin(),lc.end(),
		ostream_iterator<Club>(cout),not1(located_in("Kbh")));
}

ostream& operator<<(ostream& s, const Person& p)
{
	return s << "(" << p.name << "," << p.addr << ")";
}

ostream& operator<<(ostream& s, const list<Person*>& c)
{
	for (list<Person*>::const_iterator p = c.begin(); p!=c.end(); ++p)
		s << **p << ", ";
	return s;
}

template<class T> ostream& operator<<(ostream& s, const list<T>& c)
{
//	for (list<T>::const_iterator p = c.begin(); p!=c.end(); ++p)
//		s << *p << ", ";
	copy(c.begin(),c.end(),ostream_iterator<T>(s,", "));
	return s;
}
ostream& operator<<(ostream& s, const Club& c)
{
	s << c.name << "{ ";
	s << "[off: " << c.officers << " ]";
	s << "[mem: " << c.members << " ]";
	s << c.town ;
	return s << " }\n";
}

void make_person_list(list<Person*>& lst,
	const string& s1 = noname,
	const string& s2 = noname,
	const string& s3  = noname)
{
	if (&s1!=&noname) lst.push_back(new Person(s1));
	if (&s2!=&noname) lst.push_back(new Person(s2));
	if (&s3!=&noname) lst.push_back(new Person(s3));
}

int main()
{
	list<Club> snoot;
	snoot.push_back(Club("Horsy"));
	make_person_list(snoot.back().members,"Major","Tops");
	make_person_list(snoot.back().officers,"Major","Tops");
	snoot.back().town = "Kbh";
	snoot.push_back(Club("Hunt"));
	make_person_list(snoot.back().members,"Elk","Fox","Hare");
	make_person_list(snoot.back().officers,"Riffle");
	snoot.back().town = "Kbh";
	snoot.push_back(Club("Over_eat"));
	make_person_list(snoot.back().members,"Dinner","Breakfast","Lunch");
	make_person_list(snoot.back().officers,"Dinner","Lunch");
	snoot.back().town = "Paris";
	snoot.push_back(Club("Drink_and_be_merry"));
	make_person_list(snoot.back().members,"Beer","Red","Wiskey");
	make_person_list(snoot.back().officers,"Red");
	snoot.back().town = "Paris";

	cout << snoot;

	f(snoot);

	ff(snoot);

	extract_and_print(snoot);

	extract_and_print2(snoot);
}
```



#  复杂用户自定义输出算子

```
// Program to test user-define output operator.

// pg 612, sec 21.2.3, Output of User-Defined Type

// No guarantees offered. Constructive comments to bs@research.att.com


#include<iostream>

using namespace std;


// Note: my "complex"; not the one from the standard library

class complex {
public:
	complex(double r =0, double i =0) : re(r), im(i) { }

	double real() const { return re; }
	double imag() const { return im; }
	// ...
private:
	double re, im;
};

ostream& operator<<(ostream&s, const complex& z)
{
	return s << '(' << z.real() << ',' << z.imag() << ')';
}

int main()
{
	complex x(1,2);
	cout << "x = " << x << '\n';

	return 0;	// redundant in ISO C++
}
```



# 虚拟输出函数

```c++
// Program to test integration of a virtual output function with stream I/O.

// pg 613, sec 21.2.3.1, Virtual Output Functions

// Also illustrates how we can achieve the effect of a function that is
// polymorphic on its second argument.

// No guarantees offered. Constructive comments to bs@research.att.com


#include<iostream>

using namespace std;

class My_base {
public:
	// ...

	virtual ostream& put(ostream& s) const = 0;	// write *this to s
};

ostream& operator<<(ostream& s, const My_base& r)
{
	return r.put(s);	// use the right put()
}


class Sometype : public My_base {
public:
	// ... 

	ostream& put(ostream& s) const;	// the real output function: override My_base::put()
};

void f(const My_base& r, Sometype& s)	// use << which calls the right put()
{
	cout << r << s;
}

class Some_other_type : public My_base {
public:
	// ... 

	ostream& put(ostream& s) const;
};

int main()
{
	Sometype s;
	Some_other_type s2;
	My_base& b = s;
	f(b,s);
	f(s2,s);

	return 0;	// redundant in ISO C++
}

ostream& Sometype::put(ostream& s) const
{
	return s << "Sometype\n";
}

ostream& Some_other_type::put(ostream& s) const
{
	return s << "Some_other_type\n";
}
```



# 切片和矩阵实例

```c++
// Program to test slices and a simple N*M matrix class

// pp 670-674 and 683-684

// No guarantees offered. Constructive comments to bs@research.att.com


#include<iostream>
#include<valarray>
#include<algorithm>
#include<numeric>	// for inner_product
using namespace std;

// forward declarations to allow friend declarations:
template<class T> class Slice_iter;
template<class T> bool operator==(const Slice_iter<T>&, const Slice_iter<T>&);
template<class T> bool operator!=(const Slice_iter<T>&, const Slice_iter<T>&);
template<class T> bool operator< (const Slice_iter<T>&, const Slice_iter<T>&);

template<class T> class Slice_iter {
	valarray<T>* v;
	slice s;
	size_t curr;	// index of current element

	T& ref(size_t i) const { return (*v)[s.start()+i*s.stride()]; }
public:
	Slice_iter(valarray<T>* vv, slice ss) :v(vv), s(ss), curr(0) { }

	Slice_iter end() const
	{
		Slice_iter t = *this;
		t.curr = s.size();	// index of last-plus-one element
		return t;
	}

	Slice_iter& operator++() { curr++; return *this; }
	Slice_iter operator++(int) { Slice_iter t = *this; curr++; return t; }

	T& operator[](size_t i) { return ref(i); }		// C style subscript
	T& operator()(size_t i) { return ref(i); }		// Fortran-style subscript
	T& operator*() { return ref(curr); }			// current element

	friend bool operator==<>(const Slice_iter& p, const Slice_iter& q);
	friend bool operator!=<>(const Slice_iter& p, const Slice_iter& q);
	friend bool operator< <>(const Slice_iter& p, const Slice_iter& q);

};


template<class T>
bool operator==(const Slice_iter<T>& p, const Slice_iter<T>& q)
{
	return p.curr==q.curr
		&& p.s.stride()==q.s.stride()
		&& p.s.start()==q.s.start();
}

template<class T>
bool operator!=(const Slice_iter<T>& p, const Slice_iter<T>& q)
{
	return !(p==q);
}

template<class T>
bool operator<(const Slice_iter<T>& p, const Slice_iter<T>& q)
{
	return p.curr<q.curr
		&& p.s.stride()==q.s.stride()
		&& p.s.start()==q.s.start();
}


//-------------------------------------------------------------



// forward declarations to allow friend declarations:
template<class T> class Cslice_iter;
template<class T> bool operator==(const Cslice_iter<T>&, const Cslice_iter<T>&);
template<class T> bool operator!=(const Cslice_iter<T>&, const Cslice_iter<T>&);
template<class T> bool operator< (const Cslice_iter<T>&, const Cslice_iter<T>&);


template<class T> class Cslice_iter 
{
	valarray<T>* v;
	slice s;
	size_t curr; // index of current element
	const T& ref(size_t i) const { return (*v)[s.start()+i*s.stride()]; }
public:
	Cslice_iter(valarray<T>* vv, slice ss): v(vv), s(ss), curr(0){}
	Cslice_iter end() const
	{
		Cslice_iter t = *this;
		t.curr = s.size(); // index of one plus last element
		return t;
	}
	Cslice_iter& operator++() { curr++; return *this; }
	Cslice_iter operator++(int) { Cslice_iter t = *this; curr++; return t; }
	
	const T& operator[](size_t i) const { return ref(i); }
	const T& operator()(size_t i) const { return ref(i); }
	const T& operator*() const { return ref(curr); }

	friend bool operator==<>(const Cslice_iter& p, const Cslice_iter& q);
	friend bool operator!=<>(const Cslice_iter& p, const Cslice_iter& q);
	friend bool operator< <>(const Cslice_iter& p, const Cslice_iter& q);

};

template<class T>
bool operator==(const Cslice_iter<T>& p, const Cslice_iter<T>& q)
{
	return p.curr==q.curr
		&& p.s.stride()==q.s.stride()
		&& p.s.start()==q.s.start();
}

template<class T>
bool operator!=(const Cslice_iter<T>& p, const Cslice_iter<T>& q)
{
	return !(p==q);
}

template<class T>
bool operator<(const Cslice_iter<T>& p, const Cslice_iter<T>& q)
{
	return p.curr<q.curr
		&& p.s.stride()==q.s.stride()
		&& p.s.start()==q.s.start();
}


//-------------------------------------------------------------


class Matrix {
	valarray<double>* v;	// stores elements by column as described in 22.4.5
	size_t d1, d2;	// d1 == number of columns, d2 == number of rows
public:
	Matrix(size_t x, size_t y);		// note: no default constructor
	Matrix(const Matrix&);
	Matrix& operator=(const Matrix&);
	~Matrix();
	
	size_t size() const { return d1*d2; }
	size_t dim1() const { return d1; }
	size_t dim2() const { return d2; }

	Slice_iter<double> row(size_t i);
	Cslice_iter<double> row(size_t i) const;

	Slice_iter<double> column(size_t i);
	Cslice_iter<double> column(size_t i) const;

	double& operator()(size_t x, size_t y);					// Fortran-style subscripts
	double operator()(size_t x, size_t y) const;

	Slice_iter<double> operator()(size_t i) { return column(i); }
	Cslice_iter<double> operator()(size_t i) const { return column(i); }

	Slice_iter<double> operator[](size_t i) { return column(i); }	// C-style subscript
	Cslice_iter<double> operator[](size_t i) const { return column(i); }

	Matrix& operator*=(double);

	valarray<double>& array() { return *v; }
};


inline Slice_iter<double> Matrix::row(size_t i)
{
	return Slice_iter<double>(v,slice(i,d1,d2));
}

inline Cslice_iter<double> Matrix::row(size_t i) const
{
	return Cslice_iter<double>(v,slice(i,d1,d2));
}

inline Slice_iter<double> Matrix::column(size_t i)
{
	return Slice_iter<double>(v,slice(i*d2,d2,1));
}

inline Cslice_iter<double> Matrix::column(size_t i) const
{
	return Cslice_iter<double>(v,slice(i*d2,d2,1));
}

Matrix::Matrix(size_t x, size_t y)
{
	// check that x and y are sensible
	d1 = x;
	d2 = y;
	v = new valarray<double>(x*y);
}

Matrix::~Matrix()
{
	delete v;
}

double& Matrix::operator()(size_t x, size_t y)
{
	return column(x)[y];
}



//-------------------------------------------------------------




double mul(const Cslice_iter<double>& v1, const valarray<double>& v2)
{
	double res = 0;
	for (size_t i = 0; i<v2.size(); i++) res+= v1[i]*v2[i];
	return res;
}


valarray<double> operator*(const Matrix& m, const valarray<double>& v)
{
	if (m.dim1()!=v.size()) cerr << "wrong number of elements in m*v\n";

	valarray<double> res(m.dim2());
	for (size_t i = 0; i<m.dim2(); i++) res[i] = mul(m.row(i),v);
	return res;
}


// alternative definition of m*v

//valarray<double> operator*(const Matrix& m, valarray<double>& v)
valarray<double> mul_mv(const Matrix& m, valarray<double>& v)
{
	if (m.dim1()!=v.size()) cerr << "wrong number of elements in m*v\n";

	valarray<double> res(m.dim2());

	for (size_t i = 0; i<m.dim2(); i++) {
		const Cslice_iter<double>& ri = m.row(i);
		res[i] = inner_product(ri,ri.end(),&v[0],double(0));
	}
	return res;
}



valarray<double> operator*(valarray<double>& v, const Matrix& m)
{
	if (v.size()!=m.dim2()) cerr << "wrong number of elements in v*m\n";

	valarray<double> res(m.dim1());

	for (size_t i = 0; i<m.dim1(); i++) {
		const Cslice_iter<double>& ci = m.column(i);
		res[i] = inner_product(ci,ci.end(),&v[0],double(0));
	}
	return res;
}

Matrix& Matrix::operator*=(double d)
{
	(*v) *= d;
	return *this;
}

ostream& operator<<(ostream& os, Matrix& m)
{
	for(int y=0; y<m.dim2(); y++)
	{
		for(int x=0; x<m.dim1(); x++)
			os<<m[x][y]<<"\t";
		os << "\n";
	}
	return os;
}


//-------------------------------------------------------------


void f (int x_max, int y_max)	// test layout and basic access
{
	cout << "\nf(" << x_max <<"," << y_max << "):\n";
	
	Matrix a(x_max, y_max);

	for(int x=0; x<x_max; x++)		// initialize
		for(int y=0; y<y_max; y++)
			a[x][y]=x+y*10;

	cout<<"C-style access used to initialize:\n" << a;

	for(int x=0; x<x_max; x++)
		for(int y=0; y<y_max; y++)
			a(x,y)=x+y*10;

	cout <<"Fortran-style access used to initialize:\n" << a;
	
	cout << "addresses: \n";

	for(int x=0; x<x_max; x++)
		for(int y=0; y<y_max; y++)
			cout<<"("<< x<<","<<y<<") at "<<&a[x][y]-&a[0][0]<<"\n";
	cout <<"columns :\n";

	for(int x=0; x<x_max; x++) {
		cout << "column " << x << ":\n";
		for (Slice_iter<double> c = a.column(x); c!=c.end(); ++c)
			cout << "\t" << *c <<"\n";
	}

	cout <<"rows :\n";

	for(int y=0; y<y_max; y++) {
		cout << "row " << y << ":";
		for(Slice_iter<double> r = a.row(y); r!=r.end(); ++r)
			cout << "\t" << *r ;
		cout <<"\n";
	}


}

ostream& operator<<(ostream& os, const valarray<double>& v)
{
	for (int i = 0; i<v.size(); ++i) os << '\t' << v[i];
	return os;
}

void g(int x_max,int y_max)	// check multiplication
{
	cout << "\ng(" << x_max <<"," << y_max << "):\n";

	Matrix a(x_max,y_max);

	for(int x=0; x<x_max; x++)		// initialize
		for(int y=0; y<y_max; y++)
			a[x][y]=x+y*10;

	valarray<double> r(2,x_max);
	cout << "a*v: " << a*r << endl;
	cout << "m*v: " << mul_mv(a,r) << endl;

	valarray<double> c(2,y_max);
	cout << "v*a: " << c*a << endl;
	
}

int main()
{
	f(3,4);
	f(4,3);

	g(3,4);
	g(4,3);
}
```



# 多维矢量

```c++
// Simple multidimensional vector test.

// pp 836-837

// No guarantees offered. Constructive comments to bs@research.att.com


#include<vector>
#include<iostream>
using namespace std;

vector< vector<int> > m(3, vector<int>(5));

void init_m()
{
	for (int i = 0; i<m.size(); i++)
		for (int j = 0; j<m[i].size(); j++) m[i][j] = 10*i+j;
}

void print_m()
{
	for (int i = 0; i<m.size(); i++) {
		for (int j = 0; j<m[i].size(); j++) cout << m[i][j] << '\t';
		cout << '\n';
	}
}

int main()
{
	init_m();
	print_m();
}
```



# 多维数组

```c++
// Simple multidimensional array test.
// Note that using  vectors is simpler

// pp 837-840

// No guarantees offered. Constructive comments to bs@research.att.com

#include<iostream>
using namespace std;

void print_m34(int m[3][4])
{
	for (int i = 0; i<3; i++) {
		for (int j = 0; j<4; j++) cout << m[i][j] << '\t';
		cout << '\n';
	}
}

void print_mi4(int m[][4], int dim1)
{
	for (int i = 0; i<dim1; i++) {
		for (int j = 0; j<4; j++) cout << m[i][j] << '\t';
		cout << '\n';
	}
}

void print_mij(int* m, int dim1, int dim2)
{
	for (int i = 0; i<dim1; i++) {
		for (int j = 0; j<dim2; j++)
			cout << m[i*dim2+j] << '\t'; // obscure
		cout << '\n';
	}
}


int main()
{
	int v[3][4] = { {0,1,2,3}, {10,11,12,13}, {20,21,22,23} };

	print_m34(v);
	cout << '\n';
	print_mi4(v,3);
	cout << '\n';
	print_mij((int*)v,3,4);
	cout << '\n';
	print_mij(v[0],3,4);	// also ok
	cout << '\n';
//	print_mij(v,3,4);	// error
}
```































































