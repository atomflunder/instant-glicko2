#ifndef _GLICKO2_MATH_H_

#define M_PI 3.14159265358979323846

double g_value(double opponent_deviation);

double e_value(double rating, double opponent_rating, double g);

double v_value(double g, double e);

double delta_value(double outcome, double v, double g, double e);

double f_value(double x, double delta_squared, double deviation_squared,
               double v, double volatility, double tau);

double new_volatility(double volatility, double delta_squared,
                      double deviation_squared, double v, double tau,
                      double convergence_tolerance);

double new_deviation(double deviation, double new_volatility, double v);

double new_rating(double rating, double new_deviation, double outcome, double g,
                  double e);

#endif
