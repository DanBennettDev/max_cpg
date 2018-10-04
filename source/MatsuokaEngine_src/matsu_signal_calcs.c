
/*
matsuoka oscillator calculation functions
for MAX/MSP & PD matsuoka oscillator external
and Central Pattern Generator installation

*/

#include "matsu_signal_calcs.h"


// calculates the next value via Runge Kutta, via all intermediate steps
double matsuoka_calc_nextVal_RK(double in, double t1, double t2,
	double c1, double c2, double b, double g,
	matsuoka_internals *internals)
{
	double t1recip = 1.0 / t1;
	double t2recip = 1.0 / t2;

	STABILISE(&g, &c1, &c2, b, t1, t2recip);

	int i = (int)STEP_DIVISION;
	while (i--) {
		matsuoka_rkStep(in, t1recip, t2recip, c1, c2, b, g, internals);
	}
	matsuoka_fixNAN(internals);
	internals->out = POSPART(internals->x1) - POSPART(internals->x2);
	return internals->out;
}


// single step in Runge Kutta calculation
void matsuoka_rkStep(double in, double t1recip, double t2recip,
	double c1, double c2, double b, double g,
	matsuoka_internals *internals)
{
	matsuoka_internals k1, k2, k3, k4;

	double step = 1.0 / STEP_DIVISION;
	double halfStep = 0.5 / STEP_DIVISION;

	double oneSixthStep = (1.0 / 6.0) / STEP_DIVISION;

	k1 = matsuoka_calc_deriv(in, t1recip, t2recip, c1, c2, b, g,
		*internals);
	k2 = matsuoka_calc_deriv(in, t1recip, t2recip, c1, c2, b, g,
		matsuoka_eulerStep(*internals, k1, halfStep));
	k3 = matsuoka_calc_deriv(in, t1recip, t2recip, c1, c2, b, g,
		matsuoka_eulerStep(*internals, k2, halfStep));
	k4 = matsuoka_calc_deriv(in, t1recip, t2recip, c1, c2, b, g,
		matsuoka_eulerStep(*internals, k3, step));

	internals->x1 = internals->x1 + (k1.x1 + 2 * (k2.x1 + k3.x1) + k4.x1)* oneSixthStep;
	internals->x2 = internals->x2 + (k1.x2 + 2 * (k2.x2 + k3.x2) + k4.x2)* oneSixthStep;
	internals->v1 = internals->v1 + (k1.v1 + 2 * (k2.v1 + k3.v1) + k4.v1)* oneSixthStep;
	internals->v2 = internals->v2 + (k1.v2 + 2 * (k2.v2 + k3.v2) + k4.v2)* oneSixthStep;

}

// calculates derivative internals for matsuoka oscillator
matsuoka_internals matsuoka_calc_deriv(double in, double t1recip, double t2recip,
	double c1, double c2, double b, double g,
	matsuoka_internals internals)
{
	double posX1 = POSPART(internals.x1);
	double posX2 = POSPART(internals.x2);
	double posIn = POSPART(in);
	double negIn = NEGPART(in);

	matsuoka_internals deriv;
	deriv.x1 = (c1 - internals.x1 - (b*(internals.v1)) - (g*posX2) - posIn) * t1recip;
	deriv.x2 = (c2 - internals.x2 - (b*(internals.v2)) - (g*posX1) + negIn) * t1recip;
	deriv.v1 = (posX1 - internals.v1) * t2recip;
	deriv.v2 = (posX2 - internals.v2) * t2recip;

	return deriv;
}

// single step in euler's method calculation
matsuoka_internals matsuoka_eulerStep(matsuoka_internals init, matsuoka_internals deriv, double step)
{
	matsuoka_internals newVal;
	newVal.x1 = init.x1 + (deriv.x1*step);
	newVal.x2 = init.x2 + (deriv.x2*step);
	newVal.v1 = init.v1 + (deriv.v1*step);
	newVal.v2 = init.v2 + (deriv.v2*step);
	return newVal;
}

// enforces parameters within stable range (see K. Matsuoka, Biological cybernetics, 1985.)
void matsuoka_enforceStability(double *g, double *c1, double *c2,
	double b, double t1, double t2Recip)
{
	double gMin = 1.0 + (t1 * t2Recip);
	*g = *g < gMin ? gMin : *g;

	double cMin = *g / (1.0 + b);
	*c1 = *c1 < cMin ? cMin : *c1;
	*c2 = *c2 < cMin ? cMin : *c2;
}

// protects against denormal
void matsuoka_fixNAN(matsuoka_internals *internals)
{
	if (!isnormal(internals->x1) || !isnormal(internals->x2)
		|| !isnormal(internals->v1) || !isnormal(internals->v2)) {

		internals->x1 = 0, internals->x2 = 0, internals->v1 = 0, internals->v2 = 0;
	}
}





// Euler's method evaluation of the matsuoka osc - calculate once per phase cycle
double matsuoka_calc_nextVal(double in, double t1, double t2,
	double c1, double c2, double b, double g,
	matsuoka_internals *internals)
{
	double step = 1.0 / STEP_DIVISION;
	double posX1, posX2, posIn, negIn, dx1, dx2, dv1, dv2;
	double t1recip = 1.0 / t1;
	double t2recip = 1.0 / t2;


	for (int i = 0; i<DIVISIONS_PER_CYCLE; i++) {
		posX1 = POSPART(internals->x1);
		posX2 = POSPART(internals->x2);
		posIn = POSPART(in);
		negIn = NEGPART(in);
		dx1 = (c1 - internals->x1 - (b*(internals->v1)) - (posX2*g) - posIn) * t1recip;
		dx2 = (c2 - internals->x2 - (b*(internals->v2)) - (posX1*g) + negIn) * t1recip;
		dv1 = (posX1 - internals->v1) * t2recip;
		dv2 = (posX2 - internals->v2) * t2recip;
		internals->x1 += dx1 * step;         internals->x2 += dx2 * step;
		internals->v1 += dv1 * step;         internals->v2 += dv2 * step;
	}
	return POSPART(internals->x1) - POSPART(internals->x2);
}











