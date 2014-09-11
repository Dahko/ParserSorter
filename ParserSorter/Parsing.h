#pragma once
#include "Element.h"

// Parse a string like "(X + (V+M+A) + C +(M+L + (C + C) + C))"
// and create an IEquation object from it.
// Axioms:
// - all characters except ()+ and space are treated as a valid leafs for equation
// - spaces are ignored
// - all brackets should be paired
// - two terms should be separated with a +
// - a + should always separate two terms (no + at the beginning/end of the line)
TEquationPtr ParseString(const char* str);


// Tests
void TestClosingBracket();