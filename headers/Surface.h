#pragma once
#include <iostream>
#include "Track.h"
using namespace std;
class Surface
{
    protected:
        int qTrack;
        Track * nTrack = nullptr;
    public:
        Surface(int TrackQn,int SectorQn, int bytes):qTrack(TrackQn)
        {
            do {
                if(!nTrack){nTrack = new Track(SectorQn,qTrack-TrackQn+1,bytes);}
                else{nTrack->setTrack(new Track(SectorQn,qTrack-TrackQn+1,bytes));}
                TrackQn--;
            } while(TrackQn > 0);
        }
        ~Surface(){delete nTrack;}
        void makeSectorReserved(int t,int s)
        {
            nTrack->makeSectorReserved(t,s);
        }
        void print(int p, int sf,int t,int s)
        {
            nTrack->print(p,sf,t,s,1);
        }
        template<typename T>
        bool insertSurfaceData(T d, vector<int>&cords)
        {
            if(nTrack->insertTrackData<T>(d,cords))
                return true;
            return false;
        }
};