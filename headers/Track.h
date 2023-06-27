#pragma once
#include <iostream>
#include "Sector.h"
using namespace std;
class Track
{
    protected:
        int nTrack;
        int qSector;
        Sector * nSector = nullptr;
        Track * nextTrack = nullptr;
    public:
        Track(int SectorQn, int n):qSector(SectorQn),nTrack(n)
        {
            do {
                if(!nSector){nSector = new Sector(1);}
                else{nSector->setSector(new Sector(qSector-SectorQn+1));}
                SectorQn--;
            } while(SectorQn > 0);
        }
        ~Track(){delete nSector;delete nextTrack;}
        void makeSectorReserved(int t,int s)
        {
            if(t!=1)
                nextTrack->makeSectorReserved(t-1,s);
            else
                nSector->MakeReserved(s);
        }
        template<typename T>
        bool insertTrackData(T d,string direction)
        {
            string nd = direction + "/Pista" + to_string(nTrack);
            if(!nSector->insertData<T>(d,nd))
            {
                if(nextTrack)
                    if(nextTrack->insertTrackData<T>(d,direction))
                        return true;
                return false;
            }      
            return true;
        }
        void setTrack(Track * next)
        {
            if(!nextTrack)
                nextTrack = next;
            else
                nextTrack->setTrack(next);
        }
        void print(int p, int sf,int t, int s, int counter)
        {
            if(t != counter)
                nextTrack->print(p,sf,t,s,counter+1);
            else
                nSector->print(p,sf,t,s,1);
        }
};