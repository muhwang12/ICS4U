#include <iostream>
#include <fstream>
#include <apmatrix.h>

apmatrix<char> maze(1, 1);

bool getMaze();
bool findPath(int x, int y);
void printMaze();

using namespace std;

const char fileName_c[] = "maze1.txt";


int main() {

    getMaze();
    findPath(0, 0);
    printMaze();

}

//Reads in maze information from file
bool getMaze()
{
    int Rows = 0;
    int Cols = 0;

    ifstream fileIn;

    fileIn.open(fileName_c);

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
                cout << "h ";
                fileIn >> maze[i][j];
            }
            cout << endl;
        }
    }
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

    printMaze();

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
}

bool init_all()
{
    al_init();


}

