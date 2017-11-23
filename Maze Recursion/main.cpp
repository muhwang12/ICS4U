#include <iostream>
#include <fstream>
#include <apmatrix.h>
#include <allegro5/allegro.h>

#define BLACK al_map_rgb(0, 0, 0)
#define WHITE al_map_rgb(255, 255, 255)
#define RED al_map_rgb(255, 0, 0)

apmatrix<char> maze(1, 1);
const float FPS = 60;
const int SCREEN_W = 640;       // screen width
const int SCREEN_H = 480;       // screen height
ALLEGRO_DISPLAY *screen = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;

bool init_all();
bool getMaze();
bool findPath(int x, int y);
void printMaze();

using namespace std;

char fileName[] = "";


int main() {

    init_all();
    cout << "Enter file name: ";
    cin >> fileName;

    if(getMaze()){
    findPath(0, 0);
    printMaze();
    }

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

        cout << Rows << Cols << endl;
        maze.resize(Rows, Cols);

        for (int i = 0; i < Rows; i++){
            for (int j = 0; j < Cols; j++){
                fileIn >> maze[i][j];
            }
        }
    }
    printMaze();
    fileIn.close();


    return true;
}

bool findPath(int x, int y)
{

    if(x < 0 || y < 0 || x >= maze.numrows() || y >= maze.numcols()){
        return false;
    }
    else if(maze[x][y] == 'G'){
        maze[0][0] = 'S';
        return true;
    }
    else if(maze[x][y] == '#'){
        return false;
    }
    else if(maze[x][y] == '+'){
        return false;
    }

    maze[x][y] = '+';

    cout << x << y << endl;


    if(findPath(x-1, y))
        return true;
    else if(findPath(x, y-1))
        return true;
    else if(findPath(x+1, y))
        return true;
    else if(findPath(x, y+1))
        return true;

    maze[x][y] = '.';
    return false;


}

//Prints Maze to console
void printMaze()
{

    cout << endl;
    for(int row = 0; row < maze.numrows(); row++){
        for(int col = 0; col < maze.numcols(); col++){

            if(row == 0 && col == 0)
                cout << 'S';
            else
                cout << maze[row][col];
        }
        cout << endl;
    }
    cout << endl;
}

bool init_all()
{
    if(!al_init()) {
      cerr << "failed to initialize allegro!" << endl;
      return false;
    }

    timer = al_create_timer(1.0 / FPS);
       if(!timer) {
          cerr << "failed to create timer" << endl;
          return false;
       }

    screen = al_create_display(SCREEN_W, SCREEN_H);
        if(!screen) {
          cerr << "failed to create screen!" << endl;
          return false;
        }

    event_queue = al_create_event_queue();
        if(!event_queue) {
          cerr << "failed to create event_queue!" << endl;
          al_destroy_display(screen);
          return false;
        }

	return true;

}

