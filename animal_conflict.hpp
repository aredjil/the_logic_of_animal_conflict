#pragma once 
#include<iostream>
#include<random>
#include<string>
class Animal{
    public:
    size_t payoff; 
    char previous_strategy;
    char current_strategy;
    char next_strategy;
    size_t time_tolerance;
    bool seriously_injured=false;
    bool win=false;

    Animal(){};
    char mouse(const char &opp_move);
    char hawk(const char &opp_move);
    char bully(const char &opp_move, const int &move);
};

char Animal::mouse(const char &opp_move){
    if(opp_move == 'D'){
        next_strategy = 'R';
    }else{
        next_strategy = 'C';
    }
    return next_strategy;
}
char Animal::hawk(const char &opp_move){
    if(!seriously_injured){
        next_strategy='D';
    }else{
        next_strategy='R';
    }
    return next_strategy;
}
char Animal::bully(const char &opp_move, const int &move){
    if(move==0){
        next_strategy='D';
    }
    if(opp_move=='C'){
        next_strategy='D';
    }else if(opp_move=='D'){
        
    }
    return next_strategy;

}
