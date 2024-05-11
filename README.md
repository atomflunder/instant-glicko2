# Instant Glicko-2

Instant Glicko-2 in C, without rating periods.

## Examples & Usage

### Ratings
```c
void get_rating() {
    struct Glicko2Player p1 = {
        rating : 1500.0,
        deviation : 200.0,
        volatility : 0.06,
    };

    struct Glicko2Player p2 = {
        rating : 1400.0,
        deviation : 30.0,
        volatility : 0.06,
    };

    struct Glicko2Match m = {
        player_one : p1,
        player_two : p2,
        outcome : PLAYER_ONE_WIN,
        tau : DEFAULT_TAU, // 0.5
        convergence_tolerance : DEFAULT_CONVERGENCE_TOLERANCE, // 0.000001
    };

    m = glicko2(m);

    // Convenience function to print a player's stats.
    // Output:
    // Glicko-2 Rating: 1563.5641943063 (95% Interval: 1219.77499 - 1907.35340)
    // Glicko-2 Deviation: 175.4026559386
    // Glicko-2 Volatility: 0.0599986573048476162783871
    pretty_print_player(m.player_one);

    return;
}
```

### Expected Score

```c
void get_expected_score() {
    struct Glicko2Player p1 = {
        rating : 1320.0,
        deviation : 63.0,
        volatility : 0.06,
    };

    struct Glicko2Player p2 = {
        rating : 1420.0,
        deviation : 163.0,
        volatility : 0.06,
    };

    struct Glicko2Match m = {
        player_one : p1,
        player_two : p2,

    };

    struct ExpectedScore expsc = expected_score(m);

    // Output: Expected Score: 0.376740 - 0.623260
    printf("Expected Score: %f - %f\n", expsc.player_one, expsc.player_two);

    return;
}

```

### Confidence Interval

```c
void get_confidence_interval() {
    struct Glicko2Player p = {
        rating : 1320.0,
        deviation : 63.0,
        volatility : 0.06,
    };

    struct ConfidenceInterval c = confidence_interval(p, DEFAULT_CERTAINTY_FACTOR); // 1.96

    // Output: High: 1443.480000 - Low: 1196.520000
    printf("High: %f - Low: %f\n", c.high_end, c.low_end);

    return;
}
```

### Deviation Decay

```c
void decay_player() {
    struct Glicko2Player p1 = {
        rating : 1500.0,
        deviation : 30.0,
        volatility : 0.06,
    };

    struct Glicko2Player new_p = decay_deviation(p1);

    // Output: New Deviation: 31.759099
    printf("New Deviation: %f\n", new_p.deviation);
}
```

## Building

```bash
gcc src/glicko2.c src/glicko2_math.c -o glicko2.o -lm -fPIC && ar -rcs libglicko2.a glicko2.o 
```