#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "csv_file.h"
#include "Disc.h"
using namespace std;

class Bloque
{
    public:
        int capacity;
        int used = 0;
        int qSec;
        int num;
        vector<int> sectors;
        Disc* discRef;
        Bloque * nextBloque = nullptr;
    public:
        Bloque(int cap, int num_, int bys, Disc* ref):capacity(cap),num(num_),qSec(bys),discRef(ref){}
        ~Bloque(){delete nextBloque;}
        int getNum(){return num;}
        vector<int> getBloqNum(int nSector)
        {
            vector<int> tmp = {num,0};
            for(int i = 0; i<sectors.size(); i++)
            {
                if(i == nSector)
                {
                    tmp[1] = i;
                    return tmp;
                }
            }
            if(!nextBloque)
                return {0,0};
            return nextBloque->getBloqNum(nSector);
        }
        template<typename T>
        void insertInBlock(T data, int nB)
        {
            if(nB>1)
                nextBloque->insertInBlock(data,nB-1);
            else
            {
                for(int i = 0; i<sectors.size();i++)
                {
                    vector<int>cords = discRef->processValues(1,1,1,sectors[i]);
                    ifstream reader("d:/UNSA/BD II/Disco - BD - V3/Disco/Plato"+ to_string(cords[0]) +"/Superficie"+ to_string(cords[1]) +"/Pista"+ to_string(cords[2]) +"/Sector"+ to_string(cords[3]) +".txt");
                    string line;
                    reader>>line;
                    if(line.size()+data.size()<=qSec)
                    {
                        used += data.size();
                        ofstream writer;
                        writer.open("d:/UNSA/BD II/Disco - BD - V3/Disco/Plato"+ to_string(cords[0]) +"/Superficie"+ to_string(cords[1]) +"/Pista"+ to_string(cords[2]) +"/Sector"+ to_string(cords[3]) +".txt", fstream::app);
                        writer<<data;
                        writer.close();
                        return;
                    }
                }
                nextBloque->insertInBlock(data,nB+1);
            }
        }
        bool insertSector(int s)
        {
            if(s == 0){cout<<"\nINSERCION CANCELADA\n";return false;}
            if(sectors.size() < capacity/qSec)
            {
                vector<int>cords = discRef->processValues(1,1,1,s);
                sectors.push_back(s);
                return true;
            }
            else
            {
                if(!nextBloque->insertSector(s))
                        return false;
            }
            return false;
        }
        void modify(vector<string> information,vector<bool> dirty)
        {
            for(int i = 0; i<dirty.size();i++)
            {
                if(dirty[i])
                {
                    ofstream writer;
                    vector<int>cords = discRef->processValues(1,1,1,sectors[i]);
                    writer.open("d:/UNSA/BD II/Disco - BD - V3/Disco/Plato"+ to_string(cords[0]) +"/Superficie"+ to_string(cords[1]) +"/Pista"+ to_string(cords[2]) +"/Sector"+ to_string(cords[3]) +"_temp.txt");
                    writer<<information[i];
                    writer.close();
                    remove(("d:/UNSA/BD II/Disco - BD - V3/Disco/Plato"+ to_string(cords[0]) +"/Superficie"+ to_string(cords[1]) +"/Pista"+ to_string(cords[2]) +"/Sector"+ to_string(cords[3]) +".txt").c_str());
                    rename(("d:/UNSA/BD II/Disco - BD - V3/Disco/Plato"+ to_string(cords[0]) +"/Superficie"+ to_string(cords[1]) +"/Pista"+ to_string(cords[2]) +"/Sector"+ to_string(cords[3]) +"_temp.txt").c_str(),("d:/UNSA/BD II/Disco - BD - V3/Disco/Plato"+ to_string(cords[0]) +"/Superficie"+ to_string(cords[1]) +"/Pista"+ to_string(cords[2]) +"/Sector"+ to_string(cords[3]) +".txt").c_str());
                }
            }
        }
};