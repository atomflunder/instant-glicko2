#include <math.h>
#include <stdlib.h>
#include "glicko2_math.h"

/**
 * This file contains math stuff relating to steps 3 through 7 of the Glicko-2 algorithm outlined in the original paper.
 * http://www.glicko.net/glicko/glicko2.pdf
 */

double g_value(double opponent_deviation)
{
    // Part of Step 3
    return 1.0 / sqrt(1.0 + ((3.0 * pow(opponent_deviation, 2.0)) / pow(M_PI, 2.0)));
}

double e_value(double rating, double opponent_rating, double g)
{
    // Part of Step 3
    return 1.0 / (1.0 + exp(-g * (rating - opponent_rating)));
}

double v_value(double g, double e)
{
    // Part of Step 3
    return 1.0 / (pow(g, 2.0) * e * (1.0 - e));
}

double delta_value(double outcome, double v, double g, double e)
{
    // Step 4
    return v * (g * (outcome - e));
}

double f_value(double x, double delta_squared, double deviation_squared, double v, double volatility, double tau)
{
    // Step 5.1
    double i = exp(x) * (delta_squared - deviation_squared - v - exp(x));
    double j = 2.0 * pow(deviation_squared + v + exp(x), 2.0);
    double k = (x - log(pow(volatility, 2.0))) / pow(tau, 2.0);

    return (i / j) - k;
}

double new_volatility(double volatility, double delta_squared, double deviation_squared, double v, double tau, double convergence_tolerance)
{
    // Step 5.2
    double a = log(pow(volatility, 2.0));
    double b;

    if (delta_squared > (deviation_squared + v))
    {
        b = log(delta_squared - deviation_squared - v);
    }
    else
    {
        double k = 1.0;

        while (f_value(a - k * tau, delta_squared, deviation_squared, v, volatility, tau) < 0.0)
        {
            k += 1.0;
        }

        b = a - k * tau;
    }

    // Step 5.3
    double fa = f_value(a, delta_squared, deviation_squared, v, volatility, tau);
    double fb = f_value(b, delta_squared, deviation_squared, v, volatility, tau);

    // Step 5.4
    while (fabs(b - a) > convergence_tolerance)
    {
        double c = a + ((a - b) * fa / (fb - fa));
        double fc = f_value(c, delta_squared, deviation_squared, v, volatility, tau);

        if (fc * fb <= 0.0)
        {
            a = b;
            fa = fb;
        }
        else
        {
            fa /= 2.0;
        }

        b = c;
        fb = fc;
    }

    // Step 5.5
    return exp(a / 2.0);
}

double new_deviation(double deviation, double new_volatility, double v)
{
    // Step 6
    double pre_deviation = hypot(deviation, new_volatility);

    // Part of Step 7
    return 1.0 / (sqrt(1.0 / pow(pre_deviation, 2.0) + (1.0 / v)));
}

double new_rating(double rating, double new_deviation, double outcome, double g, double e)
{
    // Part of Step 7
    return rating + (pow(new_deviation, 2.0) * g) * (outcome - e);
}