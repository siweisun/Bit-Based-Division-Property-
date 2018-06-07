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

const int ROUNDS=1;
const int BlockSize = 100;
const int MCInter = 194;
const int ColNum = 5;
const int ColSize = 20; //BlockSize = ColNum * ColSize
//=======================
const int TotalInter = MCInter * ColNum;
const int NumIne = 10;

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

int COLUMN[ColSize][ColSize+1]={{0,1,2,3,4,5,6,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
    {7,8,9,10,11,12,13,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
    {14,15,16,17,18,19,20,21,22,0,0,0,0,0,0,0,0,0,0,0,9},
    {23,24,25,26,27,28,29,30,31,0,0,0,0,0,0,0,0,0,0,0,9},
    {32,33,34,35,36,37,38,39,40,41,42,0,0,0,0,0,0,0,0,0,11},
    {43,44,45,46,47,48,49,50,51,52,53,0,0,0,0,0,0,0,0,0,11},
    {54,55,56,57,58,59,60,61,62,63,64,65,0,0,0,0,0,0,0,0,12},
    {66,67,68,69,70,71,72,73,74,75,76,77,78,0,0,0,0,0,0,0,13},
    {79,80,81,82,83,84,85,86,87,88,89,90,0,0,0,0,0,0,0,0,12},
    {91,92,93,94,95,96,97,98,99,0,0,0,0,0,0,0,0,0,0,0,9},
    {100,101,102,103,104,105,106,107,108,0,0,0,0,0,0,0,0,0,0,0,9},
    {109,110,111,112,113,114,115,116,0,0,0,0,0,0,0,0,0,0,0,0,8},
    {117,118,119,120,121,122,123,124,125,126,127,0,0,0,0,0,0,0,0,0,11},
    {128,129,130,131,132,133,134,135,136,137,0,0,0,0,0,0,0,0,0,0,10},
    {138,139,140,141,142,143,144,145,146,147,0,0,0,0,0,0,0,0,0,0,10},
    {148,149,150,151,152,153,154,155,156,0,0,0,0,0,0,0,0,0,0,0,9},
    {157,158,159,160,161,162,163,164,0,0,0,0,0,0,0,0,0,0,0,0,8},
    {165,166,167,168,169,170,171,172,0,0,0,0,0,0,0,0,0,0,0,0,8},
    {173,174,175,176,177,178,179,180,181,182,183,0,0,0,0,0,0,0,0,0,11},
    {184,185,186,187,188,189,190,191,192,193,0,0,0,0,0,0,0,0,0,0,10}
};

int ROW[ColSize][ColSize+1]={{0,43,109,148,165,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
    {7,54,79,117,173,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
    {14,32,66,91,128,157,184,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
    {23,33,100,110,138,149,158,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
    {8,34,55,80,92,118,150,174,185,0,0,0,0,0,0,0,0,0,0,0,9},
    {15,35,44,67,93,101,119,129,186,0,0,0,0,0,0,0,0,0,0,0,9},
    {1,24,36,45,56,81,102,111,130,139,159,0,0,0,0,0,0,0,0,0,11},
    {2,46,68,82,112,140,166,175,187,0,0,0,0,0,0,0,0,0,0,0,9},
    {16,25,47,69,83,94,113,120,141,151,160,0,0,0,0,0,0,0,0,0,11},
    {26,37,57,84,95,103,131,152,167,0,0,0,0,0,0,0,0,0,0,0,9},
    {3,38,48,70,85,96,104,114,121,142,176,0,0,0,0,0,0,0,0,0,11},
    {9,17,27,39,58,71,86,105,132,143,188,0,0,0,0,0,0,0,0,0,11},
    {4,49,59,72,97,122,133,161,168,177,189,0,0,0,0,0,0,0,0,0,11},
    {10,60,73,106,134,144,169,178,190,0,0,0,0,0,0,0,0,0,0,0,9},
    {18,74,87,115,123,145,153,179,191,0,0,0,0,0,0,0,0,0,0,0,9},
    {28,40,50,61,75,88,124,154,162,170,180,0,0,0,0,0,0,0,0,0,11},
    {5,11,19,29,41,51,62,76,89,116,125,146,163,181,192,0,0,0,0,0,15},
    {12,20,30,52,63,77,90,98,126,135,155,171,193,0,0,0,0,0,0,0,13},
    {21,31,64,78,99,107,127,136,147,164,182,0,0,0,0,0,0,0,0,0,11},
    {6,13,22,42,53,65,108,137,156,172,183,0,0,0,0,0,0,0,0,0,11}
};


void MC(int in[ColSize], int out[ColSize], int t[MCInter], ofstream & fout)
{
	for (int i=0;i<ColSize;i++)
	{
		fout<<"b"<<in[i];
        for (int j=0;j<(COLUMN[i][ColSize]-1);j++)
        {
        	fout<<" - t"<<t[COLUMN[i][j]];
        }
        fout<<" - t"<<t[COLUMN[i][COLUMN[i][ColSize]-1]]<<" = 0"<<"\n";
	}

	for (int i=0;i<ColSize;i++)
	{
		fout<<"t"<<t[ROW[i][0]];
		for (int j=1;j<ROW[i][ColSize];j++)
		{
			fout<<" + t"<<t[ROW[i][j]];
		}
		fout<<" - a"<<out[i]<<" = 0"<<"\n";
	}
}

void ROUND_FUNCTION(int a[BlockSize], int b[BlockSize], int t[TotalInter], ofstream & fout)
{
	for (int j=0;j<ColNum*ColNum;j++)
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

            fout<<" >= "<<INE[i][8]<<"\n";
		}
	}

	int temp[BlockSize];
	for (int i=0;i<BlockSize;i++)
	{
		temp[i] = b[i];
	}

	int SR[ColNum][ColNum];
	for (int j=0;j<ColNum;j++)
	{
        for (int i=0;i<ColNum;i++)
        {
        	SR[i][j] = 5*j+i;
        }
	}

	int SR_Copy[ColNum][ColNum];
	for (int i=0;i<ColNum;i++)
	{
		for (int j=0;j<ColNum-i;j++)
		{
			SR_Copy[i][j] = SR[i][j+i];
		}
		for (int j=0;j<i;j++)
		{
			SR_Copy[i][j+ColNum-i] = SR[i][j];
		}
	}

	int SR_Column[ColNum*ColNum];
	for (int j=0;j<ColNum;j++)
	{
		for (int i=0;i<ColNum;i++)
		{
			SR_Column[5*j+i] = SR_Copy[i][j];
		}
	}

	for (int i=0;i<ColNum*ColNum;i++)
	{
		for (int j=0;j<4;j++)
		{
			b[4*i+j] = temp[4*SR_Column[i]+j];
		}
	}

	int tin[ColSize];
	int tout[ColSize];
	int inter[MCInter];

	for (int col=0;col<ColNum;col++)
	{
		for (int i=0;i<ColSize;i++)
		{
			tin[i] = b[ColSize*col+i];
		}
		for (int i=0;i<ColSize;i++)
		{
			tout[i] = a[ColSize*col+i] + BlockSize;
		}
		for (int i=0;i<MCInter;i++)
		{
			inter[i] = t[MCInter*col+i];
		}
		MC(tin, tout, inter, fout);
	}

	/*for (int i=0;i<ColNum;i++)
	{
		for (int j=0;j<ColNum;j++)
		{
			cout<<SR[i][j]<<",";
		}
		cout<<endl;
	}
	cout<<"-----------------"<<endl;
	for (int i=0;i<ColNum;i++)
	{
		for (int j=0;j<ColNum;j++)
		{
			cout<<SR_Copy[i][j]<<",";
		}
		cout<<endl;
	}
	cout<<"-----------------"<<endl;
	for (int i=0;i<ColNum*ColNum;i++)
	{
		cout<<SR_Column[i]<<",";
	}
	cout<<endl;
	getchar();
	getchar();*/

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
        //========================================
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









