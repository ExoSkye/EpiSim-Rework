//
// Created by protobit on 05/10/2020.
//

#include "oclAlgo.h"

void oclAlgo::getArray(std::vector<human> *humans) {
    if (count == 1) {
        backup.resize(_x * _y, {human{0, 0, infectInfo::susceptible, false}, 0});
    }
    int i = 0;
    for (human person : *humans) {
        backup[(person.y*(_x-1))+person.x] = HumanIND{person, i};
        i++;
    }
}

void oclAlgo::clearArray(std::vector<human>* humans) {
    for (human person : *humans) {
        backup[(person.y*(_x-1))+person.x] = HumanIND{human{0, 0, infectInfo::susceptible, false}, 0};
    }
}

void
oclAlgo::run(std::vector<human> *humans, int infectChance, int infectRadius, int x, int y, double immuneChance,
             int immuneLength,
             int immuneLengthVar,
             int timestep) {
    TracyCZoneN(SetupArray,"Setup Arrays",true);
    _x = x;
    _y = y;
    count++;
    if (count == 1) {
        ZoneScopedN("Initial setup")
        for (human person : *humans) {
            px.emplace_back(person.x);
            py.emplace_back(person.y);
            pi.emplace_back((int)person.infect_info);
        }
        GridPeople = cl::Buffer(context, CL_MEM_READ_ONLY, sizeof(HumanIND)*x*y);
        peoplex = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int)*humans->size());
        peopley = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int)*humans->size());
        peoplei = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int)*humans->size());
        tempi = (int*)malloc(humans->size()*sizeof(int));
        tempx = (int*)malloc(humans->size()*sizeof(int));
        tempy = (int*)malloc(humans->size()*sizeof(int));
        auto randomDevice = std::random_device();
        random_ = new std::default_random_engine(randomDevice());
    }
    else {
        ZoneScopedN("Changing values")
#pragma omp simd
        for (int i = 0; i < humans->size(); i++) {
            human person = humans->at(i);
            px[i] = person.x;
            py[i] = person.y;
            pi[i] = (int) person.infect_info;
        }
    }
    //getArray(humans);
    TracyCZoneN(GenerateNums,"Generate Random Numbers",true);
    random = random_->operator()();
    TracyCZoneEnd(GenerateNums);
    TracyCZoneN(SendArrays,"Send Arrays to the GPU",true)
    //queue.enqueueWriteBuffer(GridPeople,CL_FALSE,0,_x*_y*sizeof(HumanIND),backup.data());
    queue.enqueueWriteBuffer(peoplex,CL_FALSE,0,humans->size()*sizeof(int),px.data());
    queue.enqueueWriteBuffer(peopley,CL_FALSE,0,humans->size()*sizeof(int),py.data());
    queue.enqueueWriteBuffer(peoplei,CL_FALSE,0,humans->size()*sizeof(int),pi.data());
    queue.finish();
    TracyCZoneEnd(SendArrays);
    TracyCZoneEnd(SetupArray);
    TracyCZoneN(KernelRun,"Run Kernel",true);
    move_infect.setArg(0,peoplex);
    move_infect.setArg(1,peopley);
    move_infect.setArg(2,peoplei);
    move_infect.setArg(3,random);
    move_infect.setArg(4, infectChance);
    move_infect.setArg(5, infectRadius);
    move_infect.setArg(6, x);
    move_infect.setArg(7, y);
    move_infect.setArg(8, static_cast<long>(humans->size()));
    move_infect.setArg(9,immuneChance);
    move_infect.setArg(10,timestep);
    int ret = queue.enqueueNDRangeKernel(move_infect,cl::NullRange,cl::NDRange(humans->size()),cl::NullRange);
    queue.finish();
    TracyCZoneEnd(KernelRun);
    TracyCZoneN(ReadDataBack,"Read Data from the GPU",true);
    queue.enqueueReadBuffer(peoplex,CL_FALSE,0,humans->size()*sizeof(int),tempx);
    queue.enqueueReadBuffer(peopley,CL_FALSE,0,humans->size()*sizeof(int),tempy);
    queue.enqueueReadBuffer(peoplei,CL_FALSE,0,humans->size()*sizeof(int),tempi);
    queue.finish();
    //clearArray(humans);
    TracyCZoneN(immuneCalc,"Check if people are still immune",true);
#pragma omp parallel for
    for (int i = 0; i < humans->size(); i++) {
        humans->at(i).x = tempx[i];
        humans->at(i).y = tempy[i];
        if (humans->at(i).infect_info == infectInfo::immune) {
            if (humans->at(i).time >=
                immuneLength + ((random_->operator()() % (immuneLengthVar * 2)) - immuneLengthVar)) {
                humans->at(i).infect_info = infectInfo::susceptible;
                humans->at(i).time = 0;
            }
        }
        else {
            humans->at(i).infect_info = (infectInfo) tempi[i];
        }
        humans->at(i).time++;
    }
    TracyCZoneEnd(immuneCalc);
    TracyCZoneEnd(ReadDataBack);
}

oclAlgo::~oclAlgo() {
    end();
}

void oclAlgo::end() {

}

oclAlgo::oclAlgo() {
    int ret = 0;
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    default_platform=all_platforms[0];
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0){
        std::cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    default_device=all_devices[0];
    std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";

    context = cl::Context({default_device},nullptr,nullptr,nullptr,&ret);

    std::ifstream file("kernel.cl");

    std::string str((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());

    sources.push_back({str.c_str(),str.length()});

    program = cl::Program(context,sources);
    if(program.build({default_device},"-I libs/")!=CL_SUCCESS){
        std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device)<<"\n";
        exit(1);
    }
    //create queue to which we will push commands for the device.
    queue = cl::CommandQueue(context,default_device,0,&ret);
    move_infect = cl::Kernel(program, "move_infect");
}
