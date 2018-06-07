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
const int BlockSize = 196;
const int MCInter = 406;
const int ColNum = 7;
const int ColSize = 28; //BlockSize = ColNum * ColSize
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
	{0,1,2,3,4,5,6,7,8,9,10,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,12},
    {12,13,14,15,16,17,18,19,20,21,22,23,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,12},
    {24,25,26,27,28,29,30,31,32,33,34,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11},
    {35,36,37,38,39,40,41,42,43,44,45,46,47,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,14},
    {49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,0,0,0,0,0,0,0,0,0,0,0,0,16},
    {65,66,67,68,69,70,71,72,73,74,75,76,77,78,0,0,0,0,0,0,0,0,0,0,0,0,0,0,14},
    {79,80,81,82,83,84,85,86,87,88,89,90,91,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13},
    {92,93,94,95,96,97,98,99,100,101,102,103,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,12},
    {104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,0,0,0,0,0,0,0,0,0,0,0,0,16},
    {120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,0,0,0,0,0,0,0,0,20},
    {140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,0,0,0,0,0,0,0,0,0,0,18},
    {158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,0,0,0,0,0,0,0,21},
    {179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,0,0,0,0,0,0,0,0,0,0,0,17},
    {196,197,198,199,200,201,202,203,204,205,206,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11},
    {207,208,209,210,211,212,213,214,215,216,217,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11},
    {218,219,220,221,222,223,224,225,226,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,0,0,0,0,0,0,0,0,0,0,0,0,0,15},
    {242,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,0,0,0,0,0,0,0,0,0,0,18},
    {260,261,262,263,264,265,266,267,268,269,270,271,272,273,274,275,276,0,0,0,0,0,0,0,0,0,0,0,17},
    {277,278,279,280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296,0,0,0,0,0,0,0,0,20},
    {297,298,299,300,301,302,303,304,305,306,307,308,309,310,311,312,0,0,0,0,0,0,0,0,0,0,0,0,16},
    {313,314,315,316,317,318,319,320,321,322,323,324,325,326,327,0,0,0,0,0,0,0,0,0,0,0,0,0,15},
    {328,329,330,331,332,333,334,335,336,337,338,339,340,341,0,0,0,0,0,0,0,0,0,0,0,0,0,0,14},
    {342,343,344,345,346,347,348,349,350,351,352,353,354,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13},
    {355,356,357,358,359,360,361,362,363,364,365,366,367,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13},
    {368,369,370,371,372,373,374,375,376,377,378,379,380,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13},
    {381,382,383,384,385,386,387,388,389,390,391,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11},
    {392,393,394,395,396,397,398,399,400,401,402,403,404,405,0,0,0,0,0,0,0,0,0,0,0,0,0,0,14}
};

int ROW[ColSize][ColSize+1]={
	{0,79,120,140,179,227,313,328,381,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {12,49,92,104,141,158,196,242,297,329,342,355,392,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13},
    {24,50,65,121,159,207,260,314,343,356,368,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11},
    {35,66,105,122,218,277,298,315,369,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {25,67,123,142,160,197,228,261,299,316,344,357,382,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13},
    {1,36,80,143,161,208,229,243,278,300,317,330,358,370,393,0,0,0,0,0,0,0,0,0,0,0,0,0,15},
    {2,13,51,93,162,180,219,230,244,262,301,318,331,345,359,371,383,0,0,0,0,0,0,0,0,0,0,0,17},
    {14,52,106,124,144,163,181,245,279,302,332,372,394,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13},
    {3,26,53,68,125,145,164,182,198,220,231,246,263,303,346,384,395,0,0,0,0,0,0,0,0,0,0,0,17},
    {4,15,37,69,81,146,165,183,199,209,232,247,264,280,304,319,396,0,0,0,0,0,0,0,0,0,0,0,17},
    {5,16,27,54,82,94,166,184,200,210,221,248,265,281,320,333,360,0,0,0,0,0,0,0,0,0,0,0,17},
    {17,38,55,95,107,126,147,167,185,211,233,249,282,334,373,385,397,0,0,0,0,0,0,0,0,0,0,0,17},
    {28,39,83,127,168,201,234,250,266,321,335,347,398,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13},
    {40,56,96,108,148,212,235,251,267,283,336,348,361,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13},
    {6,57,70,109,128,169,186,222,252,268,284,349,374,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13},
    {18,29,41,71,110,149,170,187,236,253,285,305,322,337,350,386,399,0,0,0,0,0,0,0,0,0,0,0,17},
    {42,72,84,97,111,129,150,202,254,286,338,387,400,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13},
    {7,85,98,112,130,151,171,213,237,269,306,351,401,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13},
    {19,99,131,152,172,188,223,238,255,287,307,323,362,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13},
    {30,43,58,73,86,100,113,132,173,189,239,270,288,324,375,388,402,0,0,0,0,0,0,0,0,0,0,0,17},
    {31,44,59,101,114,133,153,190,203,224,256,271,289,308,325,363,389,0,0,0,0,0,0,0,0,0,0,0,17},
    {45,60,74,115,134,154,174,191,204,214,272,290,326,339,364,376,403,0,0,0,0,0,0,0,0,0,0,0,17},
    {8,75,87,135,155,175,192,205,215,225,291,309,340,352,365,377,390,0,0,0,0,0,0,0,0,0,0,0,17},
    {20,32,46,88,116,136,176,193,216,240,257,273,292,310,353,378,404,0,0,0,0,0,0,0,0,0,0,0,17},
    {9,33,61,76,102,117,156,206,258,274,293,327,391,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13},
    {10,21,47,62,77,89,118,137,177,217,275,294,341,366,405,0,0,0,0,0,0,0,0,0,0,0,0,0,15},
    {11,22,34,63,78,90,103,119,138,157,194,226,295,311,354,367,379,0,0,0,0,0,0,0,0,0,0,0,17},
    {23,48,64,91,139,178,195,241,259,276,296,312,380,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13}
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
