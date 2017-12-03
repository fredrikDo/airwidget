#ifndef LEAKAGE_H
#define LEAKAGE_H

// Included dependencies
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

enum NodeType
{
  ZONE,
  BOUNDARY
};

class Leakage 
{

private:
  vector<int> connections;
  vector<NodeType> nodeTypes;

public:
	int n1, n2;
  NodeType t1, t2;
	
	double T1, T2, P1, P2, Pstack1, Pstack2;
	
	char type;
	
  double h, C, n;

	Leakage(double, double, double, vector<int>, vector<NodeType>);
	
	double massflow(bool);
	double partialDerivative(bool);
};

#endif
