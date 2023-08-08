#pragma once
#include <iostream>
#include <vector>
#include "Bloque.h"

class Frame
{
    public:
        int id;
        int pageID = 0;
        int pinCount = 0;
        int extra = 0;
        vector<string> information;
        vector<int> sectors;
        vector<bool> dirty;
        Frame(int _id_):id(_id_){}
        ~Frame(){}
        void rechargeFrame(Bloque *ref)
        {
            sectors = ref->sectors;
            pageID = ref->getNum();
            pinCount = 1;
            for(auto i:ref->sectors)
            {
                vector<int>cords = ref->discRef->processValues(1,1,1,i);
                ifstream reader("d:/UNSA/BD II/Disco - BD - V3/Disco/Plato"+ to_string(cords[0]) +"/Superficie"+ to_string(cords[1]) +"/Pista"+ to_string(cords[2]) +"/Sector"+ to_string(cords[3]) +".txt");
                string line;
                getline(reader,line);
                information.push_back(line);
                dirty.push_back(0);
            }
        }
        void reDisc(Bloque *&dir)
        {
            for(auto i:dirty)
            {
                if(i)
                {
                    Bloque * tmp = dir;
                    for(int i = 1; i<pageID; i++)
                        tmp = tmp->nextBloque;
                    tmp->modify(information,dirty);
                    i = 0;
                    break;
                }
            }  
        }
        void backFrame(Bloque *&dir)
        {
            reDisc(dir);
            dirty.clear();
            sectors.clear();
            information.clear();
            pageID = 0;
            extra = 0;
            pinCount = 0;
        }
        void printFrame()
        {
            for(auto i:information)
                cout<<i<<endl;
            cout<<endl;
        }
};

class BufferManager
{
    protected:
        int qFrames;
        int clockPointer = 0;
        Bloque* directory;
        vector<Frame> frames;
        int method;
    public:
        BufferManager(int spaces,Bloque * tmp,int mode):qFrames(spaces),directory(tmp),method(mode)
        {
            for(int i = 0; i<qFrames; i++)
            {
                Frame temp(i);
                frames.push_back(temp);
            }
            for(auto &i:frames)
            {
                if(method == 1)
                    i.extra = 0;
            }
        }
        ~BufferManager(){}
        int getNBloqs()
        {
            Bloque * tmp = directory;
            int count = 0;
            while(tmp)
            {
                tmp = tmp->nextBloque;
                count++;
            }
            return count;
        }
        int upFrame(int blockNum)
        {
            cout<<"\nUPFRAME\n";
            for(auto j:frames)
                cout<<"\nBloque: "<<j.pageID<<" "<<j.extra<<endl;
            Bloque * tmp = directory;
            for(int i = 1; i<blockNum; i++)
                tmp = tmp->nextBloque;
            if(method == 3)
            {
                for(int i = 0; i<frames.size();i++)
                {
                    if(frames[i].pageID == blockNum)
                    {
                        frames[i].extra = 1;
                        return i;
                    }
                }

            }
            for(int i = 0; i<frames.size();i++)
            {
                if(frames[i].pageID == 0)
                {
                    frames[i].rechargeFrame(tmp);
                    switch (method)
                    {
                        case 1:
                        case 2:
                        {
                            frames[i].extra = qFrames;
                        }break;
                        case 3:
                        {
                            frames[i].extra = 1;
                        }break;
                        default:
                            break;
                    }  
                    for(int j = 0; j<frames.size(); j++)
                        if(frames[j].pageID != blockNum)
                            frames[j].extra--;
                    return i;
                }
                else if(frames[i].pageID == blockNum)
                {
                    switch (method)
                    {
                        case 1:
                        case 2:
                        {
                            frames[i].extra = qFrames;
                                for(int j = 0; j<frames.size(); j++)
                                    if(frames[j].pageID != blockNum)
                            frames[j].extra--;
                        }break;
                        case 3:
                        {
                            frames[i].extra = 1;
                        }break;
                        default:
                            break;
                    }
                    
                    return i;
                }
                else if(frames[i].pageID != blockNum && method == 3)
                {
                    frames[i].extra = 0;
                }
            }
            switch (method)
            {
                case 1:
                {
                    int min = frames[0].extra;
                    int fpos;
                    for(int i = 0; i<frames.size();i++)
                    {
                        if(frames[i].extra <= min && frames[i].pinCount == 0)
                        {
                            min = frames[i].extra;
                            fpos = i;
                        }
                    }
                    frames[fpos].backFrame(directory);
                    frames[fpos].rechargeFrame(tmp);
                    switch (method)
                    {
                        case 1:
                        {
                            frames[fpos].extra = qFrames;
                        }break;
                        
                        default:
                            break;
                    }
                        
                    for(int j = 0; j<frames.size(); j++)
                        if(frames[j].pageID != blockNum)
                            frames[j].extra--;
                    return fpos;
                }break;
                case 2:
                {
                    int max = frames[0].extra;
                    int fpos;
                    for(int i = 0; i<frames.size();i++)
                    {
                        if(frames[i].extra >= max && frames[i].pinCount == 0)
                        {
                            max = frames[i].extra;
                            fpos = i;
                        }
                    }
                    frames[fpos].backFrame(directory);
                    frames[fpos].rechargeFrame(tmp);
                    switch (method)
                    {
                        case 1:
                        {
                            frames[fpos].extra = qFrames;
                        }break;
                        
                        default:
                            break;
                    }
                        
                    for(int j = 0; j<frames.size(); j++)
                        if(frames[j].pageID != blockNum)
                            frames[j].extra--;
                    return fpos;
                }break;
                default:
                    break;
            }
            return 0;
        }
        void look4Regist(int id)
        {
            bool waos = 0;
            for(int i = 1; i<=getNBloqs();i++)
            {
                int f = upFrame(i);
                cout<<"\nBloque "<<i<<endl;
                for(int j = 0; j<frames[f].information.size();j++)
                {
                    if(frames[f].information.size() == 0)
                        break;
                    cout<<"\nSector "<<frames[f].sectors[j]<<endl;
                    if(i == 1 && j == 0)
                        j++;
                    int x = 0;
                    if(frames[f].information[j][x] == ' ')
                        while(frames[f].information[j][x] == ' ' && frames[f].information[j][x] != '\0' && x < directory->qSec)
                            x++;
                    int k = getBytes4RegistWB(frames[f].information[j].substr(x,(getNatributes("schema.txt")+2*getMaxStr("schema.txt")*countStrBitmap(frames[f].information[j].substr(x,getNatributes("schema.txt")),"schema.txt"))),"schema.txt");
                    while(x + k < directory->qSec)
                    {
                        if(frames[f].information[j][x] == ' ')
                            while(frames[f].information[j][x] == ' ' && frames[f].information[j][x] != '\0' && x < directory->qSec)
                                x++;
                        if(x >= frames[f].information[j].size())
                            break;
                        k = getBytes4RegistWB(frames[f].information[j].substr(x,(getNatributes("schema.txt")+2*getMaxStr("schema.txt")*countStrBitmap(frames[f].information[j].substr(x,getNatributes("schema.txt")),"schema.txt"))),"schema.txt");
                        string r = frames[f].information[j].substr(x,k);
                        if(id == getIDdynamicRegist(r,"schema.txt"))
                        {
                            waos = 1;
                            vector<int> cords = directory->discRef->processValues(1,1,1,frames[f].sectors[j]);
                            cout<<"\nRegistro ID "<<id<<"\n\n-Bloque "<<i;
                            cout<<"\n-Plato "<<cords[0]<<"\n-Superficie "<<cords[1]<<"\n-Pista "<<cords[2]<<"\n-Sector: "<<cords[3]<<"\n -Peso: "<<r.size()<<"\n\n";
                            cout<<transform_dynamic_to_normal(r,"schema.txt")<<endl;
                            break;
                        }
                        x += k;
                    }
                    if(waos)
                        break;
                }
                if(waos)
                    break;
            }
            if(!waos)
                cout<<"Registro no encontrado\n";

        }
        void insertRegist(string content, bool dynamic, int k = 0)
        {
            int block = -1;
            int parameter[4];
            int s = upFrame(1);
            istringstream reader(frames[s].information[0]);
            string line;
            getline(reader,line);
            string h = line;
            while(getline(reader,line))
            {
                istringstream deleteSpace(line);
                string info;
                if(block == -1)
                {
                    for(int i = 0; i<4; i++)
                    {
                        getline(deleteSpace,info,',');
                        parameter[i] = stoi(info);
                    }
                    if(parameter[3] >= content.size() && block == -1)
                        block = parameter[0];
                }
                else
                    h += "\n" + line;
            }
            if(block != -1)
            {
                frames[s].information[0] = h;
                frames[s].dirty[0] = 1;
                frames[s].reDisc(directory);

                for(int i = 0; i<4; i++)
                    cout<<parameter[i]<<endl;
                
                int f = upFrame(block);
                frames[f].pinCount++;
                for(int i = parameter[2]; i<parameter[2]+content.size(); i++)
                    frames[f].information[parameter[1]][i] = content[i-parameter[2]];
                cout<<endl<<frames[f].information[parameter[1]]<<endl;
                frames[f].dirty[parameter[1]] = 1;
                frames[f].reDisc(directory);
                frames[f].pinCount--;
            }
            else
            {
                Bloque * tmp = directory;
                for(int i = 1+k; i<=getNBloqs();i++)
                {
                    if(tmp->used + content.size() <= tmp->capacity)
                    {
                        block = i;
                        break;
                    }
                    tmp = tmp->nextBloque;
                }
                cout<<"\nHOLA\n";
                int f = upFrame(block);
                frames[f].pinCount++;
                for(int i = 0; i<frames[f].information.size();i++)
                {
                    if(block == 1 && i == 0)
                        i = 1;
                    if(frames[f].information[i].size() + content.size() <= directory->qSec)
                    {
                        frames[f].information[i] += content;
                        frames[f].dirty[i] = 1;
                        frames[f].reDisc(directory);
                        frames[f].pinCount--;
                        return;
                    }
                }
                insertRegist(content,dynamic,block);
            }
        }
        void deleteRegist(int id, bool dynamic)
        {
            int block;
            int sector;
            bool waos = 0;
            for(int i = 1; i<=getNBloqs();i++)
            {
                int f = upFrame(i);
                cout<<"\nBloque "<<i<<endl;
                for(int j = 0; j<frames[f].information.size();j++)
                {
                    if(frames[f].information.size() == 0)
                        break;
                    cout<<"\nSector "<<frames[f].sectors[j]<<endl;
                    if(i == 1 && j == 0)
                        j++;
                    int x = 0;
                    if(frames[f].information[j][x] == ' ')
                        while(frames[f].information[j][x] == ' ' && frames[f].information[j][x] != '\0' && x < directory->qSec)
                            x++;
                    int k = getBytes4RegistWB(frames[f].information[j].substr(x,(getNatributes("schema.txt")+2*getMaxStr("schema.txt")*countStrBitmap(frames[f].information[j].substr(x,getNatributes("schema.txt")),"schema.txt"))),"schema.txt");
                    while(x + k < directory->qSec)
                    {
                        if(frames[f].information[j][x] == ' ')
                            while(frames[f].information[j][x] == ' ' && frames[f].information[j][x] != '\0' && x < directory->qSec)
                                x++;
                        if(x >= frames[f].information[j].size())
                            break;
                        k = getBytes4RegistWB(frames[f].information[j].substr(x,(getNatributes("schema.txt")+2*getMaxStr("schema.txt")*countStrBitmap(frames[f].information[j].substr(x,getNatributes("schema.txt")),"schema.txt"))),"schema.txt");
                        string r = frames[f].information[j].substr(x,k);
                        cout<<r<<endl;
                        cout<<getIDdynamicRegist(r,"schema.txt")<<endl;
                        if(id == getIDdynamicRegist(r,"schema.txt"))
                        {
                            block = f;
                            sector = j;
                            waos = 1;
                            cout<<block<<"  "<<sector<<"  "<<endl;
                            break;
                        }
                        x += k;
                        cout<<x<<endl;
                    }
                    if(waos)
                        break;
                }
                if(waos)
                    break;
            }
            
            frames[block].pinCount++;
            string sect;
            int x = 0;
            if(frames[block].information[sector][x] == ' ')
            {
                while(frames[block].information[sector][x] == ' ' && x < directory->qSec)
                {
                    sect += " ";
                    x++;
                }
            }
            int k = getBytes4RegistWB(frames[block].information[sector].substr(x,(getNatributes("schema.txt")+2*getMaxStr("schema.txt")*countStrBitmap(frames[block].information[sector].substr(x,getNatributes("schema.txt")),"schema.txt"))),"schema.txt");
            while(x + k < directory->qSec)
            {
                if(frames[block].information[sector][x] == ' ')
                {
                    while(frames[block].information[sector][x] == ' ' && frames[block].information[sector][x] != '\0' && x < directory->qSec)
                    {
                        sect += " ";
                        x++;
                    }
                }
                if(x >= frames[block].information[sector].size())
                    break;
                k = getBytes4RegistWB(frames[block].information[sector].substr(x,(getNatributes("schema.txt")+2*getMaxStr("schema.txt")*countStrBitmap(frames[block].information[sector].substr(x,getNatributes("schema.txt")),"schema.txt"))),"schema.txt");
                string r = frames[block].information[sector].substr(x,k);
                if(id != getIDdynamicRegist(r,"schema.txt"))
                    sect += r; 
                else
                {
                    string str;
                    for(int i = 0; i<r.size(); i++)
                        str += " ";
                    sect += str;
                    str = "\n" + to_string(frames[block].pageID) + "," + to_string(sector) + "," + to_string(x) + "," + to_string(str.size());
                    int f = upFrame(1);
                    frames[f].information[0] += str;
                    frames[f].dirty[0] = 1;
                    frames[f].reDisc(directory);
                }
                x += k;
            }
            cout<<"hola";
            frames[block].information[sector] = sect;
            frames[block].dirty[sector] = 1;
            frames[block].reDisc(directory);
            frames[block].pinCount--;
        }
};