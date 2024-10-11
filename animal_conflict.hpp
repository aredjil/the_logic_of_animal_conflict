#pragma once
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
/**
 * Animal class.
 * TODO: Add iteration count.
 * FIXME: Fix the order of instructions.
 */
// NOTE: Add simulation duration to the paramters of the objects.
class Animal
{
public:
    /**
     * Stores the payoff of each agent
     */
    int payoff;
    /*
    * Store the previous move of agent, moves can be:
        C: Conventional tactic.
        D: Dangerous tactic.
        R: Retreat.
    */
    /**
     * Store current move
     */
    char curr_move;
    /**
     * Store next move
     */
    char next_move;
    /**
     * Store if the agent won
     */
    bool win;
    /**
     * Is the gent serious injured
     */
    bool seriously_injured;
    /**
     * Store probability of injury
     */
    double prob_injury;
    /**
     * Store the probability of retiliation needed in Retaliator and Probe-Retaliator
     * agents
     */
    double ret_prob;
    /**
     * Time limit for the contest.
     */
    int t_lim;
    Animal()
    {
        /**
         * All agents start with 0 payoff
         */
        payoff = 0;
        win = false; // 0
        seriously_injured = false;
        /**
         * Dangerous attacks 'D' have high probability of serious damage
         */
        prob_injury = 0.9; // Probability of serious damage from an attack.
        /**
         * Retaliator will retaliate with high probability of a dangerous attack
         */
        ret_prob = 0.9; // Retaliation probability.
        t_lim = 100;    // Time limit for the contest
    };
    /**
     * Helper function to generate uniform random variables.
     */
    double get_rv();
};
/**
 * Helper function to generate a unifrom random variable (RV)
 * in the interval [0, 1], U(0, 1), for each run a new RV is generated.
 */
double Animal::get_rv()
{
    {
        std::random_device rv;                                 // Collect randomness from the device
        std::mt19937_64 gen(rv());                             // Seeded Pseudo-random number generator
        std::uniform_real_distribution<double> dist(0.0, 1.0); // U(0, 1)
        return dist(gen);                                      // Return a U(0, 1).
    };
}
/**
 * Mouse class
 */

class Mouse : public Animal
{
public:
    char &rule(const char &opp_move, const int &iter);
};
/**
 * Update rule for the mouse class
 * The agent always play C, unless
 * the opponent plays D, then it retreats
 * and plays R. If the game continious
 * past a predetermined time it retreats R.
 * NOTE: WHAT SHOULD WE DO IF THE CONTEST ENDS AND THE OPPONNENT PLAYS D?
 */
char &Mouse::rule(const char &opp_move, const int &iter)
{
    if (iter > t_lim)
    {
        next_move = 'R';
        curr_move = 'R';
    }
    else if (opp_move == 'D')
    {
        next_move = 'R';
        curr_move = 'R';
        double u = get_rv();
        if (u < prob_injury)
        {
            win = false;
            payoff -= 100;
            seriously_injured = true;
        }
        else
        {
            payoff -= 2;
        }
    }
    else if (opp_move == 'R')
    {
        win = true;
    }
    else
    {
        next_move = 'C';
        curr_move = 'R';
    }
    return next_move;
};

/**
 * Hawk Class
 */
class Hawk : public Animal
{
public:
    char &rule(const char &opp_move, const char &prev_move, const int &iter);
};
/**
 * Update rule for the Hawk Class,
 * Always play D, untill the time
 * threshold is reached or if
 * seriously injured then play R
 * or if the oponnent plays R.
 */
char &Hawk::rule(const char &opp_move, const char &prev_move, const int &iter)
{
    if (iter == 0)
    {
        /**
         * If playing the first move play D.
         */
        next_move = 'D';
        curr_move = 'D';
    }
    else
    {
        /**
         * If the oponnent retreats +60 in payoff if the time is less than some limit
         */
        if (opp_move == 'R')
        {
            if (iter < t_lim)
            {
                payoff += 60;
            }
        }
        /**
         * Reply to C with D 
         */
        else if (opp_move == 'C')
        {
            next_move = 'D';
            curr_move = 'D';
        }
        /**
         * Reply to D with C if not seriously injured. 
         */
        else if (opp_move == 'D')
        {
            double u = get_rv();
            if(u < prob_injury){
                payoff -=100;
                next_move = 'R';
                curr_move = 'R';
            }else{
                payoff -= 2;
                next_move = 'C';
                curr_move = 'C';
            }
        }
        /**
         * If the opponent plays D succesively retreat.
         */
        else if (opp_move == 'D' && prev_move == 'D')
        {
            next_move = 'R';
            curr_move = 'R';
        }
    }
    return next_move;
};

/**
 * Bully class
 */
class Bully : public Animal
{
public:
    char &rule(const char &opp_curr_move, const char &opp_prev_move, const int &iter);
};
/**
 * Update rule for the bully class
 * probe (play D in the first move)
 * reponsed to C by D, conversly,
 * responsed to D by C, play R if
 * opponent plays consuctive D.
 */
char &Bully::rule(const char &opp_curr_move, const char &opp_prev_move, const int &iter)
{
    if (opp_curr_move == 'C')
    {
        next_move = 'D';
    }
    else if (opp_curr_move == 'D' && opp_prev_move == 'D')
    {
        double u = get_rv();
        if (u < prob_injury)
        {
            next_move = 'R';

            win = false;
            seriously_injured = true;
            payoff -= 100;
        }
        else
        {
            next_move = 'R';

            win = false;
            payoff -= 2;
        }
    }
    else if (opp_curr_move == 'D')
    {
        double u = get_rv();
        if (u < prob_injury)
        {
            next_move = 'R';

            win = false;
            seriously_injured = true;
            payoff -= 100;
        }
        else
        {
            next_move = 'C';

            payoff -= 2;
        }
    }
    return next_move;
}

/**
 * Retaliator class
 */

class Retaliator : public Animal
{
public:
    char &rule(const char &opp_move);
};

/**
 * Retaliator update rule
 * play C in first move
 * repond to C by C
 * if opponent plays D
 * with high probability play D
 * play R after a threshold.
 */

char &Retaliator::rule(const char &opp_move)
{
    if (opp_move == 'C')
    {
        next_move = 'C';
    }
    else if (opp_move == 'D')
    {
        double u = get_rv();
        if (u < prob_injury)
        {
            next_move = 'R';

            win = false;
            seriously_injured = true;
            payoff -= 100;
        }
        else
        {
            double u = get_rv();
            if (u < ret_prob)
            {
                next_move = 'D';
            }
            else
            {
                next_move = 'C';
            };
            payoff -= 2;
        };
    }
    else if (opp_move == 'R')
    {
        win = true;
    }
    else
    {
        std::cerr << "The opponents move is illegal" << std::endl;
    };
    return next_move;
};
/**
 * Prober-Retaliator Class
 */
class Prober_Retaliator : public Animal
{
public:
    char &rule(const char &opp_move);
};

/**
 * Prober_Retaliator update rule
 * NOTE: comment it when you immplement the function.
 */

char &Prober_Retaliator::rule(const char &opp_move)
{
    /**
     * If the opponent plays C with hgih probability play C
     * And with low probability play D.
     */
    if (opp_move == 'C')
    {
        double u = get_rv();
        if (u < ret_prob)
        {
            next_move = 'C';
        }
        else
        {
            next_move = 'D';
        };
        /**
         * After giving a probe (provocation, playing D at the first move)
         * revert to C if the opponent retaliate (plays D), otherwise play D.
         */

        /**
         * After receiving a probe with hgih probability esclate (Play D as a response)
         */
    }
    return next_move;
};
