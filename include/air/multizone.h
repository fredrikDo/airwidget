#ifndef MULTIZONE_H
#define MULTIZONE_H

// Included dependencies
#include <vector>
#include <iostream>

// Eigen
#include <Eigen/Dense>

#include "leakage.h"

using namespace std;
using namespace Eigen;

struct Zone
{
	double P;

	double h;
	double T;

	int id;
};

struct FixedPressure
{
	double P;
	int id;
};

struct Node
{
	double P;
	char type;
};

class MultiZone
{
private:

	double Te;
	vector<Leakage> leakages;
	vector<Zone> zones;
	vector<FixedPressure> fixedPressures;
	vector<Node> nodes;
	
	double sumMassFlow(int);
	double sumAbsMassFlow(int);
  double sumPartialDerivative(int, int);
	
	void update(); 

public:

	MultiZone();
	
	void addLeakage(double, double, double, int, int);
	
	void addZone(double, double);
	
	void addFixedPressure(double);
	
	void solve();
	
	void calcStackEffect();
};

#endif
