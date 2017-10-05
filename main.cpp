/*****************************************************************************
 *	Name:     Matthew Wang                                                   *
 *                                                                           *
 *	Date:     September 20, 2017                                             *
 *                                                                           *
 *	Purpose:                                                                 *
 *	                                                                         *
 *	Usage:                                                                   *
 *	                                                                         *
 *	Revision History:                                                        *
 *	                                                                         *
 *	Known Issues:                                                            *
 *	                                                                         *
 *****************************************************************************/

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <apmatrix.h>
#include <allegro5/allegro.h>                       // For allegro, must be in compiler search path.
#include <allegro5/allegro_native_dialog.h> 		// for message box
#include <allegro5/allegro_primitives.h>
#include <random>
#define RED al_map_rgb(255, 0, 0)

//Constant variables
const int mapW_c = 844;
const int mapH_c = 480;
const char fileName_c[] = "Colorado_844x480.dat";
ALLEGRO_DISPLAY *printedMap = nullptr;
ALLEGRO_COLOR red_c = al_map_rgb(255, 0, 0);
ALLEGRO_COLOR green_c = al_map_rgb(0, 255, 0);
using namespace std;


bool MapDataDrawer(apmatrix<short> &map);
short findMin(apmatrix<short> map);
short findMax(apmatrix<short> map);
void drawMap(apmatrix<short> map, short lowestElevation, short highestElevation);
short drawPath(apmatrix<short> &map, short startRow, int &change);

// NOTE: argc, argv parameters are required.
int main(int argc, char *argv[]) {
    apmatrix<short> map (mapH_c, mapW_c, 0);
    short minElev = 0;
    short maxElev = 0;
    int change = 0;
	// Initialize Allegro
	al_init();

	// Initialize display
	printedMap = al_create_display(mapW_c, mapH_c);

	// Always check if your allegro routines worked successfully.
	if (!printedMap) {
    	al_show_native_message_box(printedMap, "Error", "Error", "Failed to initialize display!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
       	return -1;
	}

    // Display message box.
    al_show_native_message_box(
        printedMap,                        // screen
        "Allegro Message Box",          // Title Bar Text
        "Allegro",                      // Title
        "Display Loaded Successfully",  // message
        nullptr,
        0);  // gives okay button.

    // Free up memory taken by display.

    bool readFile = MapDataDrawer(map);

    if (readFile == false){
        cerr << "File failed to read data into map matrix variable";
    }
        short row = 0;
        minElev = findMin(map);
        cout << "The lowest point is " << minElev << " m." << endl;
        maxElev = findMax(map);
        cout << "The highest point is " << maxElev << " m." << endl;
        drawMap(map, minElev, maxElev);
        short totalE = drawPath(map, 0, change);
        cout << totalE << endl;
        al_flip_display();
        al_rest(100);

    // Exit with no errors
	return 0;
}

//read data from given file into a 2D array (an apmatrix)
bool MapDataDrawer(apmatrix<short> &map)
{
    ifstream fileIn;

    fileIn.open(fileName_c);

    if (!fileIn){
        cerr << "Unable to open file";
        return false;
    }
    else {
        for (int i = 0; i < map.numrows(); i++){
            for (int j = 0; j < map.numcols(); j++){
                fileIn >> map[i][j];
            }
        }
    }
    fileIn.close();

    return true;
}

//return the minimum value in the map
short findMin(apmatrix<short> map)
{
    short temp = map[0][0];

    for (int i = 0; i < map.numrows(); i++){
        for (int j = 0; j < map.numcols(); j++){
            if (temp > map[i][j]){
                temp = map[i][j];
            }
        }
    }

    return temp;
}

//return the maximum value in the map
short findMax(apmatrix<short> map)
{

    short temp = 0;

    for (int i = 0; i < map.numrows(); i++){
        for (int j = 0; j < map.numcols(); j++){
            if (temp < map[i][j]) {
                temp = map[i][j];
            }
        }
    }

    return temp;
}

//draws map in black and white based on elevation values and their ratio to the highest elevation
void drawMap(apmatrix<short> map, short lowestElevation, short highestElevation)
{
    const short maxColorVal_c = 255;
    float multiplier = 0;

    for(int i = 0; i < map.numcols(); i++){
        for(int j = 0; j < map.numrows(); j++){
            multiplier = (map[j][i] - lowestElevation) / ((float)highestElevation - lowestElevation);
            al_draw_pixel(i, j, al_map_rgb(multiplier*maxColorVal_c, multiplier*maxColorVal_c, multiplier*maxColorVal_c));
        }
    }
}

short drawPath(apmatrix<short> &map, short startRow, int &change)
{
    short direction[3] = {0};
    short row = startRow;
    short totalElev = 0;

    for(int j = 0; j < map.numcols()-1; j++){
        al_draw_pixel(j, row, RED);
        short currentElev = map[row][j];
        short elevRight = map[row][j+1];
        short elevUp = 0;
        short elevDown = 0;
        short choice;
        cout << change << " ";
        if (row == 0){
            elevUp = 20000;
            elevDown = map[row+1][j+1];
        }
        else if (row == 843){
            elevUp = map[row-1][j+1];
            elevDown = 20000;
        }
        else {
            elevUp = map[row-1][j+1];
            elevDown = map[row+1][j+1];
        }
        //cout << "row: " << row << " ";
        //cout << "column: " << j << "\t";
        //cout << "current" << currentElev << " elevUP" << elevUp << " ElevD" << elevDown << " ElevR" << elevRight << " ";
        direction[0] = abs(currentElev - elevUp);
        direction[1] = abs(currentElev - elevDown);
        direction[2] = abs(currentElev - elevRight);
        //cout << "latercur" << currentElev << " dirjuan" << direction[0] << " dirdos" << direction[1] << " dirtres" << direction[2] << " ";

        totalElev += currentElev;
        if (direction[0] < direction[1] && direction[0] < direction[2]){
            change += direction[0];
            row--;
            cout << row << " ";
            cout << "up" << endl;
        }
        else if (direction[1] < direction[0] && direction[1] < direction[2]){
            row++;
            change += direction[1];
            cout << row << " ";
            cout << "down" << endl;
        }
        else if (direction[2] < direction[0] && direction[2] < direction[1]){
            change += direction[2];
            cout << row << " ";
            cout << "right" << endl;
        }
        else if (direction[0] == direction[1] && direction[2] > direction[0]){
            choice = rand() % 2;
            cout << row << " ";
            //cout << "choice:" << choice << "  ";
            cout << "dir4" << endl;
        }
        else if (direction[0] == direction[2] && direction[1] > direction[0]){
            choice = 2*rand() % 2;
            cout << row << " ";
            //cout << "choice:" << choice << "  ";
            cout << "dir5" << endl;
        }
        else if (direction[1] == direction[2] && direction[0] > direction[1]){
            choice = rand() % 2 + 1;
            cout << row << " ";
            //cout << "choice:" << choice << "  ";
            cout << "dir6" << endl;
        }
        else if (direction[1] == direction[2] && direction[0] == direction[1]){
            choice = rand() % 3;
        }
        else {
            return -1;
            cout << "invalid" << endl;
        }

        switch (choice){

            case 0: row--;
                    change += direction[0];
                    break;

            case 1: row++;
                    change += direction[1];
                    break;

            default: change += direction[2];
                    break;
        }


    }
    return totalElev;
}

//short drawLowestElevPath(map, short startRow){
    /*short change;
    for (y = 0; y < map.numcols(); y++){
        for (x = 0; x < map.numrows(); x++){
            chooseDirection(map[x+1][y-1], map[x+1][y+1], map[x][y+1], map[x][y], change);
        }
    }*/

//}
