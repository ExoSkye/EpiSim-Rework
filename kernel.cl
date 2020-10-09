typedef struct color color;
struct color
{
    int r;
    int g;
    int b;
};


typedef enum infectInfo infectInfo;
enum infectInfo
{
    susceptible,
    infectious,
    infectious_no_symptoms,
    immune,
    dead,
};

typedef struct human human;
struct human
{
    int x;
    int y;
    infectInfo infect_info;
    bool real;
    //TODO: Add more entries as needed
};

typedef struct HumanIND HumanIND;
struct HumanIND {
    human person;
    int i;
};

void kernel simple_add(global const int* A, global const int* B, global int* C){
    C[get_global_id(0)]=A[get_global_id(0)]+B[get_global_id(0)];
}

uint getRandom(ulong randoms) {
    ulong seed = randoms + get_global_id(0);
    seed = (seed * 0x5DEECE66DL + 0xBL) & ((1L << 48) - 1);
    return seed >> 16;
}

void kernel move_infect(global const HumanIND* grid, global human* humans, int infectChance, int infectRadius,
                        int x, int y, global const int* randomVals) {
    global human* person = &humans[get_global_id(0)];
    int temp_x = person->x+(getRandom(randomVals[get_global_id(0)])%3-1);
    int temp_y = person->y+(getRandom(randomVals[get_global_id(0)]+temp_x*randomVals[get_global_id(0)])%3-1);
    if (temp_x > x) {
        temp_x = x;
    }
    else if (temp_x < 0) {
        temp_x = 0;
    }
    if (temp_y > y) {
        temp_y = y;
    }
    else if (temp_y < 0) {
        temp_y = 0;
    }
    person->x = temp_x;
    person->y = temp_y;
}