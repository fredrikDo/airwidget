#include "air/leakage.h"

// Powerlaw
Leakage::Leakage(double h_, double C_, double n_, vector<int> con_, vector<NodeType> types)
{
  h = h_;
  C = C_;
  n = n_;
  n1 = con_[0];
  n2 = con_[1];
  t1 = types[0];
  t2 = types[1];

  Pstack1 = 0;
  Pstack2 = 0;
}

double Leakage::massflow(bool lin)
{	
	// Positive flow direction from n1 to n2

  double n_ = n;

  if (lin)
  {
    n_ = 1.0;
  }

	double m;
	
	if (P2 > P1)
	{
		m = - C * pow((P2 - P1), n_);
	}
	
	else
	{
		m = C * pow((P1 - P2), n_);
	}
	
	return m;
}

double Leakage::partialDerivative(bool lin)
{
  double n_ = n;

  if (lin)
  {
    n_ = 1.0;
  }

	double m = abs(massflow(lin));

	double deltaP = abs(P1 - P2);

	if (m * deltaP != 0)
	{
		return n_ * m / deltaP;
	}
	else
	{
		return 0.0;
	}
}
