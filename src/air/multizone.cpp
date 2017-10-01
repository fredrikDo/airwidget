#include "air/multizone.h"

MultiZone::MultiZone()
{
	Te = 273;
}

void MultiZone::addLeakage(double h, double C, double n, int node1, int node2)
{
	leakages.push_back(Leakage(h, C, n, node1, node2));
}

void MultiZone::addZone(double height, double temperature)
{
	zones.push_back(Zone());
	zones.back().h = height;
	zones.back().T = temperature;
	zones.back().P = 0.0;

	nodes.push_back(Node());
	nodes.back().P = 0.0;
	nodes.back().type = 'z';
	zones.back().id = nodes.size() - 1;
}

void MultiZone::addFixedPressure(double pressure)
{
	fixedPressures.push_back(FixedPressure());
	fixedPressures.back().P = pressure;

	nodes.push_back(Node());
	nodes.back().P = pressure;
	nodes.back().type = 'b';
	fixedPressures.back().id = nodes.size() - 1;
}

void MultiZone::update()
{
  // Copy zone pressures to nodes
  for (int i=0; i<zones.size(); i++)
  {
    nodes[zones[i].id].P = zones[i].P;
  }

  // Copy boundary pressures to nodes

	// Update pressures	
	for (int i=0; i<leakages.size(); i++)
	{
		leakages[i].P1 = nodes[leakages[i].n1].P + leakages[i].Pstack1;
		leakages[i].P2 = nodes[leakages[i].n2].P + leakages[i].Pstack2;	
	}
}

void MultiZone::calcStackEffect()
{
	double Patm = 101325.0;
    	double R = 287.058;
    	double g = 9.82;
            
  for (int i=0; i<leakages.size(); i++)
  {
    if (nodes[leakages[i].n1].type == 'b')
    {
      leakages[i].T1 = Te;
      leakages[i].T2 = zones[leakages[i].n2].T;
    }
    else if (nodes[leakages[i].n2].type == 'b')
    {
      leakages[i].T2 = Te;
      leakages[i].T1 = zones[leakages[i].n1].T;
    }
    else
    {
      leakages[i].T1 = zones[leakages[i].n1].T;
      leakages[i].T2 = zones[leakages[i].n2].T;
    }
  }
          
	for (int i=0; i<leakages.size(); i++)
	{
		leakages[i].Pstack1 = -(Patm / (R * leakages[i].T1)) * g * (1.0 - leakages[i].h);
		leakages[i].Pstack2 = -(Patm / (R * leakages[i].T2)) * g * (1.0 - leakages[i].h);
	}
}

double MultiZone::sumMassFlow(int zone)
{
	// // // // // // // // // // // // //
	//
	//  Calculates the total massflow to
	//  zone from all zones and boundaries
	//
	// // // // // // // // // // // // //

	double m = 0.0;

	for (int i=0; i<leakages.size(); i++)
	{
		// Calculate massflow to zone
		if (leakages[i].n1 == zone)
		{
			m -= leakages[i].massflow();
		}
		else if (leakages[i].n2 == zone)
		{
			m += leakages[i].massflow();
		}
	}
	return m;
}

double MultiZone::sumAbsMassFlow(int zone)
{
  double m = 0.0;

  for (int i=0; i<leakages.size(); i++)
  {
    if (leakages[i].n1 == zone || leakages[i].n2 == zone)
    {
      m += abs(leakages[i].massflow());
    }
  }
  return m;
}

double MultiZone::sumPartialDerivative(int z1, int z2)
{
	double pd = 0.0;

	for (int i=0; i<leakages.size(); i++)
	{
    // Boundaries
    if (nodes[leakages[i].n1].type == 'b' && leakages[i].n1 == z1)
    {
      pd -= leakages[i].partialDerivative();
    }
    else if (nodes[leakages[i].n2].type == 'b' && leakages[i].n2 == z2)
    {
      pd -= leakages[i].partialDerivative();
    }

    // Not a boundary
		else if (leakages[i].n1 == z1 && leakages[i].n2 == z2)
		{
		  pd += leakages[i].partialDerivative();
	  }
    else if (leakages[i].n1 == z2 && leakages[i].n2 == z1)
    {
      pd += leakages[i].partialDerivative();
    }
		else if (leakages[i].n1 == z1 && z1 == z2)
		{
		  pd -= leakages[i].partialDerivative();
		}
    /*else if (leakages[i].n2 == z1 && z1 == z2)
    {  
      pd -= leakages[i].partialDerivative();
    }*/
	}

	return pd;
}

void MultiZone::solve()
{
	calcStackEffect();
  update();

	int nZones = zones.size(); 
	
	VectorXd B(nZones);
	B.setZero();

	MatrixXd J(nZones, nZones);
	J.setZero();

	bool hasConverged = false;
  int iIterations = 0;

	while (!hasConverged)
	{
		// Calculate mass flows to each zone and store in vector B
		for (int i=0; i<nZones; i++)
		{
			B(i) = sumMassFlow(i); 
		}

		// Calculate Jacobian matrix J
		for (int i=0; i<nZones; i++)
		{
			for (int j=0; j<nZones; j++)
			{
				J(i,j) = sumPartialDerivative(i,j);
			}
		}

    cout << ".";
    //cout << "B1: " << B(0) << endl;
    //cout << "B2: " << B(1) << endl << endl;
    //cout << "J: " << J(0,0) << endl;

    VectorXd P;
    P = J.colPivHouseholderQr().solve(B);
	  
    // Update zone pressures
    for (int i=0; i<nZones; i++)
    {
      zones[i].P -= 0.75 * P(i);
    }

    update();

    //cout << "P1: " << zones[0].P << endl;
    //cout << "P2: " << zones[1].P << endl << endl << endl;

    // Convergence criteria
    double conv;
    for (int i=0; i<nZones; i++)
    {
      conv = abs(sumMassFlow(i)) / sumAbsMassFlow(i);
      if (conv < 0.00001)
      {
        hasConverged = true;
      }
      else
      {
        hasConverged = false;
        break;
      }
    }
    iIterations++;
    if (iIterations > 100)
    {
      hasConverged = true;
      cout << endl << "Failed to converge!" << endl;
    }
  }
}
