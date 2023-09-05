#include <iostream>
#include <fstream>

#include "../source/Ron_d10_c10_bin_file.h"
#include "../source/Ron_d10_c20_compress.h"

using namespace std;

int main()
{
    RonForALi::Compress_2_BinFile("res_b20_0.txt", "dir");
//    RonForALi::Uncompress_4_BinFile("dir", "res_b20_uncompress.txt");
    return 1;
}






