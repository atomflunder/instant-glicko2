#ifndef _GLICKO2_H_

const double DEFAULT_RATING = 1500.0;
const double DEFAULT_DEVIATION = 350.0;
const double DEFAULT_VOLATILITY = 0.06;
const double DEFAULT_TAU = 0.5;
const double DEFAULT_CONVERGENCE_TOLERANCE = 0.000001;
const double DEFAULT_CERTAINTY_FACTOR = 2;

/**
 * A player's rating in the Glicko-2 system.
 * The default rating value is 1500.0, the default deviation is 350.0, and the default volatility is 0.06,
 * but all defaults can be accessed with the included constants (ex. DEFAULT_DEVIATION).
 */
struct Glicko2Player
{
    double rating;
    double deviation;
    double volatility;
};

/**
 * The outcome of a Match: `PLAYER_ONE_WIN`, `DRAW`, or `PLAYER_TWO_WIN`.
 */
enum Glicko2Outcome
{
    PLAYER_ONE_WIN,
    PLAYER_TWO_WIN,
    DRAW,
};

/**
 * A Glicko-2 Match with the player_one, player_two, outcome, tau and convergence_tolerance fields.
 * The default tau value is 0.5, and the default convergence_tolerance is 0.000001,
 * but all defaults can be accessed with the included constants (ex. DEFAULT_TAU).
 */
struct Glicko2Match
{
    struct Glicko2Player player_one;
    struct Glicko2Player player_two;
    enum Glicko2Outcome outcome;
    double tau;
    double convergence_tolerance;
};

/**
 * The X% (default: 95%) confidence interval of a player, with a low_end and a high_end field.
 * Only used as a return type in the `confidence_interval` function.
 */
struct ConfidenceInterval
{
    double high_end;
    double low_end;
};

/**
 * The expected outcome of a match, with fields for the expected score of player_one and player_two, between 0.0 and 1.0.
 * Only used as a return type in the `expected_score` function.
 */
struct ExpectedScore
{
    double player_one;
    double player_two;
};

/**
 * Calculates Glicko-2 Ratings.
 *
 * @param match A Glicko2Match struct, which needs the player_one, player_two, outcome, tau and convergence_tolerance fields.
 *              If in doubt, use the default values included here.
 * @returns     The Glicko2Match with the updated player_one and player_two fields.
 */
extern struct Glicko2Match glicko2(struct Glicko2Match match);

/**
 * Calculates an Expected Score in a Glicko-2 Match.
 *
 * @param match A Glicko2Match struct, which needs the player_one and player_two fields.
 * @returns     The ExpectedScore struct with the scores for player_one and player_two between 0.0 and 1.0.
 */
extern struct ExpectedScore expected_score(struct Glicko2Match match);

/**
 * Decays the rating deviation of a player.
 *
 * @param player The player to decay the deviation of.
 * @returns      The updated player struct.
 */
extern struct Glicko2Player decay_deviation(struct Glicko2Player player);

/**
 * A confidence interval of the rating a player.
 *
 * @param player           The player in question.
 * @param certainty_factor The certainty factor to multiply the deviation subtracted/added to the rating by.
 *                         If in doubt, use the default value of 2.0 (or `DEFAULT_CERTAINTY_FACTOR`), for a 95% confidence interval.
 * @returns                The ConfidenceInterval struct, with a high_end and low_end.
 */
extern struct ConfidenceInterval confidence_interval(struct Glicko2Player player, double certainty_factor);

/**
 * Prints the rating of a player in a useful format.
 *
 * @param player The player in question.
 */
void pretty_print_player(struct Glicko2Player player);

#endif