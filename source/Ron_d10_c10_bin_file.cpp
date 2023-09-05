#ifndef _Ron_d10_c10_bin_file_cpp
#define _Ron_d10_c10_bin_file_cpp

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>

#include "Ron_d10_c10_bin_file.h"

using namespace std;

namespace RonForALi
{
//------------------------------------------------------------------------------------------
BinData::BinData()
{
    ;
}
BinData::BinData(int N)
{
    SetN(N);
}
BinData::~BinData()
{
    delete [] fChar;
}
void BinData::SetN(int N)
{
    fN = N;
    fNChar = (fN + 7) / 8;
    for(int i = 8 * fNChar; i >= fN; -- i)
        Write(i, 1);
}
void BinData::SetN0(int N)
{
    fN = N;
    fNChar = (fN + 7) / 8;
    fChar = new char [fNChar];
    for(int i = 8 * fNChar; i >= fN; -- i)
        Write(i, 1);
    m_N_first = fN;
}
void BinData::Write(int ibin, bool val)
{
    m_p_char = fChar + ibin / 8;
    if(val)
    {
//        (* m_p_char) = (* m_p_char) | ( BinData_NS::char1 << (7 - ibin % 8) );
        (* m_p_char) = (* m_p_char) | ( BinData_NS::char10000000 >> ibin % 8 );
    }
    else
    {
//        (* m_p_char) = (* m_p_char) & ( ~ ( BinData_NS::char1 << (7 - ibin % 8) ) );
        (* m_p_char) = (* m_p_char) & ( ~ ( BinData_NS::char10000000 >> ibin % 8 ) );
    }
}
bool BinData::Read(int ibin)
{
//    return ( fChar[ibin / 8] & ( BinData_NS::char1 << (7 - ibin % 8) ) );
    return ( fChar[ibin / 8] & ( BinData_NS::char10000000 >> ibin % 8 ) );
}
//------------------------------------------------------------------------------------------

bool BinFile::Open(const char* fn, const char* mode)
{
    if(string(mode) == string("recreate"))
    {
        fMode = 0;
        out.open(fn);
        return 1;
    }
    else if(string(mode) == string("read"))
    {
        fMode = 1;
        in.open(fn);
        if(!in)
        {
            cout << "open file failed!!!" << endl;
            return 0;
        }
        else
            return 1;
    }
    else
    {
        cout << "mode wrong!!!" << endl;
        return 0;
    }
}
void BinFile::Write_A_BinData(const BinData* bin_d)
{
    out.write(bin_d->fChar, bin_d->fNChar);
}
void BinFile::Write_A_IntData(const  int* int_d)
{
    out.write((char *) (int_d), 4);
}
void BinFile::Write_A_ShortData(const  short* short_d)
{
    out.write((char *) (short_d), 2);
}
void BinFile::Write_A_CharData(const  char* char_d)
{
    out.write(char_d, 1);
}
void BinFile::Read_A_BinDataAddress(BinData* bin_d)
{
    in.read(bin_d->fChar, bin_d->fNChar);
}
void BinFile::Read_A_IntDataAddress( int* int_d)
{
    in.read((char *) (int_d), 4);
}
void BinFile::Read_A_ShortDataAddress( short* short_d)
{
    in.read((char *) (short_d), 2);
}
void BinFile::Read_A_CharDataAddress( char* char_d)
{
    in.read(char_d, 1);
}
void BinFile::Close(void)
{
    if(fMode == 0)
        out.close();
    if(fMode == 1)
        in.close();
}
BinFile::BinFile()
{
    ;
}
BinFile::~BinFile()
{
    ;
}
//---------------------------------------------------------------------------------
bool BasePair::SetString(string s)
{
    if(strcmp(s.c_str(), fOldString) == 0)
    {
        flag_newstring_is_same_with_old_string = true;
        return 1;
    }
    else
    {
        flag_newstring_is_same_with_old_string = false;
        strcpy(fOldString, s.c_str());
    }

    int fN_c_in_s = s.length();
    SetN(fN_c_in_s * 2);
    char c;
    fNumChar_N = 0;
    for(int ic = 0, twoic = 0; ic < fN_c_in_s; ++ ic, twoic += 2)
    {
        c = s[ic];

        if(c < 'D')
        {
            if(c < 'B')   //A
            {
                Write(twoic, false);
                Write(twoic + 1, false);
            }
            else     // C
            {
                Write(twoic, false);
                Write(twoic + 1, true);
            }
        }
        else if(c < 'H')     // G
        {
            Write(twoic, true);
            Write(twoic + 1, false);
        }
        else     // T or N
        {
            Write(twoic, true);
            Write(twoic + 1, true);
            if(c < 'O')
            {
                BasePair_NS::char_N_no[fNumChar_N] = ic;
                fNumChar_N ++;
            }
        }
    } // ic
    if(fNumChar_N > 0)
    {
        fChar_N_No = BasePair_NS::char_N_no;
//        fChar_N_No = new int [fNumChar_N];
//        for(int i = 0; i < fNumChar_N; i ++)
//        {
//            fChar_N_No[i] = BasePair_NS::char_N_no[i];
//        }
        return false;
    }
    else
    {
        return true;
    }
}
BasePair::BasePair()
{
    fOldString[0] = '\0';
}
BasePair::~BasePair()
{
    if(fNumChar_N > 0)
    {
        delete [] fChar_N_No;
    }
}
//---------------------------------------------------------------------------------
void BasePair_2_String_without_N(BinData* a)
{
    BasePair_NS::c = BasePair_NS::BasePair_2_String_res;
    for(int i = 0; i < a->fN; i += 2)
    {
        if(a->Read(i))
        {
            if(a->Read(i+1))
                * BasePair_NS::c = 'T';
            else
                * BasePair_NS::c = 'G';
        }
        else
        {
            if(a->Read(i+1))
                * BasePair_NS::c = 'C';
            else
                * BasePair_NS::c= 'A';
        }
        BasePair_NS::c ++;
    } //i,ic
    * BasePair_NS::c = '\0';

}
void BasePair_2_String_with_N(BinData* a, int n, int* posi)
{
    BasePair_NS::c = BasePair_NS::BasePair_2_String_res;
    for(int i = 0; i < a->fN; i += 2)
    {
        if(a->Read(i))
        {
            if(a->Read(i+1))
                * BasePair_NS::c = 'T';
            else
                * BasePair_NS::c = 'G';
        }
        else
        {
            if(a->Read(i+1))
                * BasePair_NS::c = 'C';
            else
                * BasePair_NS::c= 'A';
        }
        BasePair_NS::c ++;
    } //i,ic
    * BasePair_NS::c = '\0';

    BasePair_NS::c = BasePair_NS::BasePair_2_String_res;
    for(int i = 0; i < n; ++ i)
    {
        BasePair_NS::c[posi[i]] = 'N';
    }
}
//------------------------------------------------------------------
short Degree::num1[200];
short Degree::num2[200];
short Degree::num3[200];
void Degree::SetString(string s)
{
    int n = s.length();
    char c;
    int n1 = 0, n2 = 0, n3 = 0;
    int i1= 0, i2 = 0, i3 = 0;
    for(int i = 0; i < n; ++ i)
    {
        c = s[i];
        if(c > '<')
//        if(c > '>')
        {
            num1[i1] = c - '<';
//            num1[i1] = c - '>';
        }
        else
        {
            num1[i1] = 0;
            if(c > '.')
//            if(c > '0')
            {
                num2[i2] = c - '.';
//                num2[i2] = c - '0';
            }
            else
            {
                num2[i2] = 0;
                num3[i3] = c - '"';
                i3 ++;
            }
            i2 ++;
        }
        i1 ++;
    }
    num1[i1] = 999;
    num2[i2] = 999;
    num3[i3] = 999;
    n1 = long_2_short(i1, num1);
    n2 = long_2_short(i2, num2);
    n3 = long_2_short(i3, num3);
    int n1_A_n2 = n1 + n2;
    int n1_A_n2_A_n3 = n1_A_n2 + n3;
    fN1 = n1;
    fN2 = n2;
    fN3 = n3;

    SetN(n1_A_n2_A_n3 * 4);
    short * val = num1;
    for(int i = 0, j = 0; j < n1; i += 4, ++ j)
    {
        Write(i,        (* val) >> 3);
        Write(i + 1,    ((* val) >> 2) % 2 );
        Write(i + 2,    ((* val) >> 1) % 2 );
        Write(i + 3,    (* val) % 2 );
        val ++;
    }
    val = num2;
    for(int i = n1 * 4, j = 0; j < n2; i += 4, ++ j)
    {
        Write(i,        (* val) >> 3);
        Write(i + 1,    ((* val) >> 2) % 2 );
        Write(i + 2,    ((* val) >> 1) % 2 );
        Write(i + 3,    (* val) % 2 );
        val ++;
    }
    val = num3;
    for(int i = n1_A_n2 * 4, j = 0; j < n3; i += 4, ++ j)
    {
        Write(i,        (* val) >> 3);
        Write(i + 1,    ((* val) >> 2) % 2 );
        Write(i + 2,    ((* val) >> 1) % 2 );
        Write(i + 3,    (* val) % 2 );
        val ++;
    }
}
int Degree::long_2_short(int n, short* a)
{
    int starti = 0;
    int endi;
    int j = 0;
    int endi_C_starti;
    for(int i = 1, i_C_1 = 0; i <= n; ++ i, ++ i_C_1)
    {
        if(a[i] != a[i_C_1])
        {
            endi = i_C_1;
            endi_C_starti = endi - starti;
            switch (endi_C_starti)
            {
            case 2:
                a[j] = a[i_C_1];
                j ++;
            case 1:
                a[j] = a[i_C_1];
                j ++;
            case 0:
                a[j] = a[i_C_1];
                j ++;
                endi_C_starti = 0;
                break;
            default:
                a[j] = a[i_C_1];
                j ++;
                a[j] = 15;
                j ++;
                if(endi_C_starti <= 15)
                    a[j] = endi_C_starti;
                else
                    a[j] = 15;
                j ++;
                endi_C_starti -= 15;
            }
            while(endi_C_starti > 0)
            {
                switch(endi_C_starti)
                {
                case 3:
                    a[j] = a[i_C_1];
                    j ++;
                case 2:
                    a[j] = a[i_C_1];
                    j ++;
                case 1:
                    a[j] = a[i_C_1];
                    j ++;
                    endi_C_starti = 0;
                    break;
                default:
                    a[j] = a[i_C_1];
                    j ++;
                    endi_C_starti --;
                    a[j] = 15;
                    j ++;
                    if(endi_C_starti <= 15)
                        a[j] = endi_C_starti;
                    else
                        a[j] = 15;
                    j ++;
                    endi_C_starti -= 15;
                }
            }
            starti = i;
        }
    }

    return j;
}
void Degree_2_String(BinData* a, int n1, int n2, int n3)
{
//    int added;
//    bool flag = 0;
    for(int i = 0, ic = 0; ic < n1; i += 4, ++ ic)
    {
        Degree_NS::num1[ic] = ( int(a->Read(i)) << 3 ) + ( int(a->Read(i + 1)) << 2 )
                              + ( int(a->Read(i + 2)) << 1 ) + int(a->Read(i + 3));
    } // 1
    for(int i = n1 * 4, ic = 0; ic < n2; i += 4, ++ ic)
    {
        Degree_NS::num2[ic] = ( int(a->Read(i)) << 3 ) + ( int(a->Read(i + 1)) << 2 )
                              + ( int(a->Read(i + 2)) << 1 ) + int(a->Read(i + 3));
    } // 2
    for(int i = (n1 + n2) * 4, ic = 0; ic < n3; i += 4, ++ ic)
    {
        Degree_NS::num3[ic] = ( int(a->Read(i)) << 3 ) + ( int(a->Read(i + 1)) << 2 )
                              + ( int(a->Read(i + 2)) << 1 ) + int(a->Read(i + 3));
    } // 3
    int m1 = Degree_NS::short_2_long(n1, Degree_NS::num1, Degree_NS::long_num1);
    int m2 = Degree_NS::short_2_long(n2, Degree_NS::num2, Degree_NS::long_num2);
    int m3 = Degree_NS::short_2_long(n3, Degree_NS::num3, Degree_NS::long_num3);
    if(false) cout << m2 << m3 << endl;

    Degree_NS::c = Degree_NS::Degree_2_String_Res;
    for(int i = 0, j = 0, k = 0; i < m1; ++ i)
    {
        if(Degree_NS::long_num1[i])
            *Degree_NS::c = '<' + Degree_NS::long_num1[i];
//            *Degree_NS::c = '>' + Degree_NS::long_num1[i];
        else if(Degree_NS::long_num2[j])
        {
            *Degree_NS::c = '.' + Degree_NS::long_num2[j];
//            *Degree_NS::c = '0' + Degree_NS::long_num2[j];
            j ++;
        }
        else
        {
            j ++;
            *Degree_NS::c = '"' + Degree_NS::long_num3[k];
            k ++;
        }
        Degree_NS::c ++;
    } // i
    * Degree_NS::c = '\0';
}
int Degree_NS::short_2_long(int na, short* a, short* b)
{
    Degree_NS::nb = 0;
//    Degree_NS::ib;
    for(int i = 0, j = 0; i < na; ++ i)
    {
        if(a[i] != 15)
        {
            b[j] = a[i];
            j ++;
            Degree_NS::nb ++;
        }
        else
        {
            i ++;
            Degree_NS::ib = a[i];
            Degree_NS::nb += Degree_NS::ib;
            for(int k = 0; k < ib; ++ k)
            {
                b[j] = b[j-1];
                j ++;
            }
        }
    } // i, j
    return Degree_NS::nb;
}
//------------------------------------------------------------------

bool Info_NS::DecomposeInfoString(string s)
{
    slength = s.length();
    if(slength == 0)
        return 0;//--------end
    for(i = 0; i < slength; ++ i)
    {
        s_char[i] = s[i];
//        if(s_char[i] == ' ')
//            s_char[i] = '`';
    }
    if(s_char[slength - 1] > '/' && s_char[slength - 1] < ':')
        s_char[slength] = '@';
    else
        s_char[slength] = '0';
    s_char[slength + 1] = '\0';
    slength ++;

    ipart = 0;
    if(s_char[0] > '/' && s_char[0] < ':')
        LastType = true;
    else
        LastType = false;
    starti = 0;
    for(ic = 1; ic < slength; ++ ic)
    {
        if(s_char[ic] > '/' && s_char[ic] < ':')
            ThisType = true;
        else
            ThisType = false;
        if(ThisType != LastType)
        {
            PartType[ipart] = LastType;
            endi = ic - 1;
            for(i = 0, j = starti; j <= endi; ++ i, ++ j)
            {
                StringPart[ipart][i] = s_char[j];
            }
            StringPart[ipart][i] = '\0';
            starti = ic;
            ipart ++;
            LastType = ThisType;
        } // if
    }// ic
    NPart = ipart;
    for(ipart = 0; ipart < NPart; ++ ipart)
    {
        if(PartType[ipart])
        {
            IntPart[ipart] = cstring_2_int(& StringPart[ipart][0]);
            if(IntPart[ipart] >= 0 && IntPart[ipart] <= 9)
            {
                PartType[ipart] = false;
            } // if 0->9 to be a string
        } // if int
    }
    return true;
}
int Info_NS::cstring_2_int(char * pc)
{
    cc = pc;
    res_a2i = *cc - '0';
    cc ++;
    while(*cc != '\0')
    {
        res_a2i *= 10;
        res_a2i += *cc - '0';
        cc ++;
    }
    return res_a2i;
}

Two_Info_data::Two_Info_data()
{
    fData1 = new Info_data;
    fData2 = new Info_data;
    Reset();
}
void Two_Info_data::Reset(void)
{
    strcpy(fData1->fString, "~!@#$%^&*()_+}{:?><`-=][';/.,]");
    strcpy(fData2->fString, "{:?><`-=][';/.,]~!@#$%^&*()_+}");
    fData1->fType = false;
    fData2->fType = true;
    fData1->fStep = 99999999;
    fData2->fStep = -99999999;
    fData1->fStepRepeatNum = 0;
    fData2->fStepRepeatNum = 0;
    fData1->fStringRepeatNum = 0;
    fData2->fStringRepeatNum = 0;
    fData1->fInt = 88888888;
    fData2->fInt = -88888888;
}
Two_Info_data::~Two_Info_data()
{
    delete fData1;
    delete fData2;
}
void Two_Info_data::Save_Data2_to_Data1(void)
{
    fDataSwap = fData1;
    fData1 = fData2;
    fData2 = fDataSwap;
}
void Two_Info_data::SetIPart(int ipart)
{
    fIPart = ipart;
    m_type = &(Info_NS::PartType[ipart]);
    m_int  = &(Info_NS::IntPart[ipart]);
    m_pc   = &(Info_NS::StringPart[ipart][0]);
}
void Two_Info_data::Fill(void)
{
    Save_Data2_to_Data1();
    fData2->fType = * m_type;
    fData2->fInt  = * m_int;
    strcpy(fData2->fString, m_pc);
    fData2->fStringLength = strlen(fData2->fString);
    if(fData2->fType == fData1->fType)
    {
        ChangeType_2_Int    = false;
        ChangeType_2_String = false;
        if(fData2->fType)   // int type
        {
            SameType_and_SameString = false;
            fData2->fStep = fData2->fInt - fData1->fInt;
            if(fData2->fStep == fData1->fStep)
            {
                SameType_and_SameStepInt = true;
                if(fData1->fStepRepeatNum == 0)
                    fData1->fStepRepeatNum = 1;
                fData2->fStepRepeatNum = fData1->fStepRepeatNum + 1;
            }
            else
            {
                SameType_and_SameStepInt = false;
                fData2->fStepRepeatNum = 0;
            }
        }
        else     // string type
        {
            SameType_and_SameStepInt = false;
            if(strcmp(fData2->fString, fData1->fString) == 0)
            {
                SameType_and_SameString = true;
                fData2->fStringRepeatNum = fData1->fStringRepeatNum + 1;
            }
            else
            {
                SameType_and_SameString = false;
                fData2->fStringRepeatNum = 0;
            }
        }
    } // if fData2->fType == fData1->fType
    else   // fData2->fType != fData1->fType
    {
        SameType_and_SameStepInt = false;
        SameType_and_SameString  = false;
        if(fData2->fType)   // int type
        {
            ChangeType_2_Int = true;
            ChangeType_2_String = false;
            fData2->fStepRepeatNum = 0;
        }
        else     // string type
        {
            ChangeType_2_Int = false;
            ChangeType_2_String = true;
            fData2->fStringRepeatNum = 0;
        }
    } // else fData2->fType != fData1->fType
    if(SameType_and_SameStepInt || SameType_and_SameString)
        ShouldOutput = false;
    else
        ShouldOutput = true;
}

All_Info_data::All_Info_data()
{
    fNLines = 0;
}
All_Info_data::~All_Info_data()
{
    for(int i = 0; i < fNPartMax; i ++)
        fBinfile[i].Close();
    delete [] fTwoInfoData;
    delete [] fBinfile;
}
void All_Info_data::SetDirName_NPartMax(const char * dirname, int npartmax)
{
    fNPartMax = npartmax;
//    string lsdir = string("ls ") + string(dirname)  + string(" > ./temp.useless");
    string lsdir = string("ls ") + string(dirname)  + string(" > /dev/null");
    int ReturnOfLS = system(lsdir.c_str());
    string mkdir;
    int ReturnOfMKDIR;
    if( ReturnOfLS != 0 )
    {
        mkdir = string("mkdir ") + string(dirname);
        ReturnOfMKDIR = system(mkdir.c_str());
        if(false) cout << ReturnOfMKDIR << endl;
    }
    fBinfile = new BinFile [npartmax];
    char filename[1000];
    char ctrtemp[100];
    for(int i = 0; i < npartmax; i ++)
    {
        strcpy(filename, "");
        strcat(filename, dirname);
        sprintf(ctrtemp, "/%d.bin", i);
        strcat(filename, ctrtemp);
        fBinfile[i].Open(filename, "recreate");
    }
    fNLines = 0;
    fTwoInfoData = new Two_Info_data [npartmax];
    for(int i = 0; i < fNPartMax; ++ i)
    {
        fTwoInfoData[i].SetIPart(i);
    }
}
void All_Info_data::Fill(string s)
{
    Info_NS::DecomposeInfoString(s);
//        cout << Info_NS::NPart << endl;
//        cout << Info_NS::StringPart[Info_NS::NPart - 1] << endl;
    for(int i = Info_NS::NPart; i < fNPartMax; ++ i)
    {
        Info_NS::StringPart[i][0] = Info_NS::flag_no_char;
        Info_NS::StringPart[i][1] = '\0';
        Info_NS::PartType[i] = false; // string
    }
    Info_NS::NPart = fNPartMax;
    for(int i = 0; i < fNPartMax; ++ i)
    {
        fTwoInfoData[i].Fill();
    }
    if(fNLines == 0)
    {
        for(int i = 0; i < fNPartMax; ++ i)
        {
            if(fTwoInfoData[i].fData2->fType)   // int
            {
                fBinfile[i].Write_A_CharData(& Info_NS::flag_c1);
            }
            else     // string
            {
                fBinfile[i].Write_A_CharData(& Info_NS::flag_c0);
            } //if
        }//i
    }
    else
    {
        Fill0();
    }
    fNLines ++;
}
void All_Info_data::Fill0(void)
{
    for(int i = 0; i < fNPartMax; ++ i)
    {
        if(fTwoInfoData[i].ShouldOutput)   // output
        {
            if(fTwoInfoData[i].fData1->fType)   // data1 int start
            {
                if(fTwoInfoData[i].fData1->fStepRepeatNum)
                {
                    fBinfile[i].Write_A_IntData(& Info_NS::flag_int_repeat);
                    fBinfile[i].Write_A_IntData(& fTwoInfoData[i].fData1->fStep);
                    fBinfile[i].Write_A_IntData(& fTwoInfoData[i].fData1->fStepRepeatNum);
                } // int -> repeat
                else   // int not repeat or leader
                    fBinfile[i].Write_A_IntData(& fTwoInfoData[i].fData1->fInt);
                if(fTwoInfoData[i].ChangeType_2_String)   // int -> string
                    fBinfile[i].Write_A_IntData(& Info_NS::flag_int_2_string);
            } // data1 int end
            else // data1 string start
            {
                fBinfile[i].Write_A_CharData(& fTwoInfoData[i].fData1->fStringLength);
                for(int j = 0; j < fTwoInfoData[i].fData1->fStringLength; ++ j)
                    fBinfile[i].Write_A_CharData(& fTwoInfoData[i].fData1->fString[j]);
                if(fTwoInfoData[i].fData1->fStringRepeatNum)   // string old repeat 0
                {
                    fBinfile[i].Write_A_CharData(& Info_NS::flag_string_repeat);
                    fBinfile[i].Write_A_IntData(& fTwoInfoData[i].fData1->fStringRepeatNum);
                } // string old repeat > 0
                if(fTwoInfoData[i].ChangeType_2_Int) // string -> int
                    fBinfile[i].Write_A_CharData(& Info_NS::flag_string_2_int);
            } // data1 string end
        } // should output end
    } // i end
}
void All_Info_data::Close(void)
{
    for(int i = 0; i < fNPartMax; ++ i)
        if(fTwoInfoData[i].fData2->fType)   // data1 int start
        {
            if(fTwoInfoData[i].fData2->fStepRepeatNum)
            {
                fBinfile[i].Write_A_IntData(& Info_NS::flag_int_repeat);
                fBinfile[i].Write_A_IntData(& fTwoInfoData[i].fData2->fStep);
                fBinfile[i].Write_A_IntData(& fTwoInfoData[i].fData2->fStepRepeatNum);
            } // int -> repeat
            else   // int not repeat or leader
                fBinfile[i].Write_A_IntData(& fTwoInfoData[i].fData2->fInt);
        } // data1 int end
        else // data1 string start
        {
            fBinfile[i].Write_A_CharData(& fTwoInfoData[i].fData2->fStringLength);
            for(int j = 0; j < fTwoInfoData[i].fData2->fStringLength; ++ j)
                fBinfile[i].Write_A_CharData(& fTwoInfoData[i].fData2->fString[j]);
            if(fTwoInfoData[i].fData2->fStringRepeatNum)   // string old repeat 0
            {
                fBinfile[i].Write_A_CharData(& Info_NS::flag_string_repeat);
                fBinfile[i].Write_A_IntData(& fTwoInfoData[i].fData2->fStringRepeatNum);
            } // string old repeat > 0
        } // data1 string end
    for(int i = 0; i < fNPartMax; ++ i)
        fBinfile[i].Close();
}
void Read_All_Info_data::Open(const char* dirname, int npartmax)
{
    string s_dir = string(dirname);
    string fn;
    char name[100];
    fNPartMax = npartmax;
    fBinfile = new BinFile [fNPartMax];
    fInfoData = new Info_data [fNPartMax];
    for(int i = 0; i < npartmax; ++ i)
    {
        sprintf(name, "/%d.bin", i);
        fn = s_dir + string(name);
        fBinfile[i].Open(fn.c_str(), "read");
        fBinfile[i].Read_A_CharDataAddress(& tempchar);
        fInfoData[i].fType = int(tempchar);
        fInfoData[i].fStepRepeatNum = 0;
        fInfoData[i].fStringRepeatNum = 0;
    }
}
void Read_All_Info_data::Read(void)
{
    strcpy(fRes, "");
    for(int i = 0; i < fNPartMax; ++ i)
    {
        ReadFile(i);
        strcat(fRes, & ( fResFile[i][0] ) );
    }
    int length = strlen(fRes);
    for(int i = 0; i < length; ++ i)
    {
        if(fRes[i] == Info_NS::flag_no_char)
        {
            fRes[i] = '\0';
            break;
        }
//        else if(fRes[i] == '`')
//        {
//            fRes[i] = ' ';
//        }
    }
}
void Read_All_Info_data::ReadFile(int i)
{
    p_fBinfile  = & (fBinfile[i]);
    p_fInfoData = & (fInfoData[i]);
    p_char      = & (fResFile[i][0]);
    if(p_fInfoData->fType) // int
    {
        if(p_fInfoData->fStepRepeatNum)
            Oh_found_it_is_only_a_step_repeat();
        else
            Read_A_int();
    }
    else // string
    {
        if(p_fInfoData->fStringRepeatNum)
            Oh_found_it_is_only_a_string_repeat();
        else
            Read_A_char();
    }
}
void Read_All_Info_data::Read_A_int(void)
{
//    cout << "Read_A_int" << endl;
    p_fBinfile->Read_A_IntDataAddress(&tempint);
    if(tempint == Info_NS::flag_int_repeat)
        Oh_found_it_is_a_flag_for_int_repeat();
    else if(tempint == Info_NS::flag_int_2_string)
        Oh_found_it_is_a_flag_for_int_2_string();
    else
        Oh_found_it_is_an_normal_int();
}
void Read_All_Info_data::Read_A_char(void)
{
//    cout << "Read_A_char" << endl;
    p_fBinfile->Read_A_CharDataAddress(& tempchar);
    if(tempchar == Info_NS::flag_string_repeat)
        Oh_found_it_is_a_flag_for_string_repeat();
    else if(tempchar == Info_NS::flag_string_2_int)
        Oh_found_it_is_a_flag_for_string_2_int();
    else
        Oh_found_it_is_a_length_for_a_string();
}
void Read_All_Info_data::Oh_found_it_is_a_flag_for_int_repeat(void)
{
//    cout << "Oh_found_it_is_a_flag_for_int_repeat" << endl;
    p_fBinfile->Read_A_IntDataAddress(& (p_fInfoData->fStep) );
    p_fBinfile->Read_A_IntDataAddress(& (p_fInfoData->fStepRepeatNum) );
    Oh_found_it_is_only_a_step_repeat();
}
void Read_All_Info_data::Oh_found_it_is_a_flag_for_int_2_string(void)
{
    p_fInfoData->fType = false;
    Read_A_char();
}
void Read_All_Info_data::Oh_found_it_is_an_normal_int(void)
{
//    cout << "Oh_found_it_is_an_normal_int" <<endl;
    sprintf(p_char, "%d", tempint);
    p_fInfoData->fInt = tempint;
}
void Read_All_Info_data::Oh_found_it_is_a_flag_for_string_repeat(void)
{
    p_fBinfile->Read_A_IntDataAddress( &(p_fInfoData->fStringRepeatNum) );
    Oh_found_it_is_only_a_string_repeat();
}
void Read_All_Info_data::Oh_found_it_is_a_flag_for_string_2_int(void)
{
    p_fInfoData->fType = true;
    Read_A_int();
}
void Read_All_Info_data::Oh_found_it_is_a_length_for_a_string(void)
{
    tempiMax = tempchar;
//    cout << tempiMax << endl;
    for(int i = 0; i < tempiMax; ++ i)
    {
        p_fBinfile->Read_A_CharDataAddress(p_char + i);
    }
    p_char[tempiMax] = '\0';
    strcpy(p_fInfoData->fString, p_char);
}
void Read_All_Info_data::Oh_found_it_is_only_a_step_repeat(void)
{
//    cout << "Oh_found_it_is_only_a_step_repeat" << endl;
    p_fInfoData->fInt += p_fInfoData->fStep;
    sprintf(p_char, "%d", p_fInfoData->fInt);
    p_fInfoData->fStepRepeatNum --;
}
void Read_All_Info_data::Oh_found_it_is_only_a_string_repeat(void)
{
//    cout << "Oh_found_it_is_only_a_string_repeat" << endl;
    p_fInfoData->fStringRepeatNum --;
}
void Read_All_Info_data::Close(void)
{
    for(int i = 0; i < fNPartMax; i ++)
    {
        fBinfile[i].Close();
    }
    delete [] fBinfile;
    delete [] fInfoData;
}
//------------------------------------------------------------------
void WriteBasePair_2_BinFile::Open(const char* dir_name, int NPairMax)
{
    string s1 = string("ls ") + string(dir_name) + string(" > /dev/null");
    int ntemp = int(system(s1.c_str()));
    if(ntemp != 0)
    {
        s1 = string("mkdir ") + string(dir_name);
        ntemp = int(system(s1.c_str()));
        if(false) cout << ntemp << endl;
    }
    fBasePair.SetN0(NPairMax);
    s1 = string(dir_name) + string("/bp_0.bin");
    fBF_4_BP.Open(s1.c_str(), "recreate");
    s1 = string(dir_name) + string("/bp_1.bin");
    fBF_4_Info_pair.Open(s1.c_str(), "recreate");
    s1 = string(dir_name) + string("/bp_2.bin");
    fBF_4_Info_N.Open(s1.c_str(), "recreate");
    s1 = string(dir_name) + string("/bp_3.bin");
    fBF_4_N.Open(s1.c_str(), "recreate");
    s1 = string(dir_name) + string("/bp_4.bin");
    fBF_4_repeat.Open(s1.c_str(), "recreate");
    fN_BasePair = 0;
    fN_N = 0;
    fBasePair_repeat_num = 1;
    fN_repeat_num = 1;
    fNLines = 0;
    fCharForBasePairRep = 0;
}
void WriteBasePair_2_BinFile::Write(string s)
{
    fBasePair.SetString(s);
    fNLines_L_8 = fNLines % 8;
    if(fBasePair.flag_newstring_is_same_with_old_string)
    {
        fCharForBasePairRep = (fCharForBasePairRep | BinData_NS::fChardifferent1[fNLines_L_8]);
    }
    else
    {
        fBF_4_BP.Write_A_BinData(& fBasePair);// write the base-pair main data
    }
    if(fNLines_L_8 == 7)
    {
        fBF_4_repeat.Write_A_CharData(& fCharForBasePairRep);
        fCharForBasePairRep = 0;
    }
    if(fNLines == 0)
    {
        fBasePair_repeat_num = 1;
        fN_BasePair = fBasePair.fN;
    }
    else if(fBasePair.fN == fN_BasePair)
    {
        fBasePair_repeat_num ++;
    }
    else
    {
        fBF_4_Info_pair.Write_A_IntData(& fBasePair_repeat_num);
        fBF_4_Info_pair.Write_A_IntData(& fN_BasePair);
        fN_BasePair = fBasePair.fN;
        fBasePair_repeat_num = 1;
    } // write the base-pair info
    if(fNLines == 0)
    {
        fN_repeat_num = 1;
        fN_N = fBasePair.fNumChar_N;
    }
    else if(fBasePair.fNumChar_N == fN_N)
    {
        fN_repeat_num ++;
    }
    else
    {
        fBF_4_Info_N.Write_A_IntData(& fN_repeat_num);
        fBF_4_Info_N.Write_A_IntData(& fN_N);
        fN_N = fBasePair.fNumChar_N;
        fN_repeat_num = 1;
    }// write the N info
    if(fBasePair.fNumChar_N)
    {
        for(int i = 0; i < fBasePair.fNumChar_N; ++ i)
        {
            fBF_4_N.Write_A_IntData(fBasePair.fChar_N_No + i);
        }
    } // write the N data
    fNLines ++;
}
void WriteBasePair_2_BinFile::Close(void)
{
    fBF_4_Info_pair.Write_A_IntData(& fBasePair_repeat_num);
    fBF_4_Info_pair.Write_A_IntData(& fN_BasePair);
    fBF_4_Info_N.Write_A_IntData(& fN_repeat_num);
    fBF_4_Info_N.Write_A_IntData(& fN_N);
    fBF_4_repeat.Write_A_CharData(& fCharForBasePairRep);
    fBF_4_BP.Close();
    fBF_4_Info_pair.Close();
    fBF_4_Info_N.Close();
    fBF_4_N.Close();
    fBF_4_repeat.Close();
}
void ReadBasePair_in_BinFile::Open(const char* dir_name, int NPairMax)
{
    string s1;
    fBasePair.SetN0(NPairMax);
    s1 = string(dir_name) + string("/bp_0.bin");
    fBF_4_BP.Open(s1.c_str(), "read");
    s1 = string(dir_name) + string("/bp_1.bin");
    fBF_4_Info_pair.Open(s1.c_str(), "read");
    s1 = string(dir_name) + string("/bp_2.bin");
    fBF_4_Info_N.Open(s1.c_str(), "read");
    s1 = string(dir_name) + string("/bp_3.bin");
    fBF_4_N.Open(s1.c_str(), "read");
    s1 = string(dir_name) + string("/bp_4.bin");
    fBF_4_repeat.Open(s1.c_str(), "read");
    fN_BasePair = 0;
    fN_N = 0;
    fBasePair_repeat_num = 0;
    fN_repeat_num = 0;
    fNLines = 0;
}
void ReadBasePair_in_BinFile::Read(void)
{
    if(fBasePair_repeat_num == 0)
    {
        fBF_4_Info_pair.Read_A_IntDataAddress(& fBasePair_repeat_num);
        fBF_4_Info_pair.Read_A_IntDataAddress(& fN_BasePair);
    }//else fN_BasePair is still the old value
    fBasePair_repeat_num --;
    fBasePair.SetN(fN_BasePair); // have got the base pair number;
    fNLines_L_8 = fNLines % 8;
    if(!fNLines_L_8)
    {
        fBF_4_repeat.Read_A_CharDataAddress(& fCharForBasePairRep);
    }
    if( (fCharForBasePairRep & BinData_NS::fChardifferent1[fNLines_L_8]) == 0 )
    {
        fBF_4_BP.Read_A_BinDataAddress(& fBasePair); // read the main data of base pair;
    } // else: repeat, it should not read;
    if(fN_repeat_num == 0)
    {
        fBF_4_Info_N.Read_A_IntDataAddress(& fN_repeat_num);
        fBF_4_Info_N.Read_A_IntDataAddress(& fN_N);
    }
    fN_repeat_num --; // have got the 'N' number;
    if(fN_N == 0)
        BasePair_2_String_without_N(& fBasePair); // transform the main data to a string without N
    else // read the 'N' position information
    {
        for(int i = 0; i < fN_N; ++ i)
            fBF_4_N.Read_A_IntDataAddress(fNPosi + i);
        BasePair_2_String_with_N(& fBasePair, fN_N, fNPosi);//transform the main data to a string with N
    }
    fNLines ++;
}
void ReadBasePair_in_BinFile::Close(void)
{
    fBF_4_BP.Close();
    fBF_4_Info_pair.Close();
    fBF_4_Info_N.Close();
    fBF_4_N.Close();
    fBF_4_repeat.Close();
}
//------------------------------------------------------------------

void WriteDegree_2_BinFile::Open(const char * dir_name, int NCharMax)
{
    string s1 = string("ls ") + string(dir_name) + string(" > /dev/null");
    int ntemp = int(system(s1.c_str()));
    if(ntemp != 0)
    {
        s1 = string("mkdir ") + string(dir_name);
        ntemp = int(system(s1.c_str()));
        if(false) cout << ntemp << endl;
    }

    fDegree.SetN0(NCharMax * 12);
    s1 = string(dir_name) + string("/dg_0.bin");
    fBF_4_DG.Open(s1.c_str(), "recreate");
    s1 = string(dir_name) + string("/dg_1.bin");
    fBF_4_3Char.Open(s1.c_str(), "recreate");
    fNLines = 0;
}
void WriteDegree_2_BinFile::Write(string s)
{
    fDegree.SetString(s);
    fBF_4_DG.Write_A_BinData(& fDegree);
    fBF_4_3Char.Write_A_CharData(& fDegree.fN1);
    fBF_4_3Char.Write_A_CharData(& fDegree.fN2);
    fBF_4_3Char.Write_A_CharData(& fDegree.fN3);
    fNLines ++;
}
void WriteDegree_2_BinFile::Close(void)
{
    fBF_4_DG.Close();
    fBF_4_3Char.Close();
}

//------------------------------------------------------------------

void ReadDegree_in_BinFile::Open(const char* dir_name, int NCharMax)
{
    string s1;
    fDegree.SetN0(NCharMax * 12);
    s1 = string(dir_name) + string("/dg_0.bin");
    fBF_4_DG.Open(s1.c_str(), "read");
    s1 = string(dir_name) + string("/dg_1.bin");
    fBF_4_3Char.Open(s1.c_str(), "read");
    fNLines = 0;
}
void ReadDegree_in_BinFile::Read(void)
{
    fBF_4_3Char.Read_A_CharDataAddress(& fN1);
    fBF_4_3Char.Read_A_CharDataAddress(& fN2);
    fBF_4_3Char.Read_A_CharDataAddress(& fN3);
    fN_for_fDegree = int(fN1) + int(fN2) + int(fN3);
    fDegree.SetN(fN_for_fDegree * 4);
    fBF_4_DG.Read_A_BinDataAddress(& fDegree);
    Degree_2_String(& fDegree, fN1, fN2, fN3);
    fNLines ++;
}
void ReadDegree_in_BinFile::Close(void)
{
    fBF_4_DG.Close();
    fBF_4_3Char.Close();
}
void GetDegreeOfMachine(const char* fn, int n)
{
    int ** hist = new int * [length_of_degree];
    for(int i = 0; i < length_of_degree; i ++)
        hist[i] = new int [40];
    for(int i = 0; i < length_of_degree; ++ i)
        for(int j = 0; j < 40; j ++)
            hist[i][j] = 0;
    string s1, s2, s3, s4;
    ifstream in(fn);
    if(!in)
    {
        cout << "In GetDegreeOfMachine(), open file failed!!!" << endl;
        exit(1);
    }
    for(int i = 0; i < n; ++ i)
    {
        if(in.eof()) break;
        getline(in, s1);
        getline(in, s2);
        getline(in, s3);
        getline(in, s4);
        for(int j = 0; j < length_of_degree; ++ j)
        {
            hist[j][s4[j] - '#'] ++;
        }
    }
    in.close();
    int max;
    for(int i = 0; i < length_of_degree; ++ i)
    {
        max = 0;
        for(int j = 0; j < 40; j ++)
        {
            if(hist[i][j] > max)
            {
                DegreeOfMachine1[i] = j;
                max = hist[i][j];
            }
        }
    }
    for(int i = 0; i < length_of_degree; ++ i)
    {
        max = 0;
        for(int j = 0; j < DegreeOfMachine1[i]; j ++)
        {
            if(hist[i][j] > max)
            {
                DegreeOfMachine2[i] = j;
                max = hist[i][j];
            }
        }
        for(int j = DegreeOfMachine1[i] + 1; j < 40; j ++)
        {
            if(hist[i][j] > max)
            {
                DegreeOfMachine2[i] = j;
                max = hist[i][j];
            }
        }
    }
    for(int i = 0; i < length_of_degree; ++ i)
    {
        DegreeOfMachine1[i] += '#';
        DegreeOfMachine2[i] += '#';
    }
    for(int i = 0; i < length_of_degree; i ++)
        delete [] hist[i];
    delete [] hist;
}
void PreDoForStringDegree(string& s)
{
    for(int i = 0; i < length_of_degree; ++ i)
    {
        if(s[i] == DegreeOfMachine1[i])
            s[i] = 'L';
        else if(s[i] == DegreeOfMachine2[i])
            s[i] = 'K';
    }
}
void AfterDoForStringDegree(char* cstr)
{
    for(int i = 0; i < length_of_degree; ++ i)
    {
        if(cstr[i] == 'L')
            cstr[i] = DegreeOfMachine1[i];
        else if(cstr[i] == 'K')
            cstr[i] = DegreeOfMachine2[i];
    }
}
//------------------------------------------------------------------
}; // namespace RonForAli
#endif // _Ron_d10_c10_bin_file_cpp































