#include <iostream>
#include <string>
//#include "headers/Bloque_system.h"
#include "headers/Disc.h"
#include "headers/file_function.h"
#include "headers/csv_file.h"
using namespace std;
int main ()
{
    bool full = 1;

    bool dynamic;
    string csvname;
    cout<<"Ingrese el nombre del .csv (.txt u otros) a usar (Incluya la extension): ",cin>>csvname;
    cout<<"Ingrese el modo de lectura ( 0 = estatico , 1 = dinamico ) : ",cin>>dynamic;

    transform_csv("titanic.csv",dynamic);

    int nRegist = 1;
    while(nRegist != 0)
    { 
        cout<<"\nIngrese el numero de registro que desea analizar (Termine con 0): ",cin>>nRegist;
        if(nRegist == 0)    break;
        printRegist(nRegist,"schema.txt");
        cout<<"\nCantidad de Bytes en el registro: "<<getBytes4EachRegist("schema.txt");
    }
    cout<<"\nCantidad de Bytes en el file: "<<getBytes4EachRegist("schema.txt")*891;
    nRegist = 1;

    cout<<endl<<endl;
    
    int * spaces = new int [3];
    int Plates, Tracks, Sectors; 
    cout<<"\nDISCO MAGNETICO\n";
    cout<<"\nPersonaliza tu disco :D\nIngrese la cantidad de Platos: ",cin>>spaces[0];
    cout<<"Ingrese la cantidad de Pistas por Superficie: ",cin>>spaces[1];
    cout<<"Ingrese la cantidad de Sectores (7140 Bytes por sector) por Pista: ",cin>>spaces[2];
    Disc disco(spaces[0],spaces[1],spaces[2]);
    delete [] spaces;
    disco.metaDiscData();
    cout<<"\nHora de ingresar nuestros datos :D\n";

    ifstream readerSchema("schema.txt");   
    ifstream reader("file.txt");
    string receptor;
    if(full)
    {
        while(getline(readerSchema,receptor) && !readerSchema.eof())
        {
            disco.insertData<string>(receptor);
            cout<<receptor;
        }cout<<"\nESQUEMA INGRESADO\n";
        disco.makeSectorReserved(1,1,1,1);
        while(getline(reader,receptor) && !reader.eof())
        {
            disco.insertData<string>(receptor);
            cout<<receptor;
        }
    }
    cout<<"\nDB Titanic ingresada\n";

    disco.metaDiscData();
    cout<<"\n---------------------------------\nHora de imprimir nuestros datos :D\n\n";
    spaces = new int;
    while(*spaces != 0)
    {
        cout<<"\n\nIngrese el numero de sector que desea imprimir (0 para terminar): ",cin>>*spaces;
        if(*spaces == 0){break;}
        disco.printSector(1,1,1,*spaces);
    }*spaces = 1;
    /*
    Bloque * directory;
    int cap;
    Bloque * temp;
    cout<<"\nIngrese cuantos sectores x bloque necesita: ",cin>>cap;
    for(int  i = 0; i<disco.getTotalSectors()/cap; i++)
    {
        if(i == 0)
        {
            directory = new Bloque(cap,i+1);
            temp = directory;
        }
        else
        {
            temp->nextBloque = new Bloque(cap,i+1);
            temp = temp->nextBloque;
        }
    }
    temp = directory;
    for(int  i = 0; i<disco.getTotalSectors()/cap; i++)
    {
        vector<int> list = disco.processValues(1,1,1,1+cap*(i));
        temp->insertSector(list[0],list[1],list[2],list[3],disco.getQSector());
        temp = temp->nextBloque;
    }
    temp = directory;
    *spaces = -1;
    while(*spaces != 0)
    {
        cout<<"\nIngrese el numero de Bloque que desea imprimir: ",cin>>*spaces;
        if(*spaces == 0){break;}
        for(int i = 0; i<*spaces-1;i++)
            temp = temp->nextBloque;
        temp->printBloque();
        temp = directory;
    }
    delete directory;*/
    delete spaces;
}