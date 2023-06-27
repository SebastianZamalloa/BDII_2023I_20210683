#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include <limits.h>
#include <bitset>
using namespace std;

std::string toBinary(int decimal)
{
    int binary = 0, remainder, product = 1;
    while (decimal != 0) {
    remainder = decimal % 2;
    binary = binary + (remainder * product);
    decimal = decimal / 2;
    product *= 10;
  }
  return to_string(binary);

}
int Binary2Hex( std::string Binary )
{
    std::bitset<32> set(Binary);      
    int hex = set.to_ulong();
     
    return hex;
}
 
// Convert the 32-bit binary into the decimal
float GetFloat32( std::string Binary )
{
    int HexNumber = Binary2Hex( Binary );
 
    bool negative  = !!(HexNumber & 0x80000000);
    int  exponent  =   (HexNumber & 0x7f800000) >> 23;    
    int sign = negative ? -1 : 1;
 
    // Subtract 127 from the exponent
    exponent -= 127;
 
    // Convert the mantissa into decimal using the
    // last 23 bits
    int power = -1;
    float total = 0.0;
    for ( int i = 0; i < 23; i++ )
    {
        int c = Binary[ i + 9 ] - '0';
        total += (float) c * (float) pow( 2.0, power );
        power--;
    }
    total += 1.0;
 
    float value = sign * (float) pow( 2.0, exponent ) * total;
 
    return value;
}
 
// Get 32-bit IEEE 754 format of the decimal value
std::string GetBinary32( float value )
{
    union
    {
         float input;   // assumes sizeof(float) == sizeof(int)
         int   output;
    }    data;
 
    data.input = value;
 
    std::bitset<sizeof(float) * CHAR_BIT>   bits(data.output);
 
    std::string mystring = bits.to_string<char, 
                                          std::char_traits<char>,
                                          std::allocator<char> >();
 
    return mystring;
}
 //filesystem::remove_all("Disk");
int main()
{
    /*
    ifstream titanic("titanic.csv");
    string word;
    ofstream file;
    file.open("titanic1.txt", fstream::app);

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
        file << '#'; 
    }
    file.close();*/
    ifstream titanic("titanic1.txt");
    string word;
    ofstream file;
    file.open("titanic2.txt", fstream::app);
    int cont = 1;
    while(getline(titanic,word,'#') && !titanic.eof())
    {
        cout<<word<<endl;
        switch(cont)
        {
            case 2:
            {
                file << word;
                cont++;
            }break;
            case 1:
            case 3:
            case 7:
            case 8:
            {
                if(word.length() == 0)
                {
                    for(int i=0;i<32;i++)
                    {
                        file << "0";
                    }
                    cont++;
                }
                else
                {
                    for(int i=0;i<32-(toBinary(stoi(word))).length();i++)
                    {
                        file << "0";
                    }
                    file << toBinary(stoi(word));
                    cont++;
                }
            }break;
            case 4:
            {
                file << word;
                for(int i=0;i<100-word.length();i++)
                    file <<" ";
                cont++;
            }break;
            case 5:
            {
                file << word;
                if(word != "female")
                    file <<"  ";
                cont++;
            }break;
            case 6:
            case 10:
            {
                if(word.length() == 0)
                {
                    for(int i=0;i<32;i++)
                    {
                        file << "0";
                    }
                    cont++;
                }
                else
                {
                    for(int i=0;i<32-(GetBinary32(stof(word))).length();i++)
                    {
                        file << "0";
                    }
                    file << GetBinary32(stof(word));
                    cont++;
                }
            }break;
            case 9:
            {
                file << word;
                for(int i=0;i<25-word.length();i++)
                    file <<" ";
                cont++;
            }break;
            case 11:
            {
                file << word;
                for(int i=0;i<20-word.length();i++)
                    file <<" ";
                cont++;
            }break;
            case 12:
            {
                file << word;
                cont = 1;
            }break;
        }
        file<<"#";
    }
    file.close();
}