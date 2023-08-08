#pragma once
#include <iostream>
#include <direct.h>
#include <cstdint>
#include <fstream>
#include <vector>
#include "Platter.h"
using namespace std;
class Disc
{
    protected:
        int SecBytes;
        int qPlato;
        int qSector;
        int qTrack;
        Platter * nPlato = nullptr;
    public:

        Disc(int platterQn, int TrackQn, int SectorQn,int bytes):qPlato(platterQn),qTrack(TrackQn),qSector(SectorQn),SecBytes(bytes)
        {
            std::cout<<"\n\nDISCO CREADO\n\n";
            do {
                if(!nPlato){nPlato = new Platter(TrackQn,SectorQn,qPlato-platterQn+1,bytes);}
                else{nPlato->setPlatter(new Platter(TrackQn,SectorQn,qPlato-platterQn+1,bytes));}
                platterQn--;
            } while(platterQn > 0);
            createDisc(qPlato,TrackQn,SectorQn);
        }
        ~Disc()
        {
            std::cout<<"\nMurio el DISCO\n\n";
            delete nPlato;
            system("rmdir /s /q Disco");
        }
        int getQPlate(){return qPlato;}
        int getQSector(){return qSector;}
        int getQTrack(){return qTrack;}
        int getTotalSectors(){return qPlato*2*qSector*qTrack;}
        int getSectorByte(){return SecBytes;}
        void makeSectorReserved(int p, int sf, int t, int s)
        {
            nPlato->makeSectorReserved(p,sf,t,s);
        }
        void metaDiscData()
        {
            cout<<"\n- 1 Disco\n- "<<qPlato<<" platos\n- "<<qTrack<<" pistas\n- "<<qSector<<" sectores";
            cout<<"\n- Bytes por Sector: "<<SecBytes<<"\n- Sectores en total en el disco: "<<getTotalSectors()<<"\nCapacidad total del disco: "<<getTotalSectors()*SecBytes<<endl;
        }
        void printSector(int p, int sf, int t, int s, int counter = 1)
        {
            if(p > qPlato)
                cout<<"\nIMPOSIBLE DE IMPRIMIR\n";
            else if(sf > 2)
                printSector(p+1,1,t,s);
            else if(t > qTrack)
                printSector(p,sf+1,t-qTrack,s);
            else if(s > qSector)
                printSector(p,sf,t+1,s-qSector);
            else
                nPlato->print(p,sf,t,s,counter);
        }
        vector<int> processValues(int p, int sf, int t, int s)
        {
            if(p > qPlato)
            {
                cout<<"\nIMPOSIBLE\n";
                vector<int> list = {0,0,0,0};
                return list;
            }
            else if(sf > 2)
                return processValues(p+1,1,t,s);
            else if(t > qTrack)
                return processValues(p,sf+1,t-qTrack,s);
            else if(s > qSector)
                return processValues(p,sf,t+1,s-qSector);
            else
            {
                vector<int> list = {p,sf,t,s};
                return list;
            }
        }
        int inverseValues(int p, int sf, int t, int s)
        {
            if(p > qPlato)
            {
                cout<<"\nIMPOSIBLE\n";
                return 0;
            }
            else if(p > 1)
                return inverseValues(p-1,sf+2,t,s);
            else if(sf > 1)
                return inverseValues(p,sf-1,t+qTrack,s);
            else if(t > 1)
                return inverseValues(p,sf,t-1,s+qSector);
            else
                return s;
        }
        void createDisc(int qPlatter, int qTrack, int qSector)
        {
            string pathOG = "d:/UNSA/BD II/Disco - BD - V3/Disco";
            string pathDisc = pathOG;
            mkdir(pathDisc.c_str());
            for(int i = 1; i<=qPlatter;i++)
            {
                pathDisc += "/Plato";
                pathDisc += to_string(i);
                mkdir(pathDisc.c_str());
                for(int j = 1; j<=2;j++)
                {
                    mkdir((pathDisc + "/Superficie" + to_string(j)).c_str());
                    pathDisc += "/Superficie" + to_string(j);
                    for(int k = 1; k<=qTrack; k++)
                    {

                        mkdir((pathDisc + "/Pista" + to_string(k)).c_str());
                        for(int l = 1; l <= qSector; l++)
                        {
                            ofstream file;
                            file.open(pathDisc + "/Pista" + to_string(k) + "/Sector" + to_string(l) + ".txt");
                            //file.flush();
                            file.close();
                        } 
                    }
                    pathDisc = pathOG + "/Plato" + to_string(i);
                }
                pathDisc = pathOG;
            }
        }
        template<typename T>
        bool insertData(T a, vector<int>&cords)
        {
            if(!nPlato->insertPlatterData<T>(a,cords))
                return false;
            return true;    
        }
};