#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;

int getFileBytes(string name)
{
    ifstream esquema(name);
    string word;
    int counter = 0;
    while(getline(esquema,word))
        counter += word.size();
    return counter;
}

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

string getLineFile(string name, int nLine)
{
    ifstream esquema(name);
    string word;
    for(int i = 0; i<=nLine; i++)
        getline(esquema,word);
    return word;
}

string transform_line(string word_, bool dynamic)
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
            getline(schema,schatribute);
            istringstream eachAtribute(schatribute);
            string woword;
            if(atribute.length() != 0 || !dynamic)
            {
                if(dynamic)
                    bitmap += "0";
                getline(eachAtribute,woword,'#');getline(eachAtribute,woword,'#');
                if(woword == "bool")
                {
                    static_var += atribute;
                    if(!dynamic && atribute.length() == 0)
                        static_var += " ";
                }
                else if(woword == "int64" || woword == "float64")
                {
                    for(int i = 0; i<8-atribute.length(); i++)
                        static_var += "0";
                    static_var += atribute;
                }
                else if(woword == "str")
                {
                    getline(eachAtribute,woword,'#');
                    if(!dynamic)
                    {
                        if(atribute.length() <= stoi(woword))
                        {
                            static_var += atribute;
                            for(int i=0;i<stoi(woword)-atribute.length();i++)
                                static_var += " ";
                        }
                        else
                        {
                            for(int i=0;i<stoi(woword);i++)
                                static_var += atribute[i];
                        }  
                    }
                    else
                    {
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
            }
            else
                bitmap += "1";
            if(!dynamic)
                static_var += "#";
        }
        if(dynamic)
        {
            for(int i = 0; i<getNStrAtributes("schema.txt"); i++)
            {
                string len = to_string(offset.length() + static_var.length() + bitmap.length() + i*(2*getMaxStr("schema.txt")));
                for(int j = 0; j<getMaxStr("schema.txt")-len.length(); j++)
                    len = "0" + len;
                for(int j = 0; j<getMaxStr("schema.txt"); j++)
                    offset[i*(2*getMaxStr("schema.txt"))+j] = len[j];
            }
        }    
    return bitmap + offset + static_var + length_var;
}

void transform_csv(string name, bool dynamic)
{
    ifstream titanic(name);
    string word;
    ofstream file;
    file.open("transformed.txt", fstream::app);
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
        file << "#\n"; 
    }
    file.close();
    
    ifstream titanic_("transformed.txt");
    string word_;
    ofstream file_;
    file_.open("file.txt", fstream::app);
    string all_regist;
    int count = 0;
    file_<<"\n";

    while(getline(titanic_,word_))
    {
        count++;
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
            getline(schema,schatribute);
            istringstream eachAtribute(schatribute);
            string woword;
            if(atribute.length() != 0 || !dynamic)
            {
                if(dynamic)
                    bitmap += "0";
                getline(eachAtribute,woword,'#');getline(eachAtribute,woword,'#');
                if(woword == "bool")
                {
                    static_var += atribute;
                    if(!dynamic && atribute.length() == 0)
                        static_var += " ";
                }
                else if(woword == "int64" || woword == "float64")
                {
                    for(int i = 0; i<8-atribute.length(); i++)
                        static_var += "0";
                    static_var += atribute;
                }
                else if(woword == "str")
                {
                    getline(eachAtribute,woword,'#');
                    if(!dynamic)
                    {
                        if(atribute.length() <= stoi(woword))
                        {
                            static_var += atribute;
                            for(int i=0;i<stoi(woword)-atribute.length();i++)
                                static_var += " ";
                        }
                        else
                        {
                            for(int i=0;i<stoi(woword);i++)
                                static_var += atribute[i];
                        }  
                    }
                    else
                    {
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
            }
            else
                bitmap += "1";
            if(!dynamic)
                static_var += "#";
        }
        if(dynamic)
        {
            for(int i = 0; i<getNStrAtributes("schema.txt"); i++)
            {
                string len = to_string(offset.length() + static_var.length() + bitmap.length() + i*(2*getMaxStr("schema.txt")));
                for(int j = 0; j<getMaxStr("schema.txt")-len.length(); j++)
                    len = "0" + len;
                for(int j = 0; j<getMaxStr("schema.txt"); j++)
                    offset[i*(2*getMaxStr("schema.txt"))+j] = len[j];
            }
        }    
        file_<<bitmap<<offset<<static_var<<length_var;
        if(count != getNatributes(name))
            file_<<"\n";
    }
    file_.close();
    titanic_.close();
    remove("transformed.txt");
}

int getBytes4EachRegist(string name, bool dynamic, int nRegist = 0)
{
    int count = 0;
    if(!dynamic)
    {
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
    }
    else
    {
        ifstream schema("file.txt");
        string schatribute;
        for(int i = 0;i<=nRegist;i++)
            getline(schema, schatribute);
        string bitmap = schatribute.substr(0,getNatributes(name));

        ifstream s(name);
        string sch;

        int offset = getNatributes(name);

        for(int i = 0;i<getNatributes(name);i++)
        {
            getline(s, sch);
            if(bitmap[i]-'0' == 0)
            {
                istringstream eachAtribute(sch);
                string woword;
                getline(eachAtribute,woword,'#');getline(eachAtribute,woword,'#');
                if(woword == "bool")
                    count += 1;
                else if(woword == "int64" || woword == "float64")
                    count += 8;
                else if(woword == "str")
                {
                    getline(eachAtribute,woword,'#');
                    string cont = getLineFile("file.txt",nRegist).substr(offset+getMaxStr("schema.txt"),getMaxStr("schema.txt"));
                    offset += getMaxStr("schema.txt")*2;
                    count += stoi(cont) + getMaxStr("schema.txt")*2;
                }
            }
        }
        count += getNatributes(name);
    }
    return count;
}

void printRegist(int nRegist, string schema, bool dynamic)
{
    if(!dynamic)
    {
        fstream file_reader("file.txt");
        string header;
        getline(file_reader,header);

        int size = getBytes4EachRegist(schema,dynamic);
        file_reader.seekg(header.size()+2+(nRegist-1)*(size+2),ios::beg);

        char * buffer = new char [size];
        file_reader.read(buffer,size);
        cout.write(buffer,size);
        file_reader.close();
        delete [] buffer;
    }
    else
    {
        ifstream file_reader("file.txt");
        int count = -1;
        string line;
        while(getline(file_reader,line))
        {
            count++;
            if(count == nRegist)
                cout<<line;
        }
    }
}

void insertRegist(string content, string name, bool dynamic)
{
    ifstream titanic(name);
    string word;
    string every;
    string header;
    
    int count = -1;
    int num = 0;
    int i = 0;
    while(getline(titanic,word))
    {
        count++;
        if(count == 0)
        {
            {
                istringstream look(word);
                string line;
                while(look>>line)
                {
                    if(line != "")
                    {
                        num = stoi(line);
                        while(look>>line)
                            header += to_string(stoi(line)-4) + " ";
                    }
                }
                int x = num;
                while(x != 0)
                {
                    x -= getLineFile(name,i).size();
                    i++;
                }
            }
        }
        else if(count == i)
        {
            if(content.size() <= getLineFile(name,i).size())
            {
                every += content;
                    for(int j=0;j<getLineFile(name,i).size()-content.size();j++)
                every += " ";
                every += "\n";
            }
            else
                i = 0;
        }
            
        else
        {
            every += word;
            if(count != getNatributes(name)-1)
                every += "\n";
        }
    }
    if(i == 0)
        every += "\n" + content;
    ofstream file_;
    file_.open("file2.txt", fstream::app);
    file_<<header<<"\n"<<every;
    file_.close();
    titanic.close();
    remove(name.c_str());
    rename("file2.txt",name.c_str());
}

void deleteRegist(string name, int num,bool dynamic)
{
    if(num > getNatributes(name)-1)
    {
        cout<<"\nEliminacion invalida\n";
        return;
    }
    ifstream titanic(name);
    string word;
    string every;
    string header;
    
    int count = -1;
    while(getline(titanic,word))
    {
        count++;
        if(count == 0)
        {
            header = word;
            istringstream look(word);
            string line;
            while(look>>line)
            {
                if(stoi(line) == num)
                    cout<<"\nEliminacion invalida\n";
            }
        }
        else if(count == num)
        {
            for(int i = 0;i<getBytes4EachRegist("schema.txt",count,num);i++)
                every += " ";
            int x = 0;
            if(dynamic)
            {
                cout<<"aaaaa";
                for(int i = 1; i<num; i++)
                {
                    x += getBytes4EachRegist("schema.txt",dynamic,i);
                }
                x+= header.size() + 1 + to_string(x).size();
                header += to_string(x) + " ";
                every += "\n";
            }
            else
            {
                header += to_string(header.size() + getBytes4EachRegist("schema.txt",dynamic)*count) + " ";
                every += "\n";
            }
        }
        else
        {
            every += word;
            if(count != getNatributes(name)-1)
                every += "\n";
        }
    }
    ofstream file_;
    file_.open("file2.txt", fstream::app);
    file_<<header<<"\n"<<every;
    file_.close();
    titanic.close();
    remove(name.c_str());
    rename("file2.txt",name.c_str());
}