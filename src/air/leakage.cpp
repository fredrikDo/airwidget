#include "air/leakage.h"
#include <cmath>

// Powerlaw
Leakage::Leakage(double height, double Coeff, double n_exp, int node1, int node2)
{
	h = height;
	C = Coeff;
	n = n_exp;
	
	n1 = node1;
	n2 = node2;
}

double Leakage::massflow()
{	
	// Positive flow direction from n1 to n2

	double m;
	
	if (P2 > P1)
	{
		m = - C * pow((P2 - P1), n);
	}
	
	else
	{
		m = C * pow((P1 - P2), n);
	}
	
	return m;
}

double Leakage::partialDerivative()
{
	double m = abs(massflow());

	double deltaP = abs(P1 - P2);

	if (m * deltaP != 0)
	{
		return m / deltaP;
	}
	else
	{
		return 0.0;
	}
}
