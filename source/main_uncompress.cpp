#include <iostream>
#include <fstream>
#include <string>
#include "../source/Ron_d10_c10_bin_file.h"
#include "../source/Ron_d10_c20_compress.h"
using namespace std;

int main()
{
    string s1, s2;
    if(getline(cin, s1) && getline(cin, s2))
        RonForALi::Uncompress_4_BinFile(s1.c_str(), s2.c_str());
    else
        cout << "!!!File name error!!!" << endl;
    return 1;
}






