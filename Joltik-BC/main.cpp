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
const int MCInter = 88;
const int TotalInter = MCInter * 4;
const int NumIne = 13;


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

int INE[NumIne][9]={
    {1, 1, 1, 1, -1, -1, -1, -1, 0},
    {-2, -1, -1, -3, 0, 2, 1, 2, -3},
    {-1, -1, 0, -1, 1, -1, 1, 0, -2},
    {0, 0, 0, 1, -1, 1, -1, -1, -1},
    {3, 0, 0, 0, -1, -1, -1, -2, -2},
    {-1, 0, -1, 2, 1, -2, -1, -1, -3},
    {-1, -1, 0, -2, 2, 1, 2, 3, 0},
    {0, 0, -1, 0, -1, 1, 0, 0, -1},
    {0, 1, 1, 0, -1, 0, -1, -1, -1},
    {1, 1, 0, 1, -1, -2, -2, 0, -2},
    {0, 0, 1, 2, -1, -1, -1, -1, -1},
    {-1, 0, -1, -1, 1, 1, 2, 2, 0},
    {1, 0, 0, 1, -1, -1, 0, -1, -1}
};

int COLUMN[16][17]={
    {0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0,5},
    {5,6,7,8,9,0,0,0,0,0,0,0,0,0,0,0,5},
    {10,11,12,13,14,0,0,0,0,0,0,0,0,0,0,0,5},
    {15,16,17,18,19,20,21,0,0,0,0,0,0,0,0,0,7},
    {22,23,24,25,26,0,0,0,0,0,0,0,0,0,0,0,5},
    {27,28,29,30,31,0,0,0,0,0,0,0,0,0,0,0,5},
    {32,33,34,35,36,0,0,0,0,0,0,0,0,0,0,0,5},
    {37,38,39,40,41,42,43,0,0,0,0,0,0,0,0,0,7},
    {44,45,46,47,48,0,0,0,0,0,0,0,0,0,0,0,5},
    {49,50,51,52,53,0,0,0,0,0,0,0,0,0,0,0,5},
    {54,55,56,57,58,0,0,0,0,0,0,0,0,0,0,0,5},
    {59,60,61,62,63,64,65,0,0,0,0,0,0,0,0,0,7},
    {66,67,68,69,70,0,0,0,0,0,0,0,0,0,0,0,5},
    {71,72,73,74,75,0,0,0,0,0,0,0,0,0,0,0,5},
    {76,77,78,79,80,0,0,0,0,0,0,0,0,0,0,0,5},
    {81,82,83,84,85,86,87,0,0,0,0,0,0,0,0,0,7}
};

int ROW[16][17]={
    {0,32,59,76,81,0,0,0,0,0,0,0,0,0,0,0,5},
    {5,22,37,44,82,0,0,0,0,0,0,0,0,0,0,0,5},
    {10,23,27,49,66,0,0,0,0,0,0,0,0,0,0,0,5},
    {15,28,54,60,71,77,83,0,0,0,0,0,0,0,0,0,7},
    {11,24,55,61,84,0,0,0,0,0,0,0,0,0,0,0,5},
    {1,16,29,62,67,0,0,0,0,0,0,0,0,0,0,0,5},
    {2,6,33,45,72,0,0,0,0,0,0,0,0,0,0,0,5},
    {7,38,50,56,63,78,85,0,0,0,0,0,0,0,0,0,7},
    {17,34,39,46,79,0,0,0,0,0,0,0,0,0,0,0,5},
    {3,40,51,68,86,0,0,0,0,0,0,0,0,0,0,0,5},
    {8,25,57,69,73,0,0,0,0,0,0,0,0,0,0,0,5},
    {12,18,30,35,41,64,74,0,0,0,0,0,0,0,0,0,7},
    {13,19,42,58,70,0,0,0,0,0,0,0,0,0,0,0,5},
    {20,26,47,65,75,0,0,0,0,0,0,0,0,0,0,0,5},
    {4,31,48,52,80,0,0,0,0,0,0,0,0,0,0,0,5},
    {9,14,21,36,43,53,87,0,0,0,0,0,0,0,0,0,7}
};

void MC(int in[16], int out[16], int t[MCInter], ofstream & fout)
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

void ROUND_FUNCTION(int a[64], int b[64], int t[TotalInter], ofstream & fout)
{
    for (int j=0;j<16;j++)
    {
        for (int i=0;i<NumIne;i++)
        {
            fout<<INE[i][0]<<" "<<"a"<<a[4*j]<<" + ";
            fout<<INE[i][1]<<" "<<"a"<<a[4*j+1]<<" + ";
            fout<<INE[i][2]<<" "<<"a"<<a[4*j+2]<<" + ";
            fout<<INE[i][3]<<" "<<"a"<<a[4*j+3]<<" + ";

            fout<<INE[i][4]<<" "<<"b"<<b[4*j]<<" + ";
            fout<<INE[i][5]<<" "<<"b"<<b[4*j+1]<<" + ";
            fout<<INE[i][6]<<" "<<"b"<<b[4*j+2]<<" + ";
            fout<<INE[i][7]<<" "<<"b"<<b[4*j+3];

            fout<<" >= "<<INE[i][8]<<"\r\n";
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
    int inter[MCInter];

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
        for (int i=0;i<MCInter;i++)
        {
            inter[i] = t[MCInter*col+i];
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
        int a[BlockSize];
        int b[BlockSize];
        int t[TotalInter];
        int na=0;
        int nb=0;
        int nt=0;

        fout<<"Maximize"<<"\n";
        for (int i=BlockSize*ROUNDS;i<BlockSize*(ROUNDS+1)-1;i++)
        {
            fout<<"a"<<i<<" + ";
        }
        fout<<"a"<<BlockSize*(ROUNDS+1)-1<<"\n";
        fout<<"\n";

        fout<<"Subject to"<<"\n";

        for (int r=0;r<ROUNDS;r++)
        {
            for (int i=0;i<BlockSize;i++)
            {
                a[i]=na++;
            }
            for (int i=0;i<BlockSize;i++)
            {
                b[i]=nb++;
            }
            for (int i=0;i<TotalInter;i++)
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
        for (int i=4;i<BlockSize;i++)
        {
            fout<<"a"<<i<<" = 0"<<"\n";
        }
        //=============================

        //=============================Output Division Property
        for (int i=0;i<BlockSize;i++)
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
        for (int i=0;i<(na+BlockSize);i++)
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














