#pragma once
#include <iostream>
#include "Surface.h"
using namespace std;
class Platter
{
    protected:
        int PlatterNum;
        Platter * nextPl = nullptr;
        Surface * Surface_1 = nullptr;
        Surface * Surface_2 = nullptr;
    public:
        Platter(int TrackQn, int SectorQn, int n, int bytes):PlatterNum(n){Surface_1 = new Surface(TrackQn,SectorQn,bytes); 
        Surface_2 = new Surface(TrackQn,SectorQn,bytes);} 
        ~Platter(){delete nextPl; delete Surface_1; delete Surface_2;}
        void setPlatter(Platter * next)
        {
            if(!nextPl)
                nextPl = next;
            else
                nextPl->setPlatter(next);
        }
        void makeSectorReserved(int p, int sf, int t, int s)
        {
            if(p != 1)
                nextPl-> makeSectorReserved(p-1,sf,t,s);
            else
            {
                if(sf == 1)
                    Surface_1->makeSectorReserved(t,s);
                else
                    Surface_2->makeSectorReserved(t,s);
            }
            
        }
        template<typename T>
        bool insertPlatterData(T d,string direction)
        {
            string nD = direction + "/Plato" + to_string(PlatterNum);
            if(!Surface_1->insertSurfaceData<T>(d,nD + "/Superficie1"))
            {       
                if(!Surface_2->insertSurfaceData<T>(d,nD + "/Superficie2"))
                {
                    if(nextPl)
                        if(nextPl->insertPlatterData<T>(d,direction))
                            return true;
                    return false;
                }
                return true;
            }
            return true;
        }
        void print(int p, int sf, int t, int s, int counter)
        {
            if(p != counter)
                nextPl-> print(p,sf,t,s,counter+1);
            else
            {
                if(sf == 1)
                    Surface_1->print(p,sf,t,s);
                else
                    Surface_2->print(p,sf,t,s);
            }
        }
};