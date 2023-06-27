#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;

int getNatributes(string name)
{
    ifstream esquema(name);
    string word;
    int counter = 0;
    while(getline(esquema,word))
        counter++;
    return counter;
}

int getNStrAtributes(string name)
{
    ifstream esquema(name);
    string word;
    int counter = 0;
    while(getline(esquema,word))
    {
        istringstream atribute(word);
        string num;
        getline(atribute,num,'#');getline(atribute,num,'#');
        if(num == "str")
        {
            counter++;
            getline(atribute,num,'#');
        }
            
    }
    return counter;
}

int getMaxStr(string name)
{
    ifstream esquema(name);
    string word;
    int max = 0;
    while(getline(esquema,word))
    {
        istringstream atribute(word);
        string num;
        getline(atribute,num,'#');getline(atribute,num,'#');
        if(num == "str")
        {
            getline(atribute,num,'#');
            if(stoi(num) >= max)
                max = stoi(num);
        }
    }
    return to_string(max).length();
}

int getHeadersByte(string name)
{
    ifstream esquema(name);
    string word;
    int counter = 0;
    while(getline(esquema,word))
    {
        istringstream atribute(word);
        string kb;
        getline(atribute,kb,'#');getline(atribute,kb,'#');
        if(kb == "int64" || kb == "float64")
            counter += 8;
        else if(kb == "bool")
            counter += 1;
        else if(kb == "str")
        {
            getline(atribute,kb,'#');
            counter += 6;
        }
    }
    return counter;
}

int main()
{ 
    ifstream titanic("titanic.csv");
    string word;
    ofstream file;
    file.open("titanic_transformed.txt", fstream::app);

    while(getline(titanic,word))
    {
        bool state = 0;
        bool doublecom = 0;
        for(int i = 0; i<word.length();i++)
        {
            
            if(word[i] == '"' && !doublecom)
            {
                
                if(word[i+1] == '"')
                {
                    file << word[i+1];
                    doublecom = 1;
                }
                else
                    state = !state;
            }
            else if(!state)
            {
                if(word[i] == ',')
                    file << '#';
                else
                    file << word[i];
            }
            else
            {
                if(doublecom)
                    doublecom = 0;
                else
                    file << word[i];
            }
        }
        file << '\n'; 
    }
    file.close();
    
    ifstream titanic_("titanic_transformed.txt");
    string word_;
    ofstream file_;
    file_.open("file.txt", fstream::app);
    string all_regist;

    while(getline(titanic_,word_))
    {
        string bitmap;
        string offset;
        string static_var;
        string length_var;

        ifstream schema("schema.txt");
        string schatribute;
        istringstream regist(word_);
        string atribute;

        while(getline(regist,atribute,'#'))
        {
            cout<<atribute<<endl;
            getline(schema,schatribute);
            istringstream eachAtribute(schatribute);
            string woword;
            if(atribute.length() != 0)
            {
                bitmap += "0";
                getline(eachAtribute,woword,'#');getline(eachAtribute,woword,'#');
                cout<<woword<<endl;
                if(woword == "bool")
                    static_var += atribute;
                else if(woword == "int64" || woword == "float64")
                {
                    for(int i = 0; i<8-atribute.length(); i++)
                        static_var += "0";
                    static_var += atribute;
             
                }
                else if(woword == "str")
                {
                    getline(eachAtribute,woword,'#');
                    if(stoi(woword) >= atribute.length())
                    {
                        length_var += atribute;
                        for(int i = 0; i<getMaxStr("schema.txt"); i++)
                            offset += "-";
                        for(int i = 0; i<getMaxStr("schema.txt")-to_string(atribute.length()).length(); i++)
                            offset += "0";
                        offset += to_string(atribute.length());
                    }
                    else bitmap += "1";
                }    
            }
            else
                bitmap += "1";
        }
        for(int i = 0; i<getNStrAtributes("schema.txt"); i++)
        {
            string len = to_string(offset.length() + static_var.length() + bitmap.length() + i*(2*getMaxStr("schema.txt")));
            for(int j = 0; j<getMaxStr("schema.txt")-len.length(); j++)
                len = "0" + len;
            for(int j = 0; j<getMaxStr("schema.txt"); j++)
                offset[i*(2*getMaxStr("schema.txt"))+j] = len[j];
        }
        file_<<offset<<static_var<<bitmap<<length_var<<"\n";


    }
    file_.close();
}