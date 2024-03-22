/*

min (max(S11 + Ti1))
SiTi S11 >= 0
Sij >= Sij-1 +tij-1
Sjk + Tjk <= Sik
Sjk >= Sik + Tik

for i to inf : T -> Schedule t 
if t = 0 
rand(seed)
delta t = val next - val current

*/

std::vector<int> generateRandomSolution(int numPorts) {

    std::vector<int> solution;
    for (int i = 0; i < numPorts; ++i) {
        solution.push_back(dis(gen));
    }

    return solution;
}


std::vector<int> simulatedAnnealing(double initialTemperature, double coolingRate, double minTemperature, int numPorts) {


    std::vector<int> currentSolution = generateRandomSolution(numPorts);
    std::vector<int> bestSolution = currentSolution;

    double currentTemperature = initialTemperature;

    while (currentTemperature > minTemperature) {
        std::vector<int> neighbourSolution = generateRandomSolution(numPorts);
        
        double currentCost = evaluate(currentSolution);
        double neighbourCost = evaluate(neighbourSolution);

        double acceptanceProbability = exp((currentCost - neighbourCost) / currentTemperature);

        if (neighbourCost < currentCost || dis(gen) < acceptanceProbability) {
            currentSolution = neighbourSolution;
            if (neighbourCost < evaluate(bestSolution)) {
                bestSolution = neighbourSolution;
            }
        }

        currentTemperature *= 1 - coolingRate;
    }

    return bestSolution;
}
