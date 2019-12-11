#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main()
{
    string a ="abcdefg";
    cout << a ;
    cout << endl;

    vector<string> strArray(10);
    strArray[0] = "hello";
    strArray[1] = "world";
    strArray[2] = "this";
    strArray[3] = "find";
    strArray[4] = "gank";
    strArray[5] = "pink";
    strArray[6 ]= "that";
    strArray[7] = "when";
    strArray[8] = "how";   
    strArray[9] = "cpp";


    for (int i=0; i<10 ; i++)
    {
        cout << strArray[i] << " ";
        cout << endl;
    }
    
}