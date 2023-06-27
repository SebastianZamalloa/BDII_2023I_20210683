#include<iostream>
#include<fstream>
#include<string>
#include <sstream>
using namespace std;

int main()
{
    ifstream titanic("titanic_transformed.txt");
    string word;
    if(getline(titanic,word))
    {
        istringstream prouve(word);
        string w;
        while(getline(prouve,w,'#'))
            cout<<w<<endl;
    }

}