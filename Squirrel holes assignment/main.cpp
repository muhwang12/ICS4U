#include <iostream>
#include <apvector.h>

using namespace std;

void countSquirrels(int h[], int tempArr[], int pos);
bool checkDone(int h[]);

int main() {
    int holes[5] = {0};
    bool done = false;
    int numDays = 0;

    holes[0] = 6;
    holes[1] = 3;
    holes[2] = 2;
    holes[3] = 0;
    holes[4] = 4;

    for (int day = 0; day < 1000; day++){
        //Temporary array to assign new values to holes instead of moving them around
        int tempArray[5] = {0};

        cout << "Day " << day + 1 << endl;


        for(int position = 0; position < 5; position++){
            countSquirrels(holes, tempArray, position);
            cout << position << ": " << holes[position] << endl;
        }

        for(int i = 0; i < 5; i++){
            holes[i] = tempArray[i];
        }

        done = checkDone(holes);

        if (done){
            numDays = day;
            break;
        }
    }

    if (done){

        cout << "Final result: " << endl;
        for(int i = 0; i < 5; i++){
            cout << i << ": " << holes[i] << endl;
        }

        cout << "Squirrels are in the same hole " << numDays << " days after Day 1.";
    }

    else {
        cout << "Not able to finish.";
    }

    return 0;
}

//Checks the current hole, the hole above, and the hole below
//and compares their values before moving the squirrels accordingly.
void countSquirrels(int h[], int tempArr[], int pos)
{
    bool top = pos == 0 ? true : false;
    bool bottom = pos == 4 ? true : false;
    int movement = 0;

    //The first element of this array is the highest hole,
    //while the last element of this array is the lowest hole
    //which means that when the position is lower, its hole is
    //higher on the tree.

    //When the current hole is not the top or bottom hole
    if (!top && !bottom){

        //When the hole has less squirrels than both other holes
        if (h[pos] - 1 < h[pos - 1] && h[pos] - 1 < h[pos + 1]){

            if (h[pos + 1] == h[pos] - 1){
                movement = 1;
            }

        }

        //When the hole has more squirrels than both other holes
        else if (h[pos] - 1 > h[pos - 1] && h[pos] - 1 > h[pos + 1]){

            if (h[pos - 1] > h[pos + 1]){
                movement = 1;
            }

            else {
                movement = 2;
            }
        }

    }

    //When the current hole is the top or bottom hole
    else {
        if (top){

            if (h[pos] - 1 > h[pos - 1]){
                movement = 2;
            }

        }

        else {

            if (h[pos] - 1 > h[pos + 1]){
                movement = 1;
            }

        }
    }

    switch (movement){

        case 1:     tempArr[pos + 1] += h[pos];

        case 2:     tempArr[pos - 1] += h[pos];

        default:    tempArr[pos] += h[pos];

    }

}

//Checks if all the squirrels are in one hole
bool checkDone(int h[]){
    int equalZero = 0;

    for(int i = 0; i < 5; i++){
        if (h[i] == 0){
            equalZero++;
        }

    }

    if (equalZero == 4){
        return true;
    }

    return false;

}
