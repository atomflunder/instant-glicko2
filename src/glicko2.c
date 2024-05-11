#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "glicko2.h"
#include "glicko2_math.h"

const double SCALING_FACTOR = 173.7178;

struct Glicko2Match glicko2(struct Glicko2Match match)
{
    // Mainly follows the original paper: http://www.glicko.net/glicko/glicko2.pdf

    // Step 2
    double p1_rating = (match.player_one.rating - DEFAULT_RATING) / SCALING_FACTOR;
    double p2_rating = (match.player_two.rating - DEFAULT_RATING) / SCALING_FACTOR;

    double p1_deviation = (match.player_one.deviation) / SCALING_FACTOR;
    double p2_deviation = (match.player_two.deviation) / SCALING_FACTOR;

    float p1_outcome;
    float p2_outcome;

    switch (match.outcome)
    {
    case PLAYER_ONE_WIN:
        p1_outcome = 1.0;
        p2_outcome = 0.0;
        break;
    case DRAW:
        p1_outcome = 0.5;
        p2_outcome = 0.5;
        break;
    case PLAYER_TWO_WIN:
        p1_outcome = 0.0;
        p2_outcome = 1.0;
        break;

    default:
        printf("Glicko-2 Error: Invalid Outcome supplied.\n");
        return match;
    }

    // Step 3
    double g1 = g_value(p2_deviation);
    double g2 = g_value(p1_deviation);

    double e1 = e_value(p1_rating, p2_rating, g1);
    double e2 = e_value(p2_rating, p1_rating, g2);

    double v1 = v_value(g1, e1);
    double v2 = v_value(g2, e2);

    // Step 4
    double delta1 = delta_value(p1_outcome, v1, g1, e1);
    double delta2 = delta_value(p2_outcome, v2, g2, e2);

    // Step 5
    double p1_new_volatility = new_volatility(
        match.player_one.volatility,
        pow(delta1, 2.0),
        pow(p1_deviation, 2.0),
        v1,
        match.tau,
        match.convergence_tolerance);

    double p2_new_volatility = new_volatility(
        match.player_two.volatility,
        pow(delta2, 2.0),
        pow(p2_deviation, 2.0),
        v2,
        match.tau,
        match.convergence_tolerance);

    // Steps 6 & 7
    double p1_new_deviation = new_deviation(p1_deviation, p1_new_volatility, v1);
    double p2_new_deviation = new_deviation(p2_deviation, p2_new_volatility, v2);

    double p1_new_rating = new_rating(p1_rating, p1_new_deviation, p1_outcome, g1, e1);
    double p2_new_rating = new_rating(p2_rating, p2_new_deviation, p2_outcome, g2, e2);

    // Step 8
    struct Glicko2Player p1_new = {
        rating : p1_new_rating * SCALING_FACTOR + DEFAULT_RATING,
        deviation : p1_new_deviation * SCALING_FACTOR,
        volatility : p1_new_volatility,
    };

    struct Glicko2Player p2_new = {
        rating : p2_new_rating * SCALING_FACTOR + DEFAULT_RATING,
        deviation : p2_new_deviation * SCALING_FACTOR,
        volatility : p2_new_volatility,
    };

    struct Glicko2Match match_new = {
        player_one : p1_new,
        player_two : p2_new,
        outcome : match.outcome,
        tau : match.tau,
        convergence_tolerance : match.convergence_tolerance,
    };

    return match_new;
}

struct ExpectedScore expected_score(struct Glicko2Match match)
{
    // The win probability of player_one is E(μ, μj, √(φj^2 + φ^2)) instead of E(μ, μj, φj).
    // And for player_two we can just use 1.0 - the probability of player_one.
    double p1_rating = (match.player_one.rating - DEFAULT_RATING) / SCALING_FACTOR;
    double p2_rating = (match.player_two.rating - DEFAULT_RATING) / SCALING_FACTOR;

    double p1_deviation = (match.player_one.deviation) / SCALING_FACTOR;
    double p2_deviation = (match.player_two.deviation) / SCALING_FACTOR;

    double exp1 = e_value(p1_rating, p2_rating, g_value(hypot(p2_deviation, p1_deviation)));

    struct ExpectedScore exp = {
        player_one : exp1,
        player_two : 1.0 - exp1,
    };

    return exp;
}

struct ConfidenceInterval confidence_interval(struct Glicko2Player player, double certainty_factor)
{
    // Mentioned in the introduction of the original paper.
    struct ConfidenceInterval interval = {
        high_end : player.deviation * certainty_factor + player.rating,
        low_end : -player.deviation * certainty_factor + player.rating,
    };

    return interval;
}

struct Glicko2Player decay_deviation(struct Glicko2Player player)
{
    // Only performs Step 6 of the Glicko-2 calculations (+ Step 2 & Step 8 for conversion).
    double deviation = hypot(player.deviation / SCALING_FACTOR, player.volatility) * SCALING_FACTOR;

    // Setting 350.0 as the upper limit.
    // The original paper makes no mention of this, but this is common practice among the Glicko-2 based rating systems in use,
    // because it makes no sense to have a returning player with a higher deviation than someone who has never played.
    if (deviation > DEFAULT_DEVIATION)
    {
        deviation = DEFAULT_DEVIATION;
    }

    struct Glicko2Player new_p = {
        rating : player.rating,
        deviation : deviation,
        volatility : player.volatility,
    };

    return new_p;
}

void pretty_print_player(struct Glicko2Player player)
{
    struct ConfidenceInterval c = confidence_interval(player, DEFAULT_CERTAINTY_FACTOR);

    printf(
        "Glicko-2 Rating: %.6f (95%% Interval: %.2f - %.2f)\nGlicko-2 Deviation: %.6f\nGlicko-2 Volatility: %.15f\n\n",
        player.rating, c.low_end, c.high_end, player.deviation, player.volatility);
}

void main()
{
    struct Glicko2Player p1 = {
        rating : 1200.4,
        deviation : 45.0,
        volatility : 0.06
    };

    struct Glicko2Player p2 = {
        rating : 1610.4,
        deviation : 245.0,
        volatility : 0.06
    };

    struct Glicko2Match m = {
        player_one : p1,
        player_two : p2,
    };

    struct ExpectedScore ex = expected_score(m);
}
