//
// Created by protobit on 05/10/2020.
//

#include "oclAlgo.h"

void oclAlgo::getArray(std::vector<human> *humans) {
    backup.clear();
    backup.resize(_x*_y,{human{0,0,infectInfo::susceptible,false},0});
    int i = 0;
    for (human person : *humans) {
        backup[person.x+person.y*_x] = HumanIND{person, i};
        i++;
    }
}


void oclAlgo::run(std::vector<human> *humans, int infectChance, int infectRadius, int x, int y) {
    _x = x;
    _y = y;
    count++;
    if (count == 1) {
        GridPeople = cl::Buffer(context, CL_MEM_READ_ONLY, sizeof(HumanIND)*x*y);
        people = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(human)*humans->size());
        temp = (human*)malloc(humans->size()*sizeof(human));
    }
    getArray(humans);
    random.resize(humans->size());
    std::generate(random.begin(), random.end(), std::rand);
    queue.enqueueWriteBuffer(GridPeople,CL_FALSE,0,_x*_y*sizeof(HumanIND),backup.data());
    queue.enqueueWriteBuffer(people,CL_FALSE,0,humans->size()*sizeof(human),humans->data());
    queue.enqueueWriteBuffer(randomBuf,CL_FALSE,0,humans->size()*sizeof(int),random.data());
    queue.finish();
    move_infect.setArg(0,GridPeople);
    move_infect.setArg(1,people);
    move_infect.setArg(2,randomBuf);
    move_infect.setArg(3, infectChance);
    move_infect.setArg(4, infectRadius);
    move_infect.setArg(5, x);
    move_infect.setArg(6, y);
    int ret = queue.enqueueNDRangeKernel(move_infect,cl::NullRange,cl::NDRange(humans->size()),cl::NullRange);
    queue.finish();
    queue.enqueueReadBuffer(people,CL_TRUE,0,_x*_y,temp);
    for (int i = 0; i < humans->size(); i++) {
        humans->at(i) = temp[i];
    }
}

oclAlgo::~oclAlgo() {

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

    context = cl::Context({default_device},NULL,NULL,NULL,&ret);

    std::ifstream file("kernel.cl");

    std::string str((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());

    sources.push_back({str.c_str(),str.length()});

    program = cl::Program(context,sources);
    if(program.build({default_device})!=CL_SUCCESS){
        std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device)<<"\n";
        exit(1);
    }

    //create queue to which we will push commands for the device.
    queue = cl::CommandQueue(context,default_device,0,&ret);

    move_infect = cl::Kernel(program, "move_infect");
}
