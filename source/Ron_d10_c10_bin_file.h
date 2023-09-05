#ifndef _Ron_d10_c10_bin_file_h
#define _Ron_d10_c10_bin_file_h

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

namespace RonForALi
{
//------------------------------------------------------------------------------------------
namespace BinData_NS
{
unsigned char char1 = 1;
unsigned char char01111111 = 127;
unsigned char char10000000 = 128;
char fChardifferent1[8] = {-128, 64, 32, 16, 8, 4, 2, 1};
};
class BinData
{
public:
    BinData();
    BinData(int N);
    virtual ~BinData();
    void SetN0(int N);
    void SetN(int N);
    char * fChar;
    int fN;//n for 0 or 1
    int fNChar;
    void Write(int ibin, bool val);
    bool Read(int ibin);
private:
    char * m_p_char;
    int m_N_first;
};
//------------------------------------------------------------------------------------------
namespace BinFile_NS
{
const int mode_recreate = 0;
const int mode_read     = 1;
};
class BinFile
{
public:
    BinFile();
    virtual ~BinFile();
    bool Open(const char * fn, const char * mode = "recreate");//"read"

    void Write_A_BinData(const BinData * bin_d);
    void Write_A_IntData(const int * int_d);
    void Write_A_ShortData(const short * short_d);
    void Write_A_CharData(const char * char_d);

    void Read_A_BinDataAddress(BinData * bin_d);
    void Read_A_IntDataAddress(int * int_d);
    void Read_A_ShortDataAddress(short * short_d);
    void Read_A_CharDataAddress(char * char_d);

    void Close(void);
    int fMode;//0 recreate, 1 read
    ofstream out;
    ifstream in;
private:
};
//-------------------------------------------------------------------------------------------
namespace BasePair_NS
{
int char_N_no[1000];
char * c;
char BasePair_2_String_res[1000];
};
class BasePair : public BinData
{
//used for transform BasePair string to bindata;///////////////////////
public:
    //ACGT: 00 01 10 11
    BasePair();
    virtual ~BasePair();
    bool SetString(string s);
    int fNumChar_N;
    int * fChar_N_No;
    bool flag_newstring_is_same_with_old_string;
private:
    char fOldString[200];
};
void BasePair_2_String_without_N(BinData * a);
void BasePair_2_String_with_N(BinData * a, int n, int * posi);
//used for transform BasePair to string;        ///////////////////////
class WriteBasePair_2_BinFile
{
// used for getting and saving the compressed data
public:
    void Open(const char * dir_name, int NPairMax = 200);
    void Write(string s);
    void Close(void);
    int      fNLines;//number of strings;
private:
    BasePair fBasePair;
    BinFile  fBF_4_BP;//bin file for base pair
    BinFile  fBF_4_Info_pair;//bin file for number pair
    BinFile  fBF_4_Info_N;//bin file for number of N
    BinFile  fBF_4_N;//bin file for N
    BinFile  fBF_4_repeat;//bin file for base pair string repeat
    int      fN_BasePair;//length of base-pair string
    int      fN_N;//number of N
    int      fBasePair_repeat_num;//length-of-base-pair-string repeat
    int      fN_repeat_num;//number-of-N repeat
    char     fCharForBasePairRep;
    //save for 8 string with bool value
    //for note that new-string is same with old string,
    //same:1, different:0
    int     fNLines_L_8;//fNLines % 8
};
class ReadBasePair_in_BinFile
{
// used for getting BasePair from bin file
// result are save in BasePair_NS::BasePair_2_String_res;
public:
    void Open(const char * dir_name, int NPairMax = 200);
    void Read(void);
    void Close(void);
private:
    BasePair fBasePair;
    BinFile  fBF_4_BP;//bin file for base pair
    BinFile  fBF_4_Info_pair;//bin file for number pair
    BinFile  fBF_4_Info_N;//bin file for number of N
    BinFile  fBF_4_N;//bin file for N
    BinFile  fBF_4_repeat;//bin file for base pair string repeat
    int      fN_BasePair;
    int      fN_N;
    int      fBasePair_repeat_num;
    int      fN_repeat_num;
    char     fCharForBasePairRep;
    int      fNPosi[200];
    int      fNLines;
    int      fNLines_L_8;
};
//--------------------------------------------------------------------------------------------
namespace Degree_NS
{
short num1[200];
short num2[200];
short num3[200];
short long_num1[200];
short long_num2[200];
short long_num3[200];
int short_2_long(int na, short * a, short * b);
char Degree_2_String_Res[200];
char * c;
int nb;
int ib;
};
class Degree : public BinData
{
//used for transform Degree string to bindata;      /////////////////////////
public:
    void SetString(string s);
    char fN1, fN2, fN3;
private:
    static int long_2_short(int n, short * a);
    static short num1[200];
    static short num2[200];
    static short num3[200];
};
void Degree_2_String(BinData * a, int n1, int n2, int n3);
//used for transform BinData to Degree string;////////////////////////////////
//BinData->SetN should be set to n1+n2+n3;//n1,n2,n3 should be save as 3 char
class WriteDegree_2_BinFile
{
public:
    void Open(const char * dir_name, int NCharMax = 200);
    void Write(string s);
    void Close(void);
private:
    Degree  fDegree;
    BinFile fBF_4_DG; // bin-file for saving degree;
    BinFile fBF_4_3Char; // bin-file for saving n1, n2, n3;
    int     fNLines;
};
class ReadDegree_in_BinFile
{
    //Results are saved in Degree_NS::Degree_2_String_Res[];
    //fDegree.fN should be (fN1+fN2+fN3) * 4
public:
    void Open(const char * dir_name, int NCharMax = 200);
    void Read(void);
    void Close(void);
    int     fNLines;
private:
    Degree  fDegree;
    BinFile fBF_4_DG;
    BinFile fBF_4_3Char;
    char    fN1, fN2, fN3;
    int     fN_for_fDegree;//(fN1+fN2+fN3) * 4
};
//--------------------------------------------------------------------------------------------
namespace Info_NS
{
bool DecomposeInfoString(string s);
bool PartType[100]; // 0:string 1:int
int  IntPart[100];
char StringPart[100][100];

char s_char[100];
char * c;
int  NPart;
int  ipart;
int  ic;
bool LastType;
bool ThisType;
int  slength;
int  starti;
int  endi;
int  i;
int  j;
int  cstring_2_int(char * pc);
char * cc;
int  k;
int  res_a2i;

int  flag_int_2_string = (-127 << 24) + (-127 << 16) + (-127 << 8) + (-127);
int  flag_int_repeat   = (-127 << 24) + (-127 << 16);
char flag_string_2_int = -31;
char flag_string_repeat = -30;
char flag_no_char = char(2);
char flag_c1 = char(1);
char flag_c0 = char(0);

}; // namespace Info_NS

class Info_data
{
public:
    bool fType;
    int  fInt;
    char fString[200];
    char fStringLength;
    int  fStep;
    int  fStepRepeatNum;
    int  fStringRepeatNum;
};
class Two_Info_data
{
public:
    Two_Info_data();
    virtual ~Two_Info_data();
    void SetIPart(int ipart);
    Info_data * fData1;
    Info_data * fData2;
    void Fill(void);
    int  fIPart;
    bool ChangeType_2_String;
    bool ChangeType_2_Int;
    bool SameType_and_SameString;
    bool SameType_and_SameStepInt;
    bool ShouldOutput;
    void Reset(void);
private:
    void Save_Data2_to_Data1(void);
    Info_data * fDataSwap;
    bool *      m_type;
    int *       m_int;
    char *      m_pc;
};
class All_Info_data
{
public:
    All_Info_data();
    virtual ~All_Info_data();
    void SetDirName_NPartMax(const char * dirname, int npartmax = 50);
    Two_Info_data * fTwoInfoData;
    BinFile * fBinfile;
    void Fill(string s);
    int fNLines;
    int fNPartMax;
    void Close(void);
private:
    void Fill0(void);
};
class Read_All_Info_data
{
public:
    void Open(const char * dirname, int npartmax = 50);
    void Read(void);
    void Close(void);
    char fRes[1000];
private:
    int  fNPartMax;
    void ReadFile(int i);
    char fResFile[100][100];
    BinFile * fBinfile;
    Info_data * fInfoData;
    char tempchar;
    int  tempint;
    int  tempiMax;
    BinFile * p_fBinfile;
    Info_data * p_fInfoData;
    char * p_char;
    void Read_A_int(void);
    void Oh_found_it_is_a_flag_for_int_repeat(void);
    void Oh_found_it_is_a_flag_for_int_2_string(void);
    void Oh_found_it_is_an_normal_int(void);
    void Oh_found_it_is_only_a_step_repeat(void);
    void Read_A_char(void);
    void Oh_found_it_is_a_flag_for_string_repeat(void);
    void Oh_found_it_is_a_flag_for_string_2_int(void);
    void Oh_found_it_is_a_length_for_a_string(void);
    void Oh_found_it_is_only_a_string_repeat(void);
};
//--------------------------------------------------------------------------------------------
const int length_of_degree = 101;
char DegreeOfMachine1[length_of_degree];
char DegreeOfMachine2[length_of_degree];
void GetDegreeOfMachine(const char * fn, int n = 30000);
void PreDoForStringDegree(string & s);
void AfterDoForStringDegree(char * cstr);
}; // namespace RonForAli


#endif // _Ron_d10_c10_bin_file_h












