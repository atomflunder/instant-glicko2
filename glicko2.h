#ifndef _GLICKO2_H_

const double DEFAULT_RATING = 1500.0;
const double DEFAULT_DEVIATION = 350.0;
const double DEFAULT_VOLATILITY = 0.06;
const double DEFAULT_TAU = 0.5;
const double DEFAULT_CONVERGENCE_TOLERANCE = 0.000001;
const double DEFAULT_CERTAINTY_FACTOR = 1.96;

struct Glicko2Player
{
    double rating;
    double deviation;
    double volatility;
};

enum Glicko2Outcome
{
    PLAYER_ONE_WIN,
    PLAYER_TWO_WIN,
    DRAW,
};

struct Glicko2Match
{
    struct Glicko2Player player_one;
    struct Glicko2Player player_two;
    enum Glicko2Outcome outcome;
    double tau;
    double convergence_tolerance;
};

struct ConfidenceInterval
{
    double high_end;
    double low_end;
};

struct ExpectedScore
{
    double player_one;
    double player_two;
};

extern struct Glicko2Match glicko2(struct Glicko2Match match);

extern struct ExpectedScore expected_score(struct Glicko2Match match);

extern struct Glicko2Player decay_deviation(struct Glicko2Player player);

extern struct ConfidenceInterval confidence_interval(struct Glicko2Player player, double certainty_factor);

void pretty_print_player(struct Glicko2Player player);

#endif