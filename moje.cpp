#include "moje.h"

player::player()
{
angle=0.0;
level=0.0;
lives=10;
speed=0.0;
}

opponent::opponent()
{
type=0;angle=0;level=4.0;speed=0.1;
}
opponent::opponent(int typ)
{
type=typ;angle=0;level=4.0;speed=0.1;
}
opponent::opponent(int typ, float angl, float lev, float spee)
{
type=typ;angle=angl;level=lev;speed=spee;
}
void opponent::checker()
{
/* TYPE 0: SPEED=0
   TYPE 1: SPEED>0 LEVEL=CONST
   TYPE 2: SPEED>0 LEVEL++
   TYPE 3: SPEED>0 LEVEL--
   TYPE 4: SPEED<0 LEVEL=CONST
   TYPE 5: SPEED<0 LEVEL++
   TYPE 6: SPEED<0 LEVEL--       */
switch(type)
{
case 0: break;
case 1: {if (angle>19.8){angle=-10.0;}}break;
case 2:{
if (angle>19.8){angle=-10.0;
	if(level<35.9){level+=4.0;}else{level=4.0;}}
}break;
case 3:{
if (angle>19.8){angle=-10.0;
	if(level>4.1){level-=4.0;}else{level=36.0;}}
}break;
case 4:{if (angle<-10.0){angle=19.8;}}break;
case 5:{
if (angle<-10.0){angle=19.8;
	if(level<35.9){level+=4.0;}else{level=4.0;}}

}break;
case 6:{
if (angle<-10.0){angle=19.8;
	if(level>4.1){level-=4.0;}else{level=36.0;}}

}break;
}


}


bool przedzial(float co,float mniejsza, float wieksza)
{
if(mniejsza<=wieksza)
{
	if((co<mniejsza) || (co>wieksza)){return false;}
	else{return true;}
}
else
{
	if((co>mniejsza) || (co<wieksza)){return false;}
	else{return true;}
}
}



void jedynkuj(int tab[10][100])
{
for(int i=0;i<10;i++)
{
    for(int j=0;j<100;j++)
    {
        tab[i][j]=1;
    }
}
int los;
for(int k=0;k<10;k++)
{
    los = rand() % 80;
    for(int l=0;l<12;l++)
    {
        tab[k][los+l]=0;
    }
}
}


void leworuch(int tab[10][100], int w)
{
    int i=0;
    if(tab[w][0]==0)
    {
        while(tab[w][i]!=1)
        {i++;}
        tab[w][i-1]=1;
        while(tab[w][i]!=0)
        {if(i==99){i++;break;}else{i++;}}
        tab[w][i-1]=0;
    }
    else{
        while(tab[w][i]!=0)
        {i++;}
        tab[w][i-1]=0;
        while(tab[w][i]!=1)
        {if(i==99){i++;break;}else{i++;}}
        tab[w][i-1]=1;
    }
}

void praworuch(int tab[10][100], int w)
{
    int i=99;
    if(tab[w][99]==0){
        while(tab[w][i]!=1)
        {i--;}
        tab[w][i+1]=1;
        while(tab[w][i]!=0)
        {if(i==0){i--;break;}else{i--;}}
        tab[w][i+1]=0;
    }
    else{
        while(tab[w][i]!=0)
        {i--;}
        tab[w][i+1]=0;
        while(tab[w][i]!=1)
        {if(i==0){i--;break;}else{i--;}}
        tab[w][i+1]=1;
    }
}

void wys(int tab[10][100])
{
    cout<<endl;
for(int i=0;i<10;i++)
{
    for(int j=0;j<100;j++)
    {
        cout<<tab[i][j];
    }
    cout<<endl<<endl;
}
}

bool tlumacz(int tab[10][100],float kat,float lev)
{
if(tab[(int)(9-round(lev/4.0))][(int)(floor((kat+10.0)/0.3))]==0 && (((int)(9-round(lev/4.0)))!=-1))
	{return true;}
else 
	{return false;}
}
bool tlumaczdol(int tab[10][100],float kat,float lev)
{
if(tab[(int)(9-round(lev/4.0)+1)][(int)(floor((kat+10.0)/0.3))]==0 && (((int)(9-round(lev/4.0)+1))!=10))
	{return true;}
else 
	{return false;}
}
