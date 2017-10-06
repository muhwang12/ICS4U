/*****************************************************************************
 *	Name:     Matthew Wang                                                   *
 *                                                                           *
 *	Date:     September 20, 2017                                             *
 *                                                                           *
 *	Purpose:    To map out a topographical map corresponding to a data       *
 *              file that gives the elevations of each point. To also        *
 *              use said elevation values to map out different paths to      *
 *              take from east to west that try to change the least in       *
 *              elevation.                                                   *
 *	                                                                         *
 *	Usage:      First, type in the algorithm you would like to use.          *
 *              The program will then output red paths and their             *
 *              corresponding information. Finally, the path with            *
 *              the least changes will be outlined in green.                 *
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

//Constant global variables
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
int drawPath(apmatrix<short> &map, int startRow, apvector<int> &pathCols);
int drawAllPaths(apmatrix<short> map, int algorithm);
void printElevation(short lowestElevation, int algorithm, int elevChange);
int drawDownhillPath(apmatrix<short> &map, int startRow,
                     apvector<int> &path, int &lowestElevPoint, int &lowestCol);


//main function
int main(int argc, char *argv[]) {

    //Declaring Variables
    apmatrix<short> map (mapH_c, mapW_c, 0);
    short minElev = 0;
    short maxElev = 0;
    int decision = -1;

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

    bool readFile = MapDataDrawer(map);

    if (readFile == false){
        cerr << "File failed to read data into map matrix variable";
    }
        //Finds highest and lowest elevations
        maxElev = findMax(map);
        cout << "The highest point is " << maxElev << " m." << endl;
        minElev = findMin(map);
        cout << "The lowest point is " << minElev << " m." << endl;

        //Input algorithm choice
        while(decision != 0 && decision != 1) {
        cout << "Choose your algorithm"
             << " (Type 0 for Greedy Algorithm and type 1 for Downhill Algorithm)"
             << ": ";

        cin >> decision;
        }

        drawMap(map, minElev, maxElev);
        int total = drawAllPaths(map, decision);
        printElevation(minElev, decision, total);
        al_flip_display();
        al_rest(100);


    // Exit with no errors
	return 0;
}

//Reads data from given file into a 2D array (an apmatrix)
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

//Returns the minimum elevation value in the map
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

//Returns the maximum elevation value in the map
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

//Draws map based on elevation values and their ratio to the highest elevation
void drawMap(apmatrix<short> map, short lowestElevation, short highestElevation)
{
    const short maxColorValue_c = 255;

    // Monochrome Version

    /*float multiplier = 0;

    for(int i = 0; i < map.numcols(); i++){
        for(int j = 0; j < map.numrows(); j++){
            multiplier = (map[j][i] - lowestElevation) / ((float)highestElevation - lowestElevation);
            al_draw_pixel(i, j, al_map_rgb(multiplier*maxColorValue_c, multiplier*maxColorValue_c, multiplier*maxColorValue_c));
        }
    }*/



    // Colored Version

    /*   For loop that separates the height elevation into three categories based on color:
       Above 2700m: White/ Gray
       Between 1800m and 2700m: Purple
       Below 1800m: Green
       The lighter the shade, the higher the elevation
    */
    int heightLimits[2] = {1800, 2700};

	for (int y = 0; y < map.numrows(); y++) {
		for (int x = 0; x < map.numcols(); x++) {
			if (map[y][x] <= heightLimits[0]) {
                //Shades of gray and white for the highest areas
				int shade = (map[y][x] - lowestElevation) * maxColorValue_c / (heightLimits[0] - lowestElevation);
				al_draw_pixel(x, y, al_map_rgb(0, shade, 0));
			}
			else if ((map[y][x] > heightLimits[0]) && (map[y][x] <= heightLimits[1])) {
                 //Shades of purple for middle areas
                 int shade[3];
			     //Decimal value for the relative shade of red
			     shade[0] = (heightLimits[0] - map[y][x]) * (240 - 160)
						/ (heightLimits[1] - heightLimits[0]) + 160;
                 //Decimal value for the relative shade of green
				 shade[1] = (heightLimits[0] - map[y][x]) * (102 - 34)
						/ (heightLimits[1] - heightLimits[0]) + 102;
                //Decimal value for the relative shade of blue
				 shade[2] = (heightLimits[0] - map[y][x]) * (240 - 160)
						/ (heightLimits[1] - heightLimits[0]) + 240;

				al_draw_pixel(x, y, al_map_rgb(shade[0], shade[1], shade[2]));
			}
			else {
                //Shades of green for lower areas
				int shade = (map[y][x] - heightLimits[1]) * (maxColorValue_c - 160)
						/ (highestElevation - heightLimits[1]) + 160;
				al_draw_pixel(x, y, al_map_rgb(shade, shade, shade));
			}
		}
	}

}

//Draws one path based on the row stated
int drawPath(apmatrix<short> &map, int startRow, apvector<int> &pathCols)
{
    //Declaring Variables
    srand((unsigned)time(0));
    //The changes in the three directions: index 0 is Up, index 1 is Down, and index 2 is Right
    short direction[3] = {0};
    int change = 0;
    int row = startRow;

    //For loop that finds every point on a path
    for(int j = 0; j < map.numcols()-1; j++){

        //Declaring variables based on
        short currentElev = map[row][j];
        short elevRight = map[row][j+1];
        short elevUp;
        short elevDown;
        short choice; //Choice variable for which direction to go

        //If statement that prevents out-of-bound errors
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
        //cout << "latercur" << currentElev << " dir1 " << direction[0] << " dir2 " << direction[1] << " dir3 " << direction[2] << " ";

        //If statement checking for which direction is best
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

        //Switch statement that implements the choice made by the if statement
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

        pathCols[j] = row;
        al_draw_pixel(j, row, RED);
    }
    //cout << "Total cols" << map.numcols() << endl;


    return change;
}

//Draws one path based on the row stated with a priority of going downhill as much as possible
int drawDownhillPath(apmatrix<short> &map, int startRow,
                     apvector<int> &path, int &lowestElevPoint, int &lowestCol)
{
    //Declaring Variables
    srand((unsigned)time(0));
    //The changes in the three directions: index 0 is Up, index 1 is Down, and index 2 is Right
    int direction[3];
    int change = 0;
    int row = startRow;
    int lowestPoint = -1;
    int tempLowCol = 0;
    int elev[3]; //Elevations of the three directions

    //For loop that finds every point on a path
    for(int j = 0; j < map.numcols()-1; j++){

        int currentElev = map[row][j];
        short choice; //Choice variable for which direction to go

        elev[2] = map[row][j+1];

        //cout << change << " ";
        //If statement that prevents out-of-bound errors
        if (row == 0){
            elev[0] = 100000;
            elev[1] = map[row+1][j+1];
        }
        else if (row == 479){
            elev[0] = map[row-1][j+1];
            elev[1] = 100000;
        }
        else {
            elev[0] = map[row-1][j+1];
            elev[1] = map[row+1][j+1];
        }
        //cout << "row: " << row << " ";
        //cout << "column: " << j << "\t";
        //cout << "current" << currentElev << " elevUP" << elev[0] << " ElevD" << elev[1] << " ElevR" << elev[2] << " ";
        direction[0] = currentElev - elev[0];
        direction[1] = currentElev - elev[1];
        direction[2] = currentElev - elev[2];
        //cout << "latercur" << currentElev << " dir1 " << direction[0] << " dir2 " << direction[1] << " dir3 " << direction[2] << endl;

        //If statement checking for which direction is best
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
            choice = 2;
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

        //Switch statement that implements the choice made by the if statement
        switch (choice){

            case 0: row--;
                    change += abs(direction[0]);
                    break;

            case 1: row++;
                    change += abs(direction[1]);
                    break;

            case 2: change += abs(direction[2]);
                    break;

            default: break;
        }

        //Calculating for lowest point per starting row
        if (lowestPoint < 0 || elev[choice] < lowestPoint){
            lowestPoint = elev[choice];
            tempLowCol = j;
        }

        path[j] = row;
        al_draw_pixel(j, row, RED);
    }
    //cout << "Total cols" << map.numcols() << endl;


    lowestElevPoint = lowestPoint;
    lowestCol = tempLowCol;
    return change;
}


//Draws all the possible paths, then marks the shortest one
int drawAllPaths(apmatrix<short> map, int algorithm)
{
    //Declaring Variables
    apvector<int> bestRunCols(map.numcols());
    apvector<int> runCols(map.numcols());
    int totalChange = 0;
    int smallestChange = -1;
    short smallestRow = 0;
    int lowestPoint = 0;
    int lowestCol = 0;

        //For loop that draws out all the paths and prints their corresponding information in command line
        for (int x = 0; x < map.numrows(); x++){

            //Different algorithms and their different outputs
            if (algorithm == 0){
            totalChange = drawPath(map, x, runCols);
            cout << "Row: " << x << "   " << "Total change: " << totalChange << "m" << endl;
            }
            else {
            totalChange = drawDownhillPath(map, x, runCols, lowestPoint, lowestCol);
            cout << "Coords for lowest point in row " << x << ": "
                 << "(" << x << ", " << lowestCol << ")" << endl
                 << "Elevation: " << lowestPoint << "   "
                 << "Total change: " << totalChange << "m" << endl;
            lowestPoint = 0;
            }



            if (smallestChange < 0 || smallestChange > totalChange){
                smallestChange = totalChange;
                smallestRow = x;
                bestRunCols = runCols;
            }
            totalChange = 0;


        }

        //Prints best run information in command line
        cout << endl
             << "Smallest index: " << smallestRow << endl
             << "Total elevation change: " << smallestChange << "m" << endl;
        //Prints best run in green
        for (int i = 0; i < map.numcols(); i++){
            al_draw_pixel(i, bestRunCols[i], GREEN);
        }


    return smallestChange;
}

//Prints chosen algorithm, lowest elevation point, and total elevation change on the map
void printElevation(short lowestElevation, int algorithm, int elevChange)
{
    char algName[20] = "";
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon();// initialize the ttf (True Type Font) addon

    ALLEGRO_FONT *font = al_load_ttf_font("font.ttf", 30, 0);

    if (!font){
        cout << "Could not load 'font.ttf'";
    }
    switch (algorithm){

        case 0: strcpy(algName, "Greedy Algorithm");
                break;

        case 1: strcpy(algName, "Downhill Algorithm");
                break;

        default: break;

    }

    al_draw_textf(font, al_map_rgb(255,255,255), 235, 50, ALLEGRO_ALIGN_CENTRE, "Algorithm: %s", algName);
    al_draw_textf(font, al_map_rgb(255,255,255), 200, 100, ALLEGRO_ALIGN_CENTRE, "Lowest Elevation: %dm", lowestElevation);
    al_draw_textf(font, al_map_rgb(255,255,255), 250, 150, ALLEGRO_ALIGN_CENTRE, "Total Elevation Change: %dm", elevChange);

}
