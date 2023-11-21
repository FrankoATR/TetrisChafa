#include <iostream>
#include <list>
#include <random>

class ProbabilisticGenerator {
public:
    ProbabilisticGenerator(const std::list<double>& probabilities) {
        double sum = 0.0;
        for (double prob : probabilities) {
            sum += prob;
        }
        if (sum != 1.0) {
            throw std::invalid_argument("Las probabilidades no suman 1");
        }

        cumulativeDistribution.push_back(probabilities.front());
        for (auto it = std::next(probabilities.begin()); it != probabilities.end(); ++it) {
            cumulativeDistribution.push_back(cumulativeDistribution.back() + *it);
        }
    }

    size_t generate() {
        double randValue = (double)rand() / RAND_MAX;
        auto it = cumulativeDistribution.begin();
        while (randValue > *it) {
            ++it;
        }
        return std::distance(cumulativeDistribution.begin(), it);
    }

private:
    std::list<double> cumulativeDistribution;
};