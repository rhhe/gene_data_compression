#ifndef _Ron_d10_c20_compress_cpp
#define _Ron_d10_c20_compress_cpp

#include <iostream>
#include <fstream>
#include <cstring>

#include "Ron_d10_c10_bin_file.h"
#include "Ron_d10_c20_compress.h"

using namespace std;

namespace RonForALi
{
void Compress_2_BinFile(const char* fn, const char* dirname)
{
//    GetDegreeOfMachine(fn, 30000);

    All_Info_data a1;
    a1.SetDirName_NPartMax(dirname);
    WriteBasePair_2_BinFile a2;
    a2.Open(dirname);
    WriteDegree_2_BinFile a4;
    a4.Open(dirname);

    string s1, s2, s3, s4;
    ifstream in(fn);
    if(!in)
    {
        cout << "!!!In RonForALi::Compress_2_BinFile(), the file is open failed!!!" << endl;
        exit(1);
    }
    do
    {
        if(in.eof())
            break;
        getline(in, s1);
        getline(in, s2);
        getline(in, s3);
        getline(in, s4);
//        PreDoForStringDegree(s4);
        a1.Fill(s1);
        a2.Write(s2);
        a4.Write(s4);
    }while(!in.eof());
    in.close();

    string fnlines_fn = string(dirname) + string("/fNLines.dat");
    ofstream out(fnlines_fn.c_str());
    out << a2.fNLines - 1 << endl;
    out.close();

    a1.Close();
    a2.Close();
    a4.Close();
}
void Uncompress_4_BinFile(const char* dirname, const char* fn)
{
    Read_All_Info_data b1;
    b1.Open(dirname);
    ReadBasePair_in_BinFile b2;
    b2.Open(dirname);
    ReadDegree_in_BinFile b4;
    b4.Open(dirname);

    int nlines;
    string fnlines_fn = string(dirname) + string("/fNLines.dat");
    ifstream in(fnlines_fn.c_str());
    if(!in)
    {
        cout << "!!!In RonForAli::Uncompress_4_BinFile(), the file is open failed!!!" << endl;
        exit(1);
    }
    in >> nlines;
    in.close();


    string s1, s2, s3, s4;
    ofstream out(fn);
    for(int i = 0; i < nlines; ++ i)
    {
        b1.Read();
        out << b1.fRes << endl;
        b2.Read();
        out << BasePair_NS::BasePair_2_String_res << endl;
        out << "+" << endl;
        b4.Read();
//        AfterDoForStringDegree(Degree_NS::Degree_2_String_Res);
        out << Degree_NS::Degree_2_String_Res << endl;
    }
    out.close();

    b1.Close();
    b2.Close();
    b4.Close();
}


}; // namespace RonForAli
#endif // _Ron_d10_c20_compress_cpp













