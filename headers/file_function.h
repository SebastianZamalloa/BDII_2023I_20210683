#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

int getBytes4EachRegist(string name)
{
    int count = 0;
    ifstream schema(name);
    string schatribute;
    while(getline(schema, schatribute))
    {
        istringstream eachAtribute(schatribute);
        string woword;
        getline(eachAtribute,woword,'#');getline(eachAtribute,woword,'#');
        if(woword == "bool")
            count += 2;
        else if(woword == "int64" || woword == "float64")
            count += 9;
        else if(woword == "str")
        {
            getline(eachAtribute,woword,'#');
            count += stoi(woword) + 1;
        }
    }
    return count;
}

void printRegist(int nRegist, string schema)
{
    fstream file_reader("file.txt");
    int size = getBytes4EachRegist(schema);
    file_reader.seekg((nRegist-1)*(size+2),ios::beg);
    char * buffer = new char [size];
    file_reader.read(buffer,size);
    cout.write(buffer,size);
    file_reader.close();
    delete [] buffer;
}