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
        bool insertSector(int s)
        {
            if(s == 0){cout<<"\nINSERCION CANCELADA\n";return false;}
            if(capacity >= used)
            {
                vector<int>cords = discRef->processValues(1,1,1,s);
                ifstream reader("d:/UNSA/BD II/Disco - BD - V3/Disco/Plato"+ to_string(cords[0]) +"/Superficie"+ to_string(cords[1]) +"/Pista"+ to_string(cords[2]) +"/Sector"+ to_string(cords[3]) +".txt");
                string line;
                getline(reader,line);

                if(line.size() == 0 && !sectors.empty())
                {
                    if(!nextBloque->insertSector(s))
                        return false;
                }

                if(used+line.size() <= capacity)
                {
                    used += line.size();
                    sectors.push_back(s);
                    return true;
                }
                else
                {
                    if(!nextBloque->insertSector(s))
                        return false;
                }
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