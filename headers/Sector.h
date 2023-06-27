#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class Sector
{
    protected:
        int nSector;
        int limit;
        int used;
        bool reserved;
        Sector * nextSct = nullptr;     
    public:
        Sector(int n):nSector(n){limit = 7140;used = 0;reserved = false;}
        ~Sector(){delete nextSct;}
        void MakeReserved(int s)
        {   
            if(s!=1)
                nextSct->MakeReserved(s-1);
            else
                reserved = true;
        }
        Sector* getSector()
        {
            return nextSct;
        }
        template<typename T>
        bool insertData(T _data_, string direction)
        {
            string d; d += _data_;
            string nd = direction + "/Sector" + to_string(nSector) + ".txt";
            if(limit == used || reserved || d.length() > limit-used)
            {
                if(nextSct)
                    if(nextSct->insertData<T>(_data_,direction))
                        return true;
                return false;
            }
            ofstream writer;
            writer.open(nd.c_str(), fstream::app);
            writer << _data_;
            writer.close();
            used += d.length();
            return true;         
        }
        void setSector(Sector * next)
        {
            if(!nextSct)
                nextSct = next;
            else
                nextSct->setSector(next);
        }
        void print(int p, int sf, int t,int s,int counter)
        {
            if(s!=counter)
                nextSct->print(p,sf,t,s,counter+1);
            else
            {
                cout<<"Sector "<<s<<endl;
                cout<<"- Plato "<<p<<"\n- Superficie "<<sf<<"\n- Pista "<<t<<"\n- Capacidad: 1024 bytes\n\n";
                ifstream reader("d:/UNSA/BD II/Disco - BD - V3/Disco/Plato"+ to_string(p) +"/Superficie"+ to_string(sf) +"/Pista"+ to_string(t) +"/Sector"+ to_string(s) +".txt");
                char c;
                while(reader.get(c) && !reader.eof())
                    cout<<c;
            }
        }
};