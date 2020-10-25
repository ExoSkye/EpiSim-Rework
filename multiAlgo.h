//
// Created by protobit on 05/10/2020.
//

#ifndef EPISIM_MULTIALGO_H
#define EPISIM_MULTIALGO_H
#include "algorithm.h"
#include "grapher.hpp"
#include <thread>
#include <numeric>
#include <functional>
#if !defined(OpenCL_Enable) && !defined(CUDA_Enable)
class multiAlgo : public algo {
private:
    int totalInfected;
    int peopleInfected;
    int count = 0;
    int move_[7][2] = { {0,0},{0,1},{1,0},{1,1},{0,-1},{-1,0},{-1,-1} };
    std::default_random_engine* random_ = nullptr;
    std::vector<std::vector<human*>> backup;
    std::vector<std::vector<human*>> getArray(std::vector<human>* humans);
    std::vector<int> infectPeople;
    int _x, _y;
    grapher* infectRate = nullptr;
    grapher* infectedPeople = nullptr;
    void threadedFunc(std::vector<human> *humans, std::vector<std::vector<human *>> *grid, int infectChance,
                      int infectRadius, int start, int end, double immuneChance, int immuneLength, int immuneLengthVar);
public:
    void
    run(std::vector<human> *humans, int infectChance, int infectRadius, int x, int y, double immuneChance, int immuneLength, int immuneLengthVar) override;
    ~multiAlgo();
    void end() override;
};

#elif defined(OpenCL_Enable)

#include "oclAlgo.h"
#define multiAlgo oclAlgo

#elif defined(CUDA_Enable)



#endif


#endif //EPISIM_MULTIALGO_H
