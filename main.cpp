/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <cstdlib>

int main()
{
    int we3, ra1, ra2, wa3, wd3, rd1exp, rd2exp;
    
    for(int i = 0; i < 140; i++){
        we3 = rand() % 2;
        ra1 = rand() % 31 + 1;
        ra2 = rand() % 31 + 1;
        wa3 = rand() % 31 + 1;
        wd3 = rand() % pow(2, 32);
        rd1exp = rand() % 2;
        rd2exp = rand() % 2;
        cout << hex << we3;
        cout << "_";
        cout << hex << ra1;
        cout << hex << ra1;
        cout << "_";
        cout << hex << we3;
        cout << "_";
        cout << hex << we3;
        cout << "_";
        
        
    }

    return 0;
}
