#ifndef MULTIZONE_H
#define MULTIZONE_H

// Included dependencies
#include <vector>
#include <iostream>

// Eigen
#include <Eigen/Dense>

//
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

struct Boundary
{
	double P;
	//int id;
};

struct Node
{
	double P;
	char type;
  int i;
};

class MultiZone
{
private:

	double Te;
	vector<Leakage> leakages;
	vector<Zone> zones;
  vector<Boundary> boundaries;
	
	double sumMassFlow(int, bool);
	double sumAbsMassFlow(int, bool);
  double sumPartialDerivative(int, int, bool);
	void calcStackEffect();
	void update(); 
  bool solver(bool lin);

public:

	MultiZone();
	
  void addLeakage(double, double, double, vector<int>, vector<NodeType>);
	
	void addZone(double, double);
	
	void addFixedPressure(double);
	
	void solve();

  double getLeakageMassflow(int i);

  void setZoneTemperature(double T_, int i);

  void adjustZoneTemperature(double T_, int i);

  double getZoneTemperature(int i);

  double getMaxLeakageMassFlow();
};

#endif
