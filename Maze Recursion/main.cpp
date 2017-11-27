/*****************************************************************************
 *	Name: Matthew Wang                                                       *
 *	Course: ICS4U                                                            *
 *	Date: November 26, 2017                                                  *
 *	                                                                         *
 *	Purpose: To read in a maze file and find a path to the goal              *
 *           using recursion.                                                *
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
#include <apmatrix.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define BLACK al_map_rgb(0, 0, 0)
#define WHITE al_map_rgb(255, 255, 255)
#define RED al_map_rgb(255, 0, 0)
#define GREEN al_map_rgb(0, 255, 0)
#define SLATEGRAY   al_map_rgb(112,128,144)
#define GOLD al_map_rgb(255, 233, 0)

apmatrix<char> maze(1, 1);
const int SCREEN_W = 640;       // screen width
const int SCREEN_H = 480;       // screen height
int startX, startY;
int boxX, boxY;
ALLEGRO_DISPLAY *screen = NULL;

bool init_all();
bool getMaze();
void findStart();
bool findPath(int x, int y);
void printMaze();
void drawMaze();
void setProperties();

using namespace std;

char fileName[] = "";

int main() {

    init_all();

    cout << "Enter file name: ";
    cin >> fileName;

    if(getMaze()){
        setProperties();

        drawMaze();
        al_flip_display();
        al_rest(0.1);

        findPath(startX, startY);

        drawMaze();
        al_flip_display();
        printMaze();

        al_rest(3);

    }

    return 0;
}

//Reads in maze information from file
bool getMaze()
{
    int Rows = 0;
    int Cols = 0;

    ifstream fileIn;

    fileIn.open(fileName);

    if (!fileIn){
        cerr << "Unable to open file";
        return false;
    }
    else {
        fileIn >> Rows;
        fileIn >> Cols;

        maze.resize(Rows, Cols);

        for (int i = 0; i < Rows; i++){
            for (int j = 0; j < Cols; j++){
                fileIn >> maze[i][j];
            }
        }
    }

    findStart();
    printMaze();
    fileIn.close();

    return true;
}

//Checks each direction and plots path until it can't be
bool findPath(int x, int y)
{
    //Updates the maze every time something is changed
    drawMaze();
    al_flip_display();
    al_rest(0.1);

    //Checks if the checked position is outside the boundaries
    if(x < 0 || y < 0 || x >= maze.numrows() || y >= maze.numcols())
        return false;
    //Checks if marker has reached goal
    else if(maze[x][y] == 'G')
        return true;
    //Checks if marker hits a wall
    else if(maze[x][y] == '#')
        return false;
    //Checks if the only direction to go has already been selected
    else if(maze[x][y] == '+')
        return false;

    maze[x][y] = '+';

    printMaze();

    //West
    if(findPath(x-1, y))
        return true;
    //North
    else if(findPath(x, y-1))
        return true;
    //East
    else if(findPath(x+1, y))
        return true;
    //South
    else if(findPath(x, y+1))
        return true;

    maze[x][y] = '.';

    return false;

}

//Prints Maze
void printMaze()
{
    cout << endl;
    for(int row = 0; row < maze.numrows(); row++){
        for(int col = 0; col < maze.numcols(); col++){

            if(row == startX && col == startY)
                cout << 'S';
            else
                cout << maze[row][col];

        }
        cout << endl;
    }
    cout << endl;


}

//Draws out maze in allegro screen
void drawMaze()
{
    for(int x = 0; x < maze.numrows(); x++){
        for(int y = 0; y < maze.numcols(); y++){

            //Plots out each block of the maze based on what character they have
            //x and y's are inversed due to the nature of allegro
            switch(maze[x][y]){

                case '#': al_draw_filled_rectangle(boxX*y, boxY*x, boxX*(y+1), boxY*(x+1), SLATEGRAY);
                          break;

                case '.': al_draw_filled_rectangle(boxX*y, boxY*x, boxX*(y+1), boxY*(x+1), WHITE);
                          break;

                case '+': al_draw_filled_rectangle(boxX*y, boxY*x, boxX*(y+1), boxY*(x+1), GOLD);
                          break;

                case 'G': al_draw_filled_rectangle(boxX*y, boxY*x, boxX*(y+1), boxY*(x+1), GREEN);
                          break;

                case 'S': al_draw_filled_rectangle(boxX*y, boxY*x, boxX*(y+1), boxY*(x+1), RED);
                          break;

                default: break;
            }

        }
    }
}

//Finds and stores coordinates for starting block
void findStart(){

    for(int row = 0; row < maze.numrows(); row++){
        for(int col = 0; col < maze.numcols(); col++){

            if(maze[row][col] == 'S'){
                startX = row;
                startY = col;
                break;
            }

        }
    }

}

//Sets properties of each block/ box based on screen size and
//number of blocks in the maze
void setProperties()
{
    boxX = SCREEN_W/ maze.numcols();
    boxY = SCREEN_H/ maze.numrows();
}

bool init_all()
{
    if(!al_init()) {
      cerr << "failed to initialize allegro!" << endl;
      return false;
    }

    if (!al_init_primitives_addon()) {
    	cerr << "Failed to initialize primatives addon!" << endl;
    	return false;
	}

    screen = al_create_display(SCREEN_W, SCREEN_H);
        if(!screen) {
          cerr << "failed to create screen!" << endl;
          return false;
        }

	return true;

}



