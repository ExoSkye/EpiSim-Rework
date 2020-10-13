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
    int infect_info;
    int real;
    //TODO: Add more entries as needed
};

typedef struct HumanIND HumanIND;
struct HumanIND {
    human person;
    int i;
};

void kernel move_infect(global const HumanIND* grid, global int* personx, global int* persony, global int* personi,
                        global const long* randomVals, int infectChance, int infectRadius,int x, int y, ulong i,double immuneChance) {
    int temp_x = personx[get_global_id(0)]+randomVals[get_global_id(0)*4]%3-1;
    int temp_y = persony[get_global_id(0)]+randomVals[get_global_id(0)*4+1]%3-1;
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
    personx[get_global_id(0)] = temp_x;
    persony[get_global_id(0)] = temp_y;
    if (personi[get_global_id(0)] == 1) {
        if ((double)((randomVals[get_global_id(0)*4+2]) % 100001 )/1000 < immuneChance) {
            personi[get_global_id(0)]=3;
        }
        else {
        for ( int ind = 0;ind<i;ind++) {
            if (personx[ind] > (personx[get_global_id(0)] - infectRadius) && personx[ind]<(personx[get_global_id(0)] + infectRadius) && persony[ind]> (persony[get_global_id(0)]- infectRadius) && persony[ind] < (persony[get_global_id(0)] + infectRadius) && personi[ind] == 0) {
                if ((randomVals[get_global_id(0)*4+3])%101 < infectChance) {
                        personi[ind] = 1;
                    }
                }
            }
        }
    }
}
