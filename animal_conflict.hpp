#pragma once 
#include<iostream>
#include<fstream>
#include<random>
#include<algorithm>
/**
 * Animal class.
 */
class Animal{
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
    Animal(){
        /**
         * All agents start with 0 payoff
         */
        payoff = 0;
        win = false; // 0
        seriously_injured = false;
        /**
         * Dangerous attacks 'D' have high probability of serious damage
         */
        prob_injury = 0.4; // Probability of serious damage from an attack.
        /**
         * Retaliator will retaliate with high probability of a dangerous attack  
         */ 
        ret_prob = 0.9; // Retaliation probability. 

    };
    /**
     * Helper function to generate uniform random variables. 
     */
    double get_rv();
};

double Animal::get_rv(){
    {
    std::random_device rv;
    std::mt19937_64 gen(rv());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(gen);
};

}
// Mouse class 

class Mouse: public Animal{
    public: 
    char& rule(const char & opp_move);
};
char& Mouse::rule(const char& opp_move){
    if(opp_move == 'D'){
        next_move= 'R';
    }else if (opp_move == 'C'){
        next_move = 'C';
        prev_move ='C';
    }else if(opp_move =='R'){
        next_move = 'C';
        prev_move= 'C';
        win=true; // 1 
    }else{
        std::cerr<<"That is an illegal move"<<std::endl;
    };
    return next_move;
};

// Hawk Class 

class Hawk: public Animal{
    public:
    char& rule(const char& opp_move);
};

char& Hawk::rule(const char& opp_move){
    if(opp_move == 'D'){
        double u = get_rv();
        if(u < prob_injury){
            next_move ='R';
            prev_move ='R';
            win=false;
            seriously_injured=true;
            payoff -= 100;
        }else{
            next_move = 'D';
            prev_move ='D';

            payoff -= 2;
        }
    }
    return next_move;
};

// Bully class 

class Bully: public Animal{
    public:
    char& rule(const char& opp_curr_move, const char& opp_prev_move); 
};

char& Bully::rule(const char& opp_curr_move, const char& opp_prev_move){
    if(opp_curr_move =='C'){
        next_move = 'D';
        prev_move ='D';

    }else if(opp_curr_move =='D' && opp_prev_move=='D'){
        double u = get_rv();
        if(u < prob_injury){
            next_move ='R';
            prev_move ='R';
            win=false;
            seriously_injured=true;
            payoff -= 100;

        }else{
            next_move = 'R';
            prev_move ='R';
            win=false;
            payoff -= 2;

        }
    }else if(opp_curr_move =='D'){
        double u = get_rv();
        if(u < prob_injury){
            next_move ='R';
            prev_move ='R';
            win=false;
            seriously_injured=true;
            payoff -= 100;
        }else{
            next_move = 'C';
            prev_move ='C';
            payoff -= 2;
        }
    }
    return next_move;
}

// Retaliator class 

class Retliator: public Animal{
    public:
    char& rule(const char& opp_move);
};

// Retliator update rule

char& Retliator::rule(const char& opp_move){
    if(opp_move =='C'){
        next_move='C';   
        prev_move = 'C';  
    }else if(opp_move == 'D'){
        double u = get_rv();
        if(u < prob_injury){
            next_move ='R';
            prev_move ='R';
            win=false;
            seriously_injured=true;
            payoff -= 100;
        }else{
            double u = get_rv();
            if(u < ret_prob){
                next_move ='D';
                prev_move = 'D';
            }else{
                next_move ='C';
                prev_move = 'C';
            };
                payoff -= 2;
        };
    }else if(opp_move =='R'){
        win=true;
    }else{
        std::cerr<<"The opponents move is illegal"<<std::endl;
    };
    return next_move;
};