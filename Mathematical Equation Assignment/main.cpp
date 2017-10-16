#include <iostream>
#include <apstring.h>



int main(){
    apstring str;
    char operation;
    int n[2];
    str = "3 + 9";


    int firstSpace = str.find(" ");

    int secondSpace = substr(firstSpace+1, str.length()-firstSpace);

    str.c_str();
    for(int i = 0; i < str.length(); i++){
        if(i = firstSpace)
        continue;
        else if (i = secondSpace)
        continue;

        switch(str[i]){

        case '+': operation = '+';
                  break;

        case '-': operation = '-';
                  break;

        default: break;

        }

    }

}
