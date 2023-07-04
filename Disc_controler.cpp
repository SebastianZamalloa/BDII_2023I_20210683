#include <iostream>
#include <string>
#include "headers/Bloque.h"
using namespace std;
int main ()
{
    bool full = 1;

    bool dynamic;
    string csvname;
    //cout<<"Ingrese el nombre del .csv (.txt u otros) a usar (Incluya la extension): ",cin>>csvname;
    csvname = "titanic.csv";
    cout<<"Ingrese el modo de lectura ( 0 = estatico , 1 = dinamico ) : ",cin>>dynamic;
    transform_csv(csvname,dynamic);
    
    int option = -1;
    
    while(option != 0)
    {
        cout<<"\nIngrese la opcion que necesite a nivel del file: \n1.Ver informacion de un registro\n2.Insertar registro\n3.Eliminar registro\nInserte: ",cin>>option;
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
                    printRegist(nRegist,"schema.txt",dynamic);
                    cout<<"\nCantidad de Bytes en el registro: "<<getBytes4EachRegist("schema.txt",dynamic,nRegist);
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
                    cout<<transform_line(result,dynamic);
                    insertRegist(transform_line(result,dynamic),"file.txt",dynamic);
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
                    deleteRegist("file.txt",count,dynamic);
                    cout<<"\nIngrese 0 si quiere terminar: ",cin>>count;
                    if(count == 0)
                        break;
                }
            }break;
        }
    }
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
        getline(reader,receptor);
        disco.insertData<string>("\n"+receptor);
        disco.makeSectorReserved(1,1,1,1);
        while(!reader.eof())
        {
            getline(reader,receptor);
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


    Bloque * directory;
    mkdir("d:/UNSA/BD II/Disco - BD - V3/Bloques");
    int cap;
    int cT = disco.getSectorByte()*disco.getTotalSectors();
    Bloque * temp;
    cout<<"\nIngrese los bytes x bloque: ",cin>>cap;
    for(int  i = 0; i<(cT/cap)+1; i++)
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
    for(int  i = 1; i<=disco.getTotalSectors(); i++)
    {
        vector<int> list = disco.processValues(1,1,1,i);
        directory->insertSector(list[0],list[1],list[2],list[3]);
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
    delete directory;
    reader.close();
    remove("file.txt");
    delete spaces;
}