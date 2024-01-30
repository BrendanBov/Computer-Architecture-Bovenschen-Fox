// RegfileTestVectorGeneration.cpp 
//

#include <iostream>
#include <iomanip>
using namespace std;

// Print a single vector in the format 
// we3_ra1_ra2_wa3_wd3_rd1exp_rd2exp
void PrintVector(int we3, int ra1, int ra2, int wa3, int wd3, int rd1exp, int rd2exp)
{
    cout << hex << setw(1) << we3 << "_";
    cout << hex << setw(2) << setfill('0') << ra1 << "_";
    cout << hex << setw(2) << setfill('0') << ra2 << "_";
    cout << hex << setw(2) << setfill('0') << wa3 << "_";
    cout << hex << setw(8) << setfill('0') << wd3 << "_";
    cout << hex << setw(8) << setfill('0') << rd1exp << "_";
    cout << hex << setw(8) << setfill('0') << rd2exp;

    cout << '\n';
}

int main()
{
    //initialize I/O values
    int we3 = 1;
    int ra1 = 0;
    int ra2 = 0;
    int wa3 = 0;
    int wd3 = 0;

    int rd1exp = 0;
    int rd2exp = 0;
    
    //each register is written a clock cycle before 
    for (int i = 1; i < 32; i++)//from 1 to 31
    {
        ra1 = ra2 = rd1exp = rd2exp = i;
        wa3 = wd3 = i + 1;

        PrintVector(we3,ra1,ra2,wa3,wd3,rd1exp,rd2exp);
    }
}
