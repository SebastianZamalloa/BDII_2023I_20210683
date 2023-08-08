#include <iostream>
#include <string>
#include <chrono>
#include "headers/Buffer.h"
using namespace std;

string exportLeafDataToString(BPlusTree<regist_direction>& tree) {
    ostringstream oss;
    Node<regist_direction>* cursor = tree.getroot();
    while (cursor != nullptr && !cursor->is_leaf) {
        cursor = cursor->children[0];
    }

    bool firstItem = true;
    while (cursor != nullptr) {
        for (size_t i = 0; i < cursor->size; ++i) {
            if (!firstItem) {
                oss << "\n";
            }
            oss << to_string(cursor->item[i].id) <<"#"<< to_string(cursor->item[i].bloq_num) <<"#"<< to_string(cursor->item[i].sec_num) <<"#"<< to_string(cursor->item[i].space);
            firstItem = false;
        }
        cursor = cursor->children[cursor->size];
    }

    return oss.str();
}
BPlusTree<regist_direction> buildTreeFromExportedData(string& exportedData, size_t degree) {
    istringstream iss(exportedData);
    string line;
    BPlusTree<regist_direction> tree(degree);

    while (getline(iss, line)) {
        istringstream line_stream(line);
        regist_direction item;
        string ph;
        while (line_stream >> ph) 
        {
            istringstream elementstream(ph);
            string word;
            for(int i = 0; i<4; i++)
            {
                getline(elementstream,word,'#');
                switch(i)
                {
                    case 0:{item.id = stoi(word);}break;
                    case 1:{item.bloq_num = stoi(word);}break;
                    case 2:{item.sec_num = stoi(word);}break;
                    case 3:{item.space = stoi(word);}break;
                }                
            }
            tree.insert(item);
        }
    }
    return tree;
}

int main ()
{
    bool full = 1;
    bool dynamic = 1;
    bool file_charger = 0;

    string csvname = "movies.csv";
    if(file_charger)
        transform_csv(csvname,dynamic);

    cout<<"\nCantidad de Bytes en el file: "<<getFileBytes("file.txt");
    cout<<endl<<endl;
    
    int * spaces = new int [4];
    int Plates, Tracks, Sectors; 
    cout<<"\nDISCO MAGNETICO\n";
    cout<<"\nPersonaliza tu disco :D\nIngrese la cantidad de Platos: ",cin>>spaces[0];
    cout<<"Ingrese la cantidad de Pistas por Superficie: ",cin>>spaces[1];
    cout<<"Ingrese la cantidad de Sectores por Pista: ",cin>>spaces[2];
    cout<<"Ingrese la cantidad de bytes por Sector: ",cin>>spaces[3];
    Disc disco(spaces[0],spaces[1],spaces[2],spaces[3]);
    int byteSec = spaces[3];
    delete [] spaces;

    BPlusTree<regist_direction> bpt(10);

    Bloque * directory;
    int cap;
    int cT = disco.getSectorByte()*disco.getTotalSectors();
    Bloque * temp;
    cout<<"\nIngrese los bytes x bloque: ",cin>>cap;
    for(int  i = 0; i<(cT/cap)+1; i++)
    {
        cout<<i<<endl;
        if(i == 0)
        {
            directory = new Bloque(cap,i+1,disco.getSectorByte(),&disco);
            temp = directory;
        }
        else
        {
            temp->nextBloque = new Bloque(cap,i+1,disco.getSectorByte(),&disco);
            temp = temp->nextBloque;
        }
    }
    for(int  i = 1; i<=disco.getTotalSectors(); i++)
        directory->insertSector(i);
    int capSec = cap/byteSec;
    disco.metaDiscData();
    cout<<"\nHora de ingresar nuestros datos :D\n";

    ifstream readerSchema("schema.txt");   
    ifstream reader("file.txt");
    string receptor;
    if(full)
    {
        while(getline(readerSchema,receptor) && !readerSchema.eof())
        {
            vector<int>sec = {0,0,0,0};
            disco.insertData<string>(receptor,sec);
            cout<<receptor;
        }cout<<"\nESQUEMA INGRESADO\n";
        disco.makeSectorReserved(1,1,1,1);
        while(getline(reader,receptor))
        {
            vector<int>sec = {0,0,0,0};
            disco.insertData<string>(receptor, sec);
            int nSEC = disco.inverseValues(sec[0],sec[1],sec[2],sec[3]);
            regist_direction temp(getIDdynamicRegist(receptor,"schema.txt"),(nSEC/capSec)+1,nSEC-(nSEC/capSec)*capSec,1);
            bpt.insert(temp);
        }
        
    }
    cout<<endl;
    bpt.bpt_print();
    cout<<"\nDB Titanic ingresada\n";

    disco.metaDiscData();
    cout<<"\n---------------------------------\nHora de imprimir nuestros datos :D\n\n";
    spaces = new int;
    while(*spaces != 0)
    {
        cout<<"\n\nIngrese el numero de sector que desea imprimir (0 para terminar): ",cin>>*spaces;
        if(*spaces == 0){break;}
        auto start = std::chrono::high_resolution_clock::now();
        disco.printSector(1,1,1,*spaces);
        auto end = chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> float_ms = end - start;
        auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        cin.ignore();
        cout<<"\nTIEMPO DE EJECUCIÓN: "<<float_ms.count()<<endl;
    }*spaces = 1;

    BufferManager manager(5,directory,1);
    int option = -1;
    
    while(option != 0)
    {
        cout<<"\nIngrese la opcion que necesite: \n1.Ver informacion de un registro\n2.Insertar registro\n3.Eliminar registro\nInserte: ",cin>>option;
        if(option == 0)
            break;
        switch(option)
        {
            case 1:
            {
                int nRegist = -1;
                while(nRegist != 0)
                { 
                    cout<<"\nIngrese el numero de registro que desea analizar (Termine con 0): ",cin>>nRegist;
                    if(nRegist == 0)    break;
                    manager.look4Regist(nRegist);
                }
            }break;
            case 2:
            {
                while(true)
                {
                    string result;
                    int count = 0;
                    ifstream schema("schema.txt");
                    string schatribute;
                    while(getline(schema,schatribute))
                    {
                        count++;
                        istringstream eachAtribute(schatribute);
                        string woword;
                        getline(eachAtribute,woword,'#');
                        string atribute = woword;
                        getline(eachAtribute,woword,'#');
                        cout<<"Ingrese el siguiente atributo \""<<atribute<<"\" ("<<woword<<"): ";;
                        string r;
                        if(woword == "str")
                            getline(cin,r);
                        else
                        {
                            cin>>r;
                            cin.ignore();
                        }
                        if(getNatributes("schema.txt") == count)
                            result+= r; 
                        else
                            result += r + "#";
                    }
                    cout<<result<<endl;
                    cout<<transform_line(result,dynamic)<<endl;
                    manager.insertRegist(transform_line(result,dynamic),dynamic,bpt);
                    cout<<"\nIngrese 0 si quiere terminar: ",cin>>count;
                    if(count == 0)
                        break;
                }
                
            }break;
            case 3:
            {
                int count;
                while(true)
                {
                    cout<<"\nIngrese la posicion de registro que quiere eliminar: ",cin>>count;
                    if(count == 0)
                        break;
                    manager.deleteRegist(count,dynamic,bpt);
                }
            }break;
        }
    }
    delete directory;
    reader.close();
    
    //remove("file.txt");
    delete spaces;
}