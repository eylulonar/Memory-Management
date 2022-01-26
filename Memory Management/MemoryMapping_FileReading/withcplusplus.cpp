#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const char FileName[] = "loremipsum.txt";

int main () 
{
char ch;
ifstream MyFile ("loremipsum.txt"); 
int count=0;

if (MyFile.is_open()) 
{

     while(MyFile.get(ch)){
		 
		 if(ch== 'a'){

            count++;
		    
  }
    }
	cout<< "The occurence of character 'a' is " << count << " times." <<endl;
}
    MyFile.close(); 


   return 0;
}