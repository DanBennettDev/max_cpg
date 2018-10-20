#pragma once
/*!
AUTHOR:         Daniel Bennett
DATE:           17/03/2016
DESCRIPTION:    matsuoka's "Neural Oscillator" for Max MSP
matsuoka oscillator calculations
Evaluated via 4th order Runge Kutta method.
Max MSP infrastructure lives in db.matsuoka~ files

Version:        0.2



Max MSP infrastructure is in db.matsuoka.h

*/

#ifdef __cplusplus 
extern "C" {
#endif

#include <math.h>


#ifndef ENFORCE_STABILITY 
#define ENFORCE_STABILITY 1
#endif


#if(ENFORCE_STABILITY)
#define STABILISE(G,C1,C2,B,T1,T2) \
        matsuoka_enforceStability(G, C1, C2, B, T1, T2);
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
#define T_MIN       (0.004 )   
#define T_MAX       (5000000.0)

#define C_INIT     1.0
#define C_MIN     0.0
#define C_MAX     100.0        

#define B_INIT     4.07
#define B_MIN     1.0
#define B_MAX     1000.0

#define G_INIT     4.07
#define G_MIN     1.0
#define G_MAX     1000.0



	typedef struct _matsuoka_internals
	{
		// equation internal variables
		double x1;
		double x2;
		double v1;
		double v2;
		double out;
	}   matsuoka_internals;



	double matsuoka_calc_nextVal(double in, double t1, double t2,
		double c1, double c2, double b, double g,
		matsuoka_internals *state);

	double matsuoka_calc_nextVal_RK(double in, double t1, double t2,
		double c1, double c2, double b, double g,
		matsuoka_internals *state);

	matsuoka_internals matsuoka_calc_deriv(double in, double t1recip, double t2recip,
		double c1, double c2, double b, double g,
		matsuoka_internals state);

	matsuoka_internals matsuoka_eulerStep(matsuoka_internals init, matsuoka_internals deriv, double step);

	void matsuoka_rkStep(double in, double t1recip, double t2recip,
		double c1, double c2, double b, double g,
		matsuoka_internals *state);

	void matsuoka_fixNAN(matsuoka_internals *state);

	void matsuoka_enforceStability(double *g, double *c1, double *c2,
		double b, double t1, double t2);



#ifdef __cplusplus 
}
#endif