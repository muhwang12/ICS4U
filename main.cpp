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
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <random>
#define RED al_map_rgb(255, 0, 0)
#define GREEN al_map_rgb(0, 255, 0)

//Constant variables
const int mapW_c = 844;
const int mapH_c = 480;
const char fileName_c[] = "Colorado_844x480.dat";
ALLEGRO_DISPLAY *printedMap = nullptr;
using namespace std;

//Prototypes
bool MapDataDrawer(apmatrix<short> &map);
short findMin(apmatrix<short> map);
short findMax(apmatrix<short> map);
void drawMap(apmatrix<short> map, short lowestElevation, short highestElevation);
int drawPath(apmatrix<short> &map, int startRow, apvector<int> &path);
int drawAllPaths(apmatrix<short> map);
void printFont(short lowestElevation);
int drawDownhillPath(apmatrix<short> &map, int startRow, apvector<int> &path);


//main function
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

        minElev = findMin(map);
        cout << "The lowest point is " << minElev << " m." << endl;
        maxElev = findMax(map);
        cout << "The highest point is " << maxElev << " m." << endl;
        drawMap(map, minElev, maxElev);
        int totalChange = drawAllPaths(map);
        cout << totalChange << endl;
        printFont(minElev);
        al_flip_display();
        al_rest(100);

    // Exit with no errors
	return 0;
}

//Read data from given file into a 2D array (an apmatrix)
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

//Return the minimum value in the map
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

//Draws one path based on the row stated
int drawPath(apmatrix<short> &map, int startRow, apvector<int> &path)
{
    srand((unsigned)time(0));
    short direction[3] = {0};
    int change = 0;
    int row = startRow;
    int totalElev = 0;

    for(int j = 0; j < map.numcols()-1; j++){
        short currentElev = map[row][j];
        short elevRight = map[row][j+1];
        short elevUp = 0;
        short elevDown = 0;
        short choice;
        //cout << change << " ";
        if (row == 0){
            elevUp = 50000;
            elevDown = map[row+1][j+1];
        }
        else if (row == 479){
            elevUp = map[row-1][j+1];
            elevDown = 50000;
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
            choice = 0;
            //cout << row << " ";
           // cout << "up" << endl;
        }
        else if (direction[1] < direction[0] && direction[1] < direction[2]){
            choice = 1;
            //cout << row << " ";
           // cout << "down" << endl;
        }
        else if (direction[2] < direction[0] && direction[2] < direction[1]){
            choice = 2;
            //cout << row << " ";
           // cout << "right" << endl;
        }
        else if (direction[0] == direction[1] && direction[2] > direction[0]){
            choice = rand() % 2;
            //cout << row << " ";
            //cout << "choice:" << choice << "  ";
            //cout << "dir4" << endl;
        }
        else if (direction[0] == direction[2] && direction[1] > direction[0]){
            choice = 2*(rand() % 2);
            //cout << row << " ";
            //cout << "choice:" << choice << "  ";
            //cout << "dir5" << endl;
        }
        else if (direction[1] == direction[2] && direction[0] > direction[1]){
            choice = (rand() % 2) + 1;
            //cout << row << " ";
            //cout << "choice:" << choice << "  ";
            //cout << "dir6" << endl;
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

            case 2: change += direction[2];
                    break;

            default: break;
        }

        choice = -1;
        path[j] = row;
        al_draw_pixel(j, row, RED);
    }
    //cout << "Total cols" << map.numcols() << endl;
    return change;
}

//Draws one path based on the row stated
int drawDownhillPath(apmatrix<short> &map, int startRow, apvector<int> &path, int lowestElevPoint, int &lowestRow)
{
    srand((unsigned)time(0));
    short direction[3] = {0};
    int change = 0;
    int row = startRow;
    int totalElev = 0;
    int lowestPoint = -1;
    int tempLowRow = 0;

    for(int j = 0; j < map.numcols()-1; j++){
        short currentElev = map[row][j];
        short elev[3] = {0};
        short elev[0] = map[row][j+1];
        short elev[1] = 0;
        short elev[2] = 0;
        short choice;
        //cout << change << " ";
        if (row == 0){
            elev[0] = 50000;
            elev[1] = map[row+1][j+1];
        }
        else if (row == 479){
            elev[0] = map[row-1][j+1];
            elev[1] = 50000;
        }
        else {
            elev[0] = map[row-1][j+1];
            elev[1] = map[row+1][j+1];
        }
        //cout << "row: " << row << " ";
        //cout << "column: " << j << "\t";
        //cout << "current" << currentElev << " elevUP" << elevUp << " ElevD" << elevDown << " ElevR" << elevRight << " ";
        direction[0] = currentElev - elevUp;
        direction[1] = currentElev - elevDown;
        direction[2] = currentElev - elevRight;
        //cout << "latercur" << currentElev << " dirjuan" << direction[0] << " dirdos" << direction[1] << " dirtres" << direction[2] << " ";

        totalElev += currentElev;
        if (direction[0] > direction[1] && direction[0] > direction[2]){
            choice = 0;
            //cout << row << " ";
           // cout << "up" << endl;
        }
        else if (direction[1] > direction[0] && direction[1] > direction[2]){
            choice = 1;
            //cout << row << " ";
           // cout << "down" << endl;
        }
        else if (direction[2] > direction[0] && direction[2] > direction[1]){
            choice = 2
            //cout << row << " ";
           // cout << "right" << endl;
        }
        else if (direction[0] == direction[1] && direction[2] < direction[0]){
            choice = rand() % 2;
            //cout << row << " ";
            //cout << "choice:" << choice << "  ";
            //cout << "dir4" << endl;
        }
        else if (direction[0] == direction[2] && direction[1] < direction[0]){
            choice = 2*(rand() % 2);
            //cout << row << " ";
            //cout << "choice:" << choice << "  ";
            //cout << "dir5" << endl;
        }
        else if (direction[1] == direction[2] && direction[0] < direction[1]){
            choice = (rand() % 2) + 1;
            //cout << row << " ";
            //cout << "choice:" << choice << "  ";
            //cout << "dir6" << endl;
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

            case 2: change += direction[2];
                    break;

            default: break;
        }

        if (lowestPoint < 0 || elev[choice] < lowestPoint){
            lowestPoint = elev[choice];
            tempLowCol = j;
        }

        choice = -1;
        path[j] = row;
        al_draw_pixel(j, row, RED);
    }
    //cout << "Total cols" << map.numcols() << endl;
    lowestRow = tempLowRow;
    return change;
}


//Draws all the possible paths, then marks the shortest one
int drawAllPaths(apmatrix<short> map, int algorithm)
{
    apvector<int> bestRunCols(map.numcols());
    apvector<int> runCols(map.numcols());
    int totalChange = 0;
    int smallestChange = -1;
    short smallestRow = 0;
    int lowestPoint = 0;
    int lowestRow = 0;

        for (int x = 0; x < map.numrows(); x++){
            if (algorithm == 0)
            totalChange = drawPath(map, x, runCols);
            else {
            totalChange = drawDownhillPath(map, x, runCols, lowestPoint, lowestRow);
            al_draw_pixel(x, lowestRow, BLUE);
            }

            cout << "row: " << x << "totalChange: " << totalChange << endl;

            if (smallestChange < 0 || smallestChange > totalChange){
                smallestChange = totalChange;
                smallestRow = x;
                bestRunCols = runCols;
            }
            totalChange = 0;
        }

        cout << "smallest index: " << smallestRow << endl;
        for (int i = 0; i < map.numcols(); i++){
            al_draw_pixel(i, bestRunCols[i], GREEN);
        }


    return smallestChange;
}

void printFont(short lowestElevation)
{
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon();// initialize the ttf (True Type Font) addon

    ALLEGRO_FONT *font = al_load_ttf_font("font.ttf", 30, 0);

    if (!font){
        cout << "Could not load 'font.ttf'";
    }

    al_draw_textf(font, al_map_rgb(255,255,255), 200, 50, ALLEGRO_ALIGN_CENTRE, "Lowest Elevation: %d m", lowestElevation);

}
