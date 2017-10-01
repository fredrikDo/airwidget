#ifndef LEAKAGE_H
#define LEAKAGE_H

// Included dependencies
#include <vector>
#include <iostream>

using namespace std;

class Leakage 
{

private:
	
public:

	double T1, T2, P1, P2, Pstack1, Pstack2;
	int n1, n2;
	
	double h, C, n;
	char type;

	Leakage(double, double, double, int, int);
	
	double massflow();
	double partialDerivative();
};

#endif
