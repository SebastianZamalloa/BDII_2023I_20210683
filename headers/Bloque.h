#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "csv_file.h"
#include "Disc.h"
using namespace std;

class Bloque
{
    protected:
        int capacity;
        int used = 0;
        int num;
    public:
        Bloque * nextBloque = nullptr;
    public:
        Bloque(int cap, int num_):capacity(cap),num(num_)
        {
            ofstream writer;
            writer.open(("d:/UNSA/BD II/Disco - BD - V3/Bloques/Bloque" + to_string(num) + ".txt").c_str(), fstream::app); 
            writer.close(); 
        }
        ~Bloque(){delete nextBloque;}
        int getNum(){return num;}
        bool insertSector(int p, int sf, int t, int s)
        {
            if(s == 0){cout<<"\nINSERCION CANCELADA\n";return false;}
            if(capacity >= used)
            {
                ifstream reader("d:/UNSA/BD II/Disco - BD - V3/Disco/Plato"+ to_string(p) +"/Superficie"+ to_string(sf) +"/Pista"+ to_string(t) +"/Sector"+ to_string(s) +".txt");
                ofstream writer;
                writer.open(("d:/UNSA/BD II/Disco - BD - V3/Bloques/Bloque" + to_string(num) + ".txt").c_str(), fstream::app);
                string line;
                getline(reader,line);
                if(used+line.size() <= capacity)
                {
                    used += line.size();
                    writer<<line;
                    writer.close();
                    return true;
                }
                else
                {
                    if(!nextBloque->insertSector(p,sf,t,s))
                        return false;
                }
                writer.close();
            }
            else
            {
                if(!nextBloque->insertSector(p,sf,t,s))
                        return false;
            }
            return false;
        }/*
        void showMetadata()
        {
            cout<<"Bloque "<<num<<": \n\n- Capacidad: "<<capacity<<" sectores\n- Capacidad en bytes: "<<capacity*7140;
            string keywords[4] = {"\n- Plato ","\n- Superficie ","\n- Pista ","\n- Sector "};
            for(int i=0;i<coords.size();i++)
                cout<<keywords[i]<<coords[i];
            cout<<endl<<endl;
        }*/
        void printBloque()
        {
            //showMetadata();
            ifstream reader("d:/UNSA/BD II/Disco - BD - V3/Bloques/Bloque" + to_string(num) + ".txt");
            cout<<"d:/UNSA/BD II/Disco - BD - V3/Bloques/Bloque" + to_string(num) + ".txt"<<endl;
            char c;
            while(reader.get(c) && !reader.eof())
                cout<<c;
            cout<<endl<<endl;
        }
};