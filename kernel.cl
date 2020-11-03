#include <mt19937.cl>

void kernel move_infect(global int* personx, global int* persony, global int* personi,
                        const long randomVal, int infectChance, int infectRadius,int x, int y, ulong i,double immuneChance,int timestep) {
    mt19937_state state;
    mt19937_seed(&state,randomVal+get_global_id(0));
    int temp_x = personx[get_global_id(0)]+((int)(mt19937_uint(state) % (timestep * 2 + 1)) - timestep);
    int temp_y = persony[get_global_id(0)]+((int)(mt19937_uint(state) % (timestep * 2 + 1)) - timestep);
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
        if ((double)(mt19937_uint(state)*100000 % 100001 )/1000 < immuneChance) {
            personi[get_global_id(0)]=3;
        }
        else {
        for ( int ind = 0;ind<i;ind++) {
            if (personx[ind] > (personx[get_global_id(0)] - infectRadius) && personx[ind]<(personx[get_global_id(0)] + infectRadius) && persony[ind]> (persony[get_global_id(0)]- infectRadius) && persony[ind] < (persony[get_global_id(0)] + infectRadius) && personi[ind] == 0) {
                if ((int)mt19937_uint(state)%101 < infectChance) {
                        personi[ind] = 1;
                    }
                }
            }
        }
    }
}
