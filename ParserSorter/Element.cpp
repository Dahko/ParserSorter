#include "Element.h"

void TestClosingBracket() {
	std::string s("(123+dfsd +(1+(2+1) )+1) + 200 + (1+2)");
	TStrIter itClosing = GetClosingBracket(s.begin()+1, s.end());
	printf((itClosing - s.begin()) ? "ok" : "oops");
	std::string s2(s.begin(), itClosing+1);
	printf("\n"+s2.c_str());
	printf("\n");
}

void main() {
	TestClosingBracket();

	system("pause");
}