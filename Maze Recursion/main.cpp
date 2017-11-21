/*****************************************************************************
 *	Name: Matthew Wang                                                       *
 *	Course: ICS4U                                                            *
 *	Date: November 16, 2017                                                  *
 *	                                                                         *
 *	Purpose: Recursive stuff                                                 *
 *	                                                                         *
 *	Usage:                                                                   *
 *	                                                                         *
 *	Revision History:                                                        *
 *	                                                                         *
 *	Known Issues:                                                            *
 *	                                                                         *
 *****************************************************************************/

#include <iostream>
#include <apstring.h>
#include <apstring.cpp>

using namespace std;

apstring binary(int n);
long fibonacci(int n);

int main() {
    apstring result = "";
    int num;
    long res;
    char cmd;

    cout << endl << "\tICS4U Recursion" << endl;
    cout << "\tRecursion programs " << endl;

    while (true) {  // Repeat (until break)

        // Show the menu and prompt:

        cout << "\n";  // Output a blank line

        cout << "\tOptions:\n\n";
        cout << "\t(B)inary conversion from Decimal number\n";      // '\t' is tab
        cout << "\t(F)ibonnaci number position\n";
        cout << "\t(Q)uit\n";
        cout << endl;
        cout << "Next command ==> ";

        // Accept command:

        cin >> cmd;             // Read one char.
        cin.ignore(80, '\n');   // Skip remaining input (up to 80
                                //   chars) to the end of the line.
        cmd = toupper(cmd);     // Convert letter to upper case
                                //   to allow lower case input
                                //   for commands (for convenience).

        // Quit if 'Q'

        if (cmd == 'Q')
            break;                  // Quit processing commands

        cout << "\n\n****************************************\n";

        // Process command:

        switch (cmd) {

          case 'B':           // Binary conversion

            cout << "Decimal number: ";
            cin >> num;
            result = binary(num);
            cout << result << endl;

            break;

          case 'F':           // Fibonacci sequence term tracker

            cout << "Fibonacci number: ";
            cin >> num;
            res = fibonacci(num);
            cout << "Fibonacci number for the "
                 << num;
            switch (abs(num % 10)) {

            case 1: cout << "st";
                    break;

            case 2: cout << "nd";
                    break;

            case 3: cout << "rd";
                    break;

            default: cout << "th";
                     break;

            }
            cout << " term is: " << res << endl;

            break;

        }

        cout << "****************************************\n";
    }



    cout << result << endl;


}

//Converts decimal number to binary number
apstring binary(int n)
{

    apstring binaryNum = "";

    if(n == 1){
        binaryNum = '1' + binaryNum;
    }
    else if (n == 0) {
        binaryNum = '0' + binaryNum;
    }
    else if(!(n <= 1) && n % 2 == 0){
        cout << n << endl;
        binaryNum = binary(n/2) + '0';

    }
    else if (!(n <= 1) && n % 2 == 1) {
        cout << n << endl;
        binaryNum = binary(n/2) + '1';

    }

    return binaryNum;
}

//Returns nth term of the fibonacci sequence
long fibonacci(int n)
{
    long number;

    if(n <= 0)
        return 0;
    else if(n < 3)
        return 1;

    number = fibonacci(n-1) + fibonacci(n-2);
    return number;
}


