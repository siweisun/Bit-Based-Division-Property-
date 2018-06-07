/* This is the supporting material of the paper entitled */
/* "MILP-aided bit-based division property for primitives with non-bit-permutation linear layers". */
/* Please find the paper at https://eprint.iacr.org/2016/811.pdf. */

/* For more information, feedback or questions, please contact lingsun@mail.sdu.edu.cn. */
/* Implemented by Sun Ling, Key Laboratory of Cryptologic Technology and Information Security, */
/* Ministry of Education, Shandong University, Jinan 250100, China. */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <math.h>

using namespace std;

const int ROUNDS=4;
const int BlockSize = 64;

string Fout_i2s(int in)
{
    string re="result";
    if (in==0)
    {
        re+="0";
    }
    else
    {
        int len=0,temp=in;
        while(temp!=0)
        {
            temp/=10;
            len++;
        }
        temp=in;
        while(len--)
        {
            re+=(int)('0'+temp/pow(10,len));
            temp=temp%(int)(pow(10,len)+0.5);
        }
    }
    re += ".lp";
    return re;
}

int INE[10][9]={
    {1, 1, 1, 1, -1, -1, -1, -1, 0},
    {-2, -2, -2, -4, 1, 4, 1, -3, -7},
    {0, 0, 0, -2, -1, -1, -1, 2, -3},
    {-2, -1, -1, 0, 3, 3, 3, 2, 0},
    {1, 1, 1, 1, -2, -2, 1, -2, -1},
    {0, 0, 0, 1, 1, -1, -2, -1, -2},
    {0, -1, -1, -2, 1, 0, 1, -1, -3},
    {0, 0, 0, 0, -1, 1, -1, 1, -1},
    {0, -2, -2, 0, 1, -1, 1, 2, -3},
    {0, 0, 0, -1, 1, 1, 1, 1, 0}
};

int COLUMN[16][17]={
    {0,1,2,3,4,5,6,7,8,0,0,0,0,0,0,0,9},
    {9,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,7},
    {16,17,18,19,20,21,0,0,0,0,0,0,0,0,0,0,6},
    {22,23,24,25,26,27,0,0,0,0,0,0,0,0,0,0,6},
    {28,29,30,31,32,33,34,0,0,0,0,0,0,0,0,0,7},
    {35,36,37,38,39,40,41,42,0,0,0,0,0,0,0,0,8},
    {43,44,45,46,47,48,49,50,0,0,0,0,0,0,0,0,8},
    {51,52,53,54,55,56,57,0,0,0,0,0,0,0,0,0,7},
    {58,59,60,61,62,63,64,65,66,67,0,0,0,0,0,0,10},
    {68,69,70,71,72,73,74,75,76,0,0,0,0,0,0,0,9},
    {77,78,79,80,81,82,83,84,85,0,0,0,0,0,0,0,9},
    {86,87,88,89,90,91,92,93,94,0,0,0,0,0,0,0,9},
    {95,96,97,98,99,100,101,102,0,0,0,0,0,0,0,0,8},
    {103,104,105,106,107,108,109,0,0,0,0,0,0,0,0,0,7},
    {110,111,112,113,114,115,0,0,0,0,0,0,0,0,0,0,6},
    {116,117,118,119,120,121,122,123,0,0,0,0,0,0,0,0,8}
};

int ROW[16][17]={
    {16,28,68,103,0,0,0,0,0,0,0,0,0,0,0,0,4},
    {0,22,35,77,110,0,0,0,0,0,0,0,0,0,0,0,5},
    {1,9,43,58,86,95,116,0,0,0,0,0,0,0,0,0,7},
    {10,51,59,96,0,0,0,0,0,0,0,0,0,0,0,0,4},
    {2,23,36,44,60,78,104,111,0,0,0,0,0,0,0,0,8},
    {3,11,29,45,52,61,69,87,97,112,117,0,0,0,0,0,11},
    {12,17,37,53,62,70,79,105,118,0,0,0,0,0,0,0,9},
    {18,30,38,71,88,98,106,0,0,0,0,0,0,0,0,0,7},
    {13,24,31,39,46,54,63,72,89,119,0,0,0,0,0,0,10},
    {4,19,40,47,55,64,73,80,99,0,0,0,0,0,0,0,9},
    {5,14,25,48,56,65,74,81,90,107,0,0,0,0,0,0,10},
    {6,20,26,32,41,49,66,82,113,120,0,0,0,0,0,0,10},
    {15,42,75,83,91,108,121,0,0,0,0,0,0,0,0,0,7},
    {21,50,84,92,100,114,0,0,0,0,0,0,0,0,0,0,6},
    {7,27,33,57,93,101,109,122,0,0,0,0,0,0,0,0,8},
    {8,34,67,76,85,94,102,115,123,0,0,0,0,0,0,0,9}
};

void MC(int in[16], int out[16], int t[124], ofstream & fout)
{
    for (int i=0;i<16;i++)
    {
        fout<<"b"<<in[i];
        for (int j=0;j<(COLUMN[i][16]-1);j++)
        {
            fout<<" - t"<<t[COLUMN[i][j]];
        }
        fout<<" - t"<<t[COLUMN[i][COLUMN[i][16]-1]]<<" = 0"<<"\n";
    }

    for (int i=0;i<16;i++)
    {
        fout<<"t"<<t[ROW[i][0]];
        for (int j=1;j<ROW[i][16];j++)
        {
            fout<<" + t"<<t[ROW[i][j]];
        }
        fout<<" - a"<<out[i]<<" = 0"<<"\n";
    }
}

void ROUND_FUNCTION(int a[64], int b[64], int t[496], ofstream & fout)
{
    for (int j=0;j<16;j++)
    {
        for (int i=0;i<10;i++)//0
        {
            fout<<INE[i][0]<<" "<<"a"<<a[4*j+0]<<" + ";
            fout<<INE[i][1]<<" "<<"a"<<a[4*j+1]<<" + ";
            fout<<INE[i][2]<<" "<<"a"<<a[4*j+2]<<" + ";
            fout<<INE[i][3]<<" "<<"a"<<a[4*j+3]<<" + ";

            fout<<INE[i][4]<<" "<<"b"<<b[4*j+0]<<" + ";
            fout<<INE[i][5]<<" "<<"b"<<b[4*j+1]<<" + ";
            fout<<INE[i][6]<<" "<<"b"<<b[4*j+2]<<" + ";
            fout<<INE[i][7]<<" "<<"b"<<b[4*j+3];

            fout<<" >= "<<INE[i][8]<<"\n";
        }
    }

    int temp[64];
    for (int i=0;i<64;i++)
    {
        temp[i]=b[i];
    }

    int SR[16] = {0,5,10,15, 4,9,14,3, 8,13,2,7, 12,1,6,11};

    for (int i=0;i<16;i++)
    {
        for (int j=0;j<4;j++)
        {
            b[4*i+j] = temp[4*SR[i]+j];
        }
    }

    int tin[16];
    int tout[16];
    int inter[124];
    for (int col = 0;col<4;col++)
    {
        for (int i=0;i<16;i++)
        {
            tin[i] = b[16*col+i];
        }
        for (int i=0;i<16;i++)
        {
            tout[i] = a[16*col+i] + 64;
        }
        for (int i=0;i<124;i++)
        {
            inter[i] = t[124*col+i];
        }
        MC(tin, tout, inter,fout);
    }
}

int main()
{
    for (int file = 0;file<BlockSize;file++)
    {
        string outtem = Fout_i2s(file);
        const char* out_tem = outtem.c_str();
        ofstream fout(out_tem, ios::out);
        int a[64];
        int b[64];
        int t[496];
        int na=0;
        int nb=0;
        int nt=0;

        fout<<"Maximize"<<"\n";
        for (int i=64*ROUNDS;i<64*ROUNDS+63;i++)
        {
            fout<<"a"<<i<<" + ";
        }
        fout<<"a"<<(64*ROUNDS+63)<<"\n";
        fout<<"\n";

        fout<<"Subject to"<<"\n";

        for (int r=0;r<ROUNDS;r++)
        {
            for (int i=0;i<64;i++)
            {
                a[i]=na++;
            }
            for (int i=0;i<64;i++)
            {
                b[i]=nb++;
            }
            for (int i=0;i<496;i++)
            {
                t[i]=nt++;
            }
            ROUND_FUNCTION(a,b,t,fout);
        }

        //=============================Input Division Property

        for (int i=0;i<4;i++)
        {
            fout<<"a"<<i<<" = 1"<<"\n";
        }
        for (int i=4;i<20;i++)
        {
            fout<<"a"<<i<<" = 0"<<"\n";
        }
        for (int i=20;i<24;i++)
        {
            fout<<"a"<<i<<" = 1"<<"\n";
        }
        for (int i=24;i<40;i++)
        {
            fout<<"a"<<i<<" = 0"<<"\n";
        }
        for (int i=40;i<44;i++)
        {
            fout<<"a"<<i<<" = 1"<<"\n";
        }

        for (int i=44;i<BlockSize;i++)
        {
            fout<<"a"<<i<<" = 0"<<"\n";
        }
        //=============================

        //=============================Output Division Property
        for (int i=0; i<64;i++)
        {
            if (i!=file)
            {
                fout<<"a"<<BlockSize*ROUNDS+i<<" = 0"<<"\n";
            }
            else
            {
                fout<<"a"<<BlockSize*ROUNDS+i<<" = 1"<<"\n";
            }
        }
        //=============================

        fout<<"Binary"<<"\n";
        for (int i=0;i<(na+64);i++)
        {
            fout<<"a"<<i<<" ";
        }
        for (int i=0;i<nb;i++)
        {
            fout<<"b"<<i<<" ";
        }
        for (int i=0;i<nt;i++)
        {
            fout<<"t"<<i<<" ";
        }
        fout<<"\n";
        fout<<"End"<<"\n";
    
        fout.close();
    }
    return 0;
}
