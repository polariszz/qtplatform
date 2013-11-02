#pragma warning(disable : 4996)
/* for file operate
 * by Bin Tang
 * integrated by polariszz
 */
#include "fileoperate.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define PI 3.141592653
#define N 3
#define _CRT_SECURE_NO_WARNINGS(_CRT_SECURE_NO_DEPRECATE )

void transfer(char* file1, char* file2, char* file3) {
    FILE *fp1, *fpw, *fpw1;
    int i, n;
    char name[300];
    double x, y, z;
    int num;
    if ((fp1 = fopen(file1, "r")) == NULL) {
        printf("Can not open this file ! \n");
        return;
    }

    if ((fpw = fopen(file2, "w")) == NULL) {
        printf("Can not open this file ! \n");
        return;
    }

    if ((fpw1 = fopen(file3, "w")) == NULL) {
        printf("Can not open this file !\n");
        return;
    }
    fprintf(fpw,"        /n");
        fprintf(fpw1,"        /n");
        fgets(name,300,fp1);
        fgets(name,300,fp1);
        fgets(name,300,fp1);
        fgets(name,300,fp1);
        num=0;
        while( !feof( fp1 ) )
        {
            fgets(name,300,fp1);
            for(i=0;i<20;i++)
            {
                if(feof( fp1 ))
                    break;
                fscanf(fp1,"%d",&n);
                fscanf(fp1,"%lf",&x);
                fscanf(fp1,"%lf",&y);
                fscanf(fp1,"%lf\n",&z);
                fprintf(fpw,"%8d%16.8e%16.8e%16.8e\n",n,x,y,z);
                fprintf(fpw1,"%d\n",n);
                num++;
            }
        }
        rewind(fpw);
        rewind(fpw1);
        fprintf(fpw,"%8d\n",num);
        fprintf(fpw1,"%8d\n",num);
        fclose(fp1);
        fclose(fpw1);
        fclose(fpw);
}


void couple(char *file1, char *file2, char *file3, char *file4) {
    FILE *fp1, *fp2, *fpw, *fpw1;
    int i, n;
    char name[300];
    double x, y, z;
    int num1, num2;

    if((fp1 = fopen(file1, "r")) == NULL){
        printf("Can not open this file! \n");
        return;
    }
    if((fp2 = fopen(file2, "r")) == NULL){
        printf("Can not open this file! \n");
        return;
    }
    if((fpw = fopen(file3, "w")) == NULL){
        printf("Can not open this file! \n");
        return;
    }
    if((fpw1 = fopen(file4, "w")) == NULL){
        printf("Can not open this file! \n");
        return;
    }

    fprintf(fpw,"        /n");
        fprintf(fpw1,"        /n");
        fgets(name,300,fp1);//跳过文件排头部分
        fgets(name,300,fp1);
        fgets(name,300,fp1);
        fgets(name,300,fp1);
        fgets(name,300,fp2);
        fgets(name,300,fp2);
        fgets(name,300,fp2);
        fgets(name,300,fp2);
        num1=0;
        num2=0;
        while(!feof(fp1))
        {
            fgets(name,300,fp1);
            if (!feof(fp2))
            {
                fgets(name,300,fp2);
            }
            for(i=0;i<20;i++)
            {
                if (!feof(fp2))
                {
                    fscanf(fp1,"%d",&n);
                    fscanf(fp1,"%lf",&x);
                    fscanf(fp1,"%lf",&y);
                    fscanf(fp1,"%lf\n",&z);
                    fprintf(fpw,"%d\n",n);
                    fscanf(fp2,"%d",&n);
                    fscanf(fp2,"%lf",&x);
                    fscanf(fp2,"%lf",&y);
                    fscanf(fp2,"%lf\n",&z);
                    fprintf(fpw,"%d\n",n);
                    num1++;
                }
                else
                {
                    if (feof(fp1))
                    {
                        break;
                    }
                    fscanf(fp1,"%d",&n);
                    fscanf(fp1,"%lf",&x);
                    fscanf(fp1,"%lf",&y);
                    fscanf(fp1,"%lf\n",&z);
                    fprintf(fpw1,"%d\n",n);
                    num2++;

                }
            }

        }
        rewind(fpw);
        rewind(fpw1);
        fprintf(fpw,"%8d\n",num1);
        fprintf(fpw1,"%8d\n",num2);
        fclose(fp1);
        fclose(fpw1);
        fclose(fpw);
}

struct node
{
    double x[3];
    double pres;
    double tem;
    double conv;
    double l;
};
typedef struct node* P_n;
double Basefunc_l(P_n nlist,double* x,int l)//求拉格朗日的基函数值 s为记录被选择用于插值的节点号的结构
{
    int i;
    double temp=1;
    for(i=0;i<N;i++)
    {
        if(i!=l)
        {
            temp*=((x[0]-nlist[i].x[0])*(nlist[l].x[0]-nlist[i].x[0])+(x[1]-nlist[i].x[1])*(nlist[l].x[1]-nlist[i].x[1])+(x[2]-nlist[i].x[2])*(nlist[l].x[2]-nlist[i].x[2]));
            temp/=((nlist[l].x[0]-nlist[i].x[0])*(nlist[l].x[0]-nlist[i].x[0])+(nlist[l].x[1]-nlist[i].x[1])*(nlist[l].x[1]-nlist[i].x[1])+(nlist[l].x[2]-nlist[i].x[2])*(nlist[l].x[2]-nlist[i].x[2]));
        }
    }
    return temp;
}

double* Mul_lagrange(P_n nlist,double* x)//拉格朗日插值公式
{
    int i;
    double *info = new double[3];
    double t=0;
    double t1;
    if (nlist[0].l>9)//最近测点大于3mm的点将忽略
    {
        return NULL;
    }
    for(i=0;i<3;i++)
    {
        info[i]=0;
    }
    for(i=0;i<N;i++)
    {
        t1=Basefunc_l(nlist,x,i);
        t+=t1;//系数归一化的分母值
        info[0]+=nlist[i].tem*t1;
        info[1]+=nlist[i].conv*t1;
        info[2]+=nlist[i].pres*t1;
    }
    for (i=0;i<3;i++)
    {
        info[i]/=t;
    }
    return info;
}

double** initializer(FILE *fp1, int* count) {
    double** data;
    char s[300];
    double temp1;
    int i,num;
    for (i=0;i<6;i++)//读入原始数据文件开始文字部分，即将文件指针移到数据数据处
    {
        fgets(s,300,fp1);
    }
    num=0;
    while(!feof(fp1))
    {
        num++;
        for (i=0;i<3;i++)//读入坐标
        {
            fscanf(fp1,"%lf,",&temp1);
        }
        fscanf(fp1,"%lf,",&temp1);//修改压强
        fscanf(fp1,"%lf,",&temp1);//半径不需要
        fscanf(fp1,"%lf,",&temp1);//总压不需要
        fscanf(fp1,"%lf,",&temp1);//温度
        fscanf(fp1,"%lf\n",&temp1);//修改对流换热系数
    }
    rewind(fp1);
    *count=num;
    data=(double**)malloc(sizeof(double*)*num);
    for (i=0;i<num;i++)
    {
        data[i]=(double*)malloc(sizeof(double)*6);
    }

    for (i=0;i<6;i++)//读入原始数据文件开始文字部分，即将文件指针移到数据数据处
    {
        fgets(s,300,fp1);
    }
    num=0;
    while(!feof(fp1))
    {
        for (i=0;i<3;i++)//读入坐标
        {
            fscanf(fp1,"%lf,",&temp1);
            temp1*=1000;//毫米单位，防止测点太近导致的误差
            data[num][i]=temp1;
        }
        fscanf(fp1,"%lf,",&temp1);//修改压强
        data[num][3]=temp1;
        fscanf(fp1,"%lf,",&temp1);//半径不需要
        fscanf(fp1,"%lf,",&temp1);//总压不需要
        fscanf(fp1,"%lf,",&temp1);//温度
        data[num][4]=temp1;
        fscanf(fp1,"%lf\n",&temp1);//修改对流换热系数
        data[num][5]=temp1;
        num++;
    }
    fclose(fp1);
    return data;
}

void search(double* x,P_n nlist,double** data,int num)
{
    double t;
    int i,j,k;
    node temp;
    for (i=0;i<N;i++)
    {
        for (j=0;j<3;j++)//读入坐标
        {
            nlist[i].x[j]=data[i][j];
        }
        nlist[i].pres=data[i][3];
        nlist[i].tem=data[i][4];
        nlist[i].conv=data[i][5];
        nlist[i].l=(nlist[i].x[0]-x[0])*(nlist[i].x[0]-x[0])+(x[1]-nlist[i].x[1])*(x[1]-nlist[i].x[1])+(x[2]-nlist[i].x[2])*(x[2]-nlist[i].x[2]);
    }
    for (i=0;i<N;i++)
    {
        for (j=0;j<N-1-i;j++)
        {
            if (nlist[j+1].l<nlist[j].l)
            {
                temp=nlist[j];
                nlist[j]=nlist[j+1];
                nlist[j+1]=temp;
            }
        }
    }
    for(k=N;k<num;k++)
    {
        t=(data[k][0]-x[0])*(data[k][0]-x[0])+(x[1]-data[k][1])*(x[1]-data[k][1])+(x[2]-data[k][2])*(x[2]-data[k][2]);
        if (t<nlist[N-1].l)
        {
            temp.x[0]=data[k][0];
            temp.x[1]=data[k][1];
            temp.x[2]=data[k][2];
            temp.l=t;
            temp.pres=data[k][3];
            temp.tem=data[k][4];
            temp.conv=data[k][5];
        }
        for (i=0;i<N-1;i++)
        {
            if (t<nlist[i].l)
            {
                for (j=N-2;j>=i;j--)
                {
                    nlist[j+1]=nlist[j];
                }
                nlist[i]=temp;
                break;
            }
        }
    }
}

void loadcal(char *file1, char* file2, char* file3) {
    FILE *fp1, *fp2, *fpw;
    node nlist[N];
    double *info,x[3];
    int num,count=0,t;
    double **data;
    if ((fp1=fopen(file1, "r")) == NULL){
        printf("File %s cannot be opened. \n", file1);
        return;
    }
    if ((fp2=fopen(file2, "r")) == NULL) {
        printf("File %s cannot be opened. \n", file2);
        return;
    }
    if ((fpw=fopen(file3, "w")) == NULL) {
        printf("File %s cannot be opened. \n", file3);
        return;
    }
    info=(double*)malloc(sizeof(double)*3);
    fscanf(fp2,"%d\n",&t);
    fprintf(fpw,"          \n");
    data=initializer(fp1,&count);
    while(!feof(fp2))
    {
        fscanf(fp2,"%d%lf%lf%lf\n",&num,&x[0],&x[1],&x[2]);
        search(x,nlist,data,count);
        info=Mul_lagrange(nlist,x);
        if(info==NULL)
        {
            t--;
            continue;
        }
        fprintf(fpw,"%8d%16.8e%16.8e%16.8e\n",num,info[0],info[1],info[2]);
    }
    rewind(fpw);
    fprintf(fpw,"%d",t);
    fclose(fpw);
    int k = 0;
    for (int i =0; i < 1000000000; i++)
        k = i + 2;
}
