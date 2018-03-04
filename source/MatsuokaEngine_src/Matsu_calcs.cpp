#include "Matsu_calcs.h"



double MatsuCalcs::nextVal(double in, double t1, double t2, double c1, 
							double c2, double b, double g)
{
	double step = 1.0 / STEP_DIVISION;
	double t1recip = 1.0 / t1;
	double t2recip = 1.0 / t2;

	STABILISE(&g, &c1, &c2, b, t1, t2recip);

	int i = (int)STEP_DIVISION;
	while (i--) {
		rkStep(in, t1recip, t2recip, c1, c2, b, g, step);
	}
	fixNAN();
	return POSPART(internals.x1) - POSPART(internals.x2);
}

void MatsuCalcs::rkStep(double in, double t1recip, double t2recip,
	double c1, double c2, double b, double g, double step)
{

	k1 = calc_deriv(in, t1recip, t2recip, c1, c2, b, g, internals);

	k2 = calc_deriv(in, t1recip, t2recip, c1, c2, b, g, 
						eulerStep(internals, k1, step*0.5));
	k3 = calc_deriv(in, t1recip, t2recip, c1, c2, b, g,
						eulerStep(internals, k2, step*0.5));
	k4 = calc_deriv(in, t1recip, t2recip, c1, c2, b, g,
						eulerStep(internals, k3, step));

	internals.x1 = internals.x1 + (k1.x1 + 2 * (k2.x1 + k3.x1) + k4.x1)* (1.0/6.0) * step;
	internals.x2 = internals.x2 + (k1.x2 + 2 * (k2.x2 + k3.x2) + k4.x2)* (1.0/6.0) * step;
	internals.v1 = internals.v1 + (k1.v1 + 2 * (k2.v1 + k3.v1) + k4.v1)* (1.0/6.0) * step;
	internals.v2 = internals.v2 + (k1.v2 + 2 * (k2.v2 + k3.v2) + k4.v2)* (1.0/6.0) * step;

}

// calculates derivative internals for matsuoka oscillator
MatsuCalcs::matsuoka_internals MatsuCalcs::calc_deriv(double in, double t1recip, double t2recip,
	double c1, double c2, double b, double g, matsuoka_internals state)
{
	double posX1 = POSPART(state.x1);
	double posX2 = POSPART(state.x2);
	double posIn = POSPART(in);
	double negIn = NEGPART(in);

	temp.x1 = (c1 - state.x1 - (b*(state.v1)) - (g*posX2) - posIn) * t1recip;
	temp.x2 = (c2 - state.x2 - (b*(state.v2)) - (g*posX1) + negIn) * t1recip;
	temp.v1 = (posX1 - state.v1) * t2recip;
	temp.v2 = (posX2 - state.v2) * t2recip;

	return temp;
}

// single step in euler's method calculation
MatsuCalcs::matsuoka_internals MatsuCalcs::eulerStep(matsuoka_internals init, 
														matsuoka_internals deriv, double step)
{
	temp.x1 = init.x1 + (deriv.x1*step);
	temp.x2 = init.x2 + (deriv.x2*step);
	temp.v1 = init.v1 + (deriv.v1*step);
	temp.v2 = init.v2 + (deriv.v2*step);
	return temp;
}


// enforces parameters within stable range (see K. Matsuoka, Biological cybernetics, 1985.)
void MatsuCalcs::enforceStability(double *g, double *c1, double *c2,
	double b, double t1, double t2Recip)
{
	double gMin = 1.0 + (t1 * t2Recip);
	*g = *g < gMin ? gMin : *g;

	double cMin = *g / (1.0 + b);
	*c1 = *c1 < cMin ? cMin : *c1;
	*c2 = *c2 < cMin ? cMin : *c2;
}



// protects against denormal
void MatsuCalcs::fixNAN()
{
	if (!isnormal(internals.x1) || !isnormal(internals.x2)
		|| !isnormal(internals.v1) || !isnormal(internals.v2)) {

		internals.x1 = 0, internals.x2 = 0, internals.v1 = 0, internals.v2 = 0;
	}
}