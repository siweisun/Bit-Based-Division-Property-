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
const int BlockSize = 144;
const int MCInter = 269;
const int ColNum = 6;
const int ColSize = 24; //BlockSize = ColNum * ColSize
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

int COLUMN[ColSize][ColSize+1]={
	{0,1,2,3,4,5,6,7,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,10,11,12,13,14,15,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
    {17,18,19,20,21,22,23,24,25,26,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
    {27,28,29,30,31,32,33,34,35,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {36,37,38,39,40,41,42,43,44,45,46,47,0,0,0,0,0,0,0,0,0,0,0,0,12},
    {48,49,50,51,52,53,54,55,56,57,58,0,0,0,0,0,0,0,0,0,0,0,0,0,11},
    {59,60,61,62,63,64,65,66,67,68,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
    {69,70,71,72,73,74,75,76,77,78,79,80,0,0,0,0,0,0,0,0,0,0,0,0,12},
    {81,82,83,84,85,86,87,88,89,90,91,0,0,0,0,0,0,0,0,0,0,0,0,0,11},
    {92,93,94,95,96,97,98,99,100,101,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
    {102,103,104,105,106,107,108,109,110,111,112,113,0,0,0,0,0,0,0,0,0,0,0,0,12},
    {114,115,116,117,118,119,120,121,122,123,124,125,126,127,0,0,0,0,0,0,0,0,0,0,14},
    {128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,0,0,0,0,0,0,0,0,0,15},
    {143,144,145,146,147,148,149,150,151,152,153,154,155,0,0,0,0,0,0,0,0,0,0,0,13},
    {156,157,158,159,160,161,162,163,164,165,166,167,168,0,0,0,0,0,0,0,0,0,0,0,13},
    {169,170,171,172,173,174,175,176,177,178,179,0,0,0,0,0,0,0,0,0,0,0,0,0,11},
    {180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,0,0,0,0,0,0,0,0,0,15},
    {195,196,197,198,199,200,201,202,203,204,205,206,207,0,0,0,0,0,0,0,0,0,0,0,13},
    {208,209,210,211,212,213,214,215,216,217,218,219,0,0,0,0,0,0,0,0,0,0,0,0,12},
    {220,221,222,223,224,225,226,227,228,229,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
    {230,231,232,233,234,235,236,237,238,239,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
    {240,241,242,243,244,245,246,247,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {249,250,251,252,253,254,255,256,257,258,259,0,0,0,0,0,0,0,0,0,0,0,0,0,11},
    {260,261,262,263,264,265,266,267,268,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9}
};

int ROW[ColSize][ColSize+1]={
	{0,48,81,114,128,156,180,220,240,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,59,82,92,129,143,169,181,195,249,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
    {17,36,69,93,102,130,144,157,196,208,230,260,0,0,0,0,0,0,0,0,0,0,0,0,12},
    {27,37,103,145,170,209,231,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
    {10,38,60,83,146,197,210,232,250,261,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
    {18,39,49,70,94,158,182,211,221,241,262,0,0,0,0,0,0,0,0,0,0,0,0,0,11},
    {1,28,40,50,61,104,131,171,198,222,233,251,0,0,0,0,0,0,0,0,0,0,0,0,12},
    {2,51,71,115,132,183,199,242,252,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {3,19,29,72,95,105,116,133,172,184,223,253,263,0,0,0,0,0,0,0,0,0,0,0,13},
    {11,30,41,106,117,134,147,185,200,264,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
    {4,20,52,118,148,159,201,212,234,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {12,21,62,73,84,96,107,119,160,213,243,254,265,0,0,0,0,0,0,0,0,0,0,0,13},
    {22,31,42,63,97,120,135,149,186,202,224,235,0,0,0,0,0,0,0,0,0,0,0,0,12},
    {32,43,53,74,85,108,150,161,187,203,214,244,0,0,0,0,0,0,0,0,0,0,0,0,12},
    {5,44,54,64,86,98,121,136,162,173,188,204,215,225,255,0,0,0,0,0,0,0,0,0,15},
    {13,23,33,55,75,87,109,122,137,174,189,216,266,0,0,0,0,0,0,0,0,0,0,0,13},
    {6,56,65,76,110,123,138,151,163,175,205,226,236,267,0,0,0,0,0,0,0,0,0,0,14},
    {14,66,77,124,152,164,176,190,217,237,245,0,0,0,0,0,0,0,0,0,0,0,0,0,11},
    {24,78,88,165,177,191,206,227,246,256,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
    {34,45,57,67,79,99,111,125,139,153,166,192,218,228,257,0,0,0,0,0,0,0,0,0,15},
    {7,35,58,89,100,140,154,207,238,268,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
    {8,15,68,101,112,155,167,219,239,247,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10},
    {16,25,46,80,90,113,126,141,168,178,193,229,248,258,0,0,0,0,0,0,0,0,0,0,14},
    {26,47,91,127,142,179,194,259,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8}
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
        	SR[i][j] = ColNum*j+i;
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
			SR_Column[ColNum*j+i] = SR_Copy[i][j];
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







