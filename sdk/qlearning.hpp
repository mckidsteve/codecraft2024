const int States = 4;
const int Actions = 9;


std::vector<std::vector<double>> Q(States , std::vector<double>(Actions , 0));

double getReward(int state , int action){
    return getvalue(state) / getcost(action);
}

void qlearning (int numEp , double learnrate , double discountfactor , double exp_rate){

    for (int ep = 0 ; ep < numEp ; ++ep){
        int state = 0;
        while( state != States - 1){
            int action = std::distance(Q[state].begin() , std::max_element(Q[state].begin() , Q[state].end()));

            double reward = getReward(state , action);
            int newState = std::min(state + action , States - 1);

            Q[state][action] += learnrate * (reward + discountfactor * (*std::max_element(Q[newState].begin() , Q[newState].end() - Q[state][action])));
            state = newState;
        }
    }
}