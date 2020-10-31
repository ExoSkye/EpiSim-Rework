//
// Created by protobit on 05/10/2020.
//

#ifndef EPISIM_OCLALGO_H
#define EPISIM_OCLALGO_H

#include "algorithm.h"
#include "grapher.hpp"
#include <numeric>
#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.hpp>
#include <iostream>
#include <fstream>
#include <random>
class oclAlgo : public algo {
private:
    std::default_random_engine randomEngine;
    std::vector<long> random;
    std::minstd_rand minstdRand;
    cl::Platform default_platform;
    cl::Device default_device;
    cl::Context context;
    cl::Program::Sources sources;
    cl::Program program;
    cl::Buffer GridPeople;
    cl::Buffer peoplex;
    cl::Buffer peopley;
    cl::Buffer peoplei;
    std::vector<int> px;
    std::vector<int> py;
    std::vector<int> pi;
    cl::Buffer randomBuf;
    cl::Kernel move_infect;
    cl::CommandQueue queue;
    int totalInfected;
    int peopleInfected;
    int count = 0;
    int move_[7][2] = { {0,0},{0,1},{1,0},{1,1},{0,-1},{-1,0},{-1,-1} };
    std::default_random_engine* random_ = nullptr;
    std::vector<HumanIND> backup;
    int* tempi;
    int* tempx;
    int* tempy;
    void getArray(std::vector<human>* humans);
    void clearArray(std::vector<human>* humans);
    std::vector<int> infectPeople;
    int _x, _y;
    grapher* infectRate = nullptr;
    grapher* infectedPeople = nullptr;
public:
    oclAlgo();
    void
    run(std::vector<human> *humans, int infectChance, int infectRadius, int x, int y, double immuneChance,
        int immuneLength,
        int immuneLengthVar,int timestep) override;
    ~oclAlgo();
    void end() override;
};


#endif //EPISIM_OCLALGO_H
