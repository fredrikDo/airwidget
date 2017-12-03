#include "air/multizone.h"

MultiZone::MultiZone()
{
	Te = 273;
}

void MultiZone::addLeakage(double h, double C, double n, 
  vector<int> con, vector<NodeType> types)
{
  //
  // Postive flow direction is defined from lower node index
  // to higher node index/*
  leakages.push_back(Leakage(h, C, n, con, types)); 
  
  /*
  if (node1 > node2)
  {
	  leakages.push_back(Leakage(h, C, n, node2, node1));
  }
  else
  {
	  leakages.push_back(Leakage(h, C, n, node1, node2));
  }*/
}

void MultiZone::addZone(double h_, double T_)
{
	zones.push_back(Zone());
	zones.back().h = h_;
	zones.back().T = T_;
	zones.back().P = 0.0;
}

void MultiZone::addFixedPressure(double pressure)
{
	boundaries.push_back(Boundary());
	boundaries.back().P = pressure;
}

void MultiZone::update()
{
	// Update pressures	
	for (int i=0; i<leakages.size(); i++)
	{
		//leakages[i].P1 = nodes[leakages[i].n1].P + leakages[i].Pstack1;
		//leakages[i].P2 = nodes[leakages[i].n2].P + leakages[i].Pstack2;

    // Update 1st node
    if (leakages[i].t1 == ZONE)
    {
      leakages[i].P1 = zones[leakages[i].n1].P + leakages[i].Pstack1;
    }
    else if (leakages[i].t1 == BOUNDARY)
    {
      leakages[i].P1 = boundaries[leakages[i].n1].P + leakages[i].Pstack1;
    }
    // Update 2nd node
    if (leakages[i].t2 == ZONE)
    {
      leakages[i].P2 = zones[leakages[i].n2].P + leakages[i].Pstack2;
    }
    else if (leakages[i].t2 == BOUNDARY)
    {
      leakages[i].P2 = boundaries[leakages[i].n2].P + leakages[i].Pstack2;
    }
	}
}

void MultiZone::calcStackEffect()
{
	double Patm = 101325.0;
  double R = 287.058;
  double g = 9.82;
            
  for (int i=0; i<leakages.size(); i++)
  {
    if (leakages[i].t1 == BOUNDARY)
    {
      leakages[i].T1 = Te;
      leakages[i].T2 = zones[leakages[i].n2].T;
    }
    else if (leakages[i].t2 == BOUNDARY)
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

double MultiZone::sumMassFlow(int zone, bool lin)
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
		if (leakages[i].n1 == zone && leakages[i].t1 == ZONE)
		{
			m -= leakages[i].massflow(lin); // was (-)
		}
		else if (leakages[i].n2 == zone && leakages[i].t2 == ZONE)
		{
			m += leakages[i].massflow(lin);
		}
	}
	return m;
}

double MultiZone::sumAbsMassFlow(int zone, bool lin)
{
  double m = 0.0;

  for (int i=0; i<leakages.size(); i++)
  {
    if ((leakages[i].n1 == zone && leakages[i].t1 == ZONE)
          || (leakages[i].n2 == zone && leakages[i].t2 == ZONE))
    {
      m += abs(leakages[i].massflow(lin));
    }
  }
  return m;
}

double MultiZone::sumPartialDerivative(int z1, int z2, bool lin)
{
	double pd = 0.0;

  // May contain redundant functions and expressions.... CHANGE THIS!

	for (int i=0; i<leakages.size(); i++)
	{
    // Boundaries
    if (leakages[i].t2 == BOUNDARY && leakages[i].n1 == z1)
    {
      pd -= leakages[i].partialDerivative(lin);
    }
    else if (leakages[i].t2 == BOUNDARY && leakages[i].n1 == z2)
    {
      pd += leakages[i].partialDerivative(lin);
    }

    else if (leakages[i].t1 == BOUNDARY && leakages[i].n2 == z1)
    {
      pd -= leakages[i].partialDerivative(lin); 
    }
    else if (leakages[i].t1 == BOUNDARY && leakages[i].n2 == z2)
    {
      pd += leakages[i].partialDerivative(lin); 
    }

    // Not a boundary
		else if (leakages[i].n1 == z1 && leakages[i].n2 == z2)
		{
		  pd += leakages[i].partialDerivative(lin);
	  }
    else if (leakages[i].n1 == z2 && leakages[i].n2 == z1)
    {
      pd += leakages[i].partialDerivative(lin);
    }

    // Between the same zone
		else if (leakages[i].n1 == z1 && z1 == z2)
		{
		  pd -= leakages[i].partialDerivative(lin);
		}
    else if (leakages[i].n2 == z1 && z1 == z2)
    {  
      pd -= leakages[i].partialDerivative(lin);
    }
	}

	return pd;
}

double MultiZone::getLeakageMassflow(int i)
{
  return leakages[i].massflow(false);
}

void MultiZone::setZoneTemperature(double T_, int i)
{
  if (i < zones.size())
  {
    zones[i].T = T_;
  }
  else
  {
    cout << " " << endl;
  }
}

void MultiZone::adjustZoneTemperature(double T_, int i)
{
  if (i < zones.size())
  {
    zones[i].T += T_;
  }
  else
  {
    cout << " " << endl;
  }
}

double MultiZone::getZoneTemperature(int i)
{
  if (i < zones.size())
  {
    return zones[i].T;
  }
  else
  {
    return 0;
  }
}

double MultiZone::getMaxLeakageMassFlow()
{
  // Return maximum massflow in all leakages

  double maxMassFlow = 0.0;
  for (int i=0; i<leakages.size(); i++)
  {
    if (abs(leakages[i].massflow(false)) > maxMassFlow)
    {
      maxMassFlow = abs(leakages[i].massflow(false));
    }
  }
  return maxMassFlow; 
}

bool MultiZone::solver(bool lin)
{
  bool runningSolver = true;
  bool hasConverged = false;

	int nZones = zones.size(); 
  int iIterations = 0;
	
	VectorXd B(nZones);
	B.setZero();

	MatrixXd J(nZones, nZones);
	J.setZero();

  while (runningSolver)
	{
		// Calculate mass flows to each zone and store in vector B
		for (int i=0; i<nZones; i++)
		{
			B(i) = sumMassFlow(i,lin); 
		}

		// Calculate Jacobian matrix J
		for (int i=0; i<nZones; i++)
		{
			for (int j=0; j<nZones; j++)
			{
				J(i,j) = sumPartialDerivative(i,j,lin);
			}
		}
    //cout << "B: " << B << endl;
    //cout << "J: " << J << endl;

    VectorXd P;
    P = J.colPivHouseholderQr().solve(B);

    //cout << "P: " << P << endl << endl;
	  
    // Update zone pressures
    for (int i=0; i<nZones; i++)
    {
      zones[i].P -= 0.75 * P(i);
    }

    update();
  
    // Convergence criteria
    double conv;
    for (int i=0; i<nZones; i++)
    {
      conv = abs(sumMassFlow(i,lin)) / sumAbsMassFlow(i,lin);
      if (conv < 0.00001)
      {
        hasConverged = true;
        runningSolver = false;
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
      hasConverged = false;
      runningSolver = false;
      cout << endl << "Failed to converge!" << endl;

      // reset values to zero
      for (int i=0; i<leakages.size(); i++)
      {
        leakages[i].P1 = 0.0;
        leakages[i].P2 = 0.0;
      }
      for (int i=0; i<zones.size(); i++)
      {
        zones[i].P = 0.0;
      }
    }
  }
  return hasConverged;
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

	//bool hasConverged = false;
  //int iIterations = 0;
  //bool lin = true;

  if (solver(true)) // Start with linnear initialization, proceed if convergence
  {
    solver(false);
  }

  /*
  while (!hasConverged)
	{
    if (iIterations > 20)
    {
      lin = false;
    }

		// Calculate mass flows to each zone and store in vector B
		for (int i=0; i<nZones; i++)
		{
			B(i) = sumMassFlow(i,lin); 
		}

		// Calculate Jacobian matrix J
		for (int i=0; i<nZones; i++)
		{
			for (int j=0; j<nZones; j++)
			{
				J(i,j) = sumPartialDerivative(i,j,lin);
			}
		}
    //cout << "B: " << B << endl;
    //cout << "J: " << J << endl;

    VectorXd P;
    P = J.colPivHouseholderQr().solve(B);

    //cout << "P: " << P << endl << endl;
	  
    // Update zone pressures
    for (int i=0; i<nZones; i++)
    {
      zones[i].P -= 0.75 * P(i);
    }

    update();
  
    // Convergence criteria
    double conv;
    for (int i=0; i<nZones; i++)
    {
      conv = abs(sumMassFlow(i,lin)) / sumAbsMassFlow(i,lin);
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
    
      // reset values to zero
      for (int i=0; i<leakages.size(); i++)
      {
        //leakages[i].P1 = 0.0;
        //leakages[i].P2 = 0.0;
      }
    }
  }*/
}
