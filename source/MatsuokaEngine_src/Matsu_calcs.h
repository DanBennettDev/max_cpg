#pragma once
/*!
AUTHOR:         Daniel Bennett
DATE:           04/03/2018
DESCRIPTION:    matsuoka's "Neural Oscillator" for Max MSP
matsuoka oscillator calculations
Evaluated via 4th order Runge Kutta method.
Max MSP infrastructure lives in db.matsuoka~ files

Version:        0.2



Max MSP infrastructure is in db.matsuoka.h
*/




#include <math.h>


#ifndef ENFORCE_STABILITY 
#define ENFORCE_STABILITY 1
#endif


#if(ENFORCE_STABILITY)
#define STABILISE(G,C1,C2,B,T1,T2) \
        enforceStability(G, C1, C2, B, T1, T2);
#else 
#define STABILISE(G,C1,C2,B,T1,T2) 
#endif



#define POSPART(X)  (X > 0.0 ? X : 0.0)
#define NEGPART(X)  (X < 0.0 ? X : 0.0)

#define STEP_DIVISION 8.0
#define DIVISIONS_PER_CYCLE 2.0

#define STEP_MIN    1
#define STEP_MAX    64

// ranges for values into the equation
#define V_INIT      0.0

#define RATE_INIT   8400.0
#define RATE_MIN   0.0
#define RATE_MAX   44100.0

// representative values at oscilator peak
#define X1_INIT     0.4774
#define X2_INIT     -0.869708
#define V1_INIT     0.129358
#define V2_INIT     0.142406

#define T1_INIT     853.855
#define T2_INIT     3415.420
#define T_MIN       0.04     // must be >0.0
#define T_MAX       500000.0

#define C_INIT     1.0
#define C_MIN     0.0
#define C_MAX     100.0        

#define B_INIT     4.07
#define B_MIN     1.0
#define B_MAX     1000.0

#define G_INIT     4.07
#define G_MIN     1.0
#define G_MAX     1000.0


class MatsuCalcs
{
public:
	struct matsuoka_internals
	{
		// equation internal variables
		double x1{ 0 };
		double x2{ 0 };
		double v1{ 0 };
		double v2{ 0 };

	};


	double nextVal(double in, double t1, double t2,
		double c1, double c2, double b, double g);

	double x1() const
	{
		return internals.x1;
	}
	double x2() const
	{
		return internals.x2;
	}
	double v1() const
	{
		return internals.v1;
	}
	double v2() const
	{
		return internals.v2;
	}

	void x1(double val)
	{
		internals.x1 = val;
	}
	void x2(double val)
	{
		internals.x2 = val;
	}
	void v1(double val)
	{
		internals.v1 = val;
	}
	void v2(double val)
	{
		internals.v2 = val;
	}




private:


	matsuoka_internals internals, temp, k1, k2, k3, k4;

	matsuoka_internals calc_deriv(double in, double t1recip, double t2recip,
		double c1, double c2, double b, double g, matsuoka_internals state);

	matsuoka_internals eulerStep(matsuoka_internals init, matsuoka_internals deriv, double step);

	void rkStep(double in, double t1recip, double t2recip,
		double c1, double c2, double b, double g, double step);

	void fixNAN();

	void enforceStability(double *g, double *c1, double *c2,
		double b, double t1, double t2);




};