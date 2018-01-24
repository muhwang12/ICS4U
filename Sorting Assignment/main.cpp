/*****************************************************************************
 *	Name: Matthew Wang                                                       *
 *	Course: ICS4U                                                            *
 *	Date: January 24, 2018                                                   *
 *	                                                                         *
 *	Purpose: Use sorting algorithms to sort out numbers                      *
 *	                                                                         *
 *	Usage:                                                                   *
 *	                                                                         *
 *	Revision History:                                                        *
 *	                                                                         *
 *	Known Issues:                                                            *
 *	                                                                         *
 *****************************************************************************/

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <apstring.h>
#include <vector>
#include <fstream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

//Merge sort from: https://www.geeksforgeeks.org/merge-sort/
//Quick sort from: https://www.geeksforgeeks.org/quick-sort/
//Insertion sort from: https://www.geeksforgeeks.org/insertion-sort/

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_BITMAP *screenBitmap = NULL;
ALLEGRO_FONT *algorithmFont = NULL;

using namespace std;

//Mergesort functions
void merge(vector<int> &v, int l, int m, int r);
void mergeSort(vector<int> &v, int l, int r);

//Quicksort functions
void swap(int *a, int *b);
int partition(vector<int> &v, int low, int high);
void quickSort(vector<int> &v, int low, int high);

//Insertion sort function
void insertionSort(vector<int> &v, int n);

void randomNum(vector<int> &vector);
void printVector(vector<int> &vector);

char chooseSort();
void selectSort(char choice, vector<int> &v);

void drawList(vector<int> &v, int pos);

bool initAllegro(const char* title);
void cleanUp();
void clearBitmap();

int windowWidth_c = 640;
int windowHeight_c = 480;
int numSwap = 0;
char c;

//Timer for tracking how long it takes for a sorting algorithm to run
class Timer
{
public:
    void start()
    {
        m_StartTime = std::chrono::system_clock::now();
        m_bRunning = true;
    }

    void stop()
    {
        m_EndTime = std::chrono::system_clock::now();
        m_bRunning = false;
    }

    double elapsedMilliseconds()
    {
        std::chrono::time_point<std::chrono::system_clock> endTime;

        if(m_bRunning)
        {
            endTime = std::chrono::system_clock::now();
        }
        else
        {
            endTime = m_EndTime;
        }

        return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
    }

    double elapsedSeconds()
    {
        return elapsedMilliseconds() / 1000.0;
    }

private:
    std::chrono::time_point<std::chrono::system_clock> m_StartTime;
    std::chrono::time_point<std::chrono::system_clock> m_EndTime;
    bool                                               m_bRunning = false;
};

int main()
{
    Timer timer;

    srand(NULL);
    initAllegro("Sort");
    vector<int> vec (200);
    randomNum(vec);

    cout << "Given: " << endl;
    printVector(vec);

    c = chooseSort();
    timer.start();
    selectSort(c, vec);
    timer.stop();

    cout << endl;
    cout << "Sorted: " << endl;
    printVector(vec);

    cout << endl;
    cout << "Number of swaps: " << numSwap << endl;
    std::cout << "Seconds: " << timer.elapsedSeconds() << std::endl;
    std::cout << "Milliseconds: " << timer.elapsedMilliseconds() << std::endl;

    al_rest(5);
    cleanUp();
    return 0;
}

//Function to choose sorting algorithm
char chooseSort()
{
    char choice;
    cout << endl << endl
         << "(I)nsertion Sort" << endl
         << "(Q)uick Sort" << endl
         << "(M)erge Sort" << endl;

    while(choice != 'I' && choice != 'Q' && choice != 'M')
    {
        cout << "Choose a sorting algorithm: ";
        cin >> choice;
    }

    return choice;
}

//Takes choice and directs it towards its associated algorithm
void selectSort(char choice, vector<int> &v)
{

    switch (choice){

        case 'I':   insertionSort(v, v.size());
                    break;

        case 'M':   mergeSort(v, 0, v.size()-1);
                    break;

        case 'Q':   quickSort(v, 0, v.size()-1);
                    break;

    }


}

//Creates a vector of random integers
void randomNum(vector<int> &vector)
{
    int count = 0;
    for(int i = 0; i < 200; i++){
        count++;
        vector[i] = rand() % 200 + 1;
    }
    cout << "Count: " << count << endl;
}

//Merges vectors together
void merge(vector<int> &v, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp vectors */
    vector<int> L(n1);
    vector<int> R(n2);
    /* Copy data to temp vectors L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = v[l + i];
    for (j = 0; j < n2; j++)
        R[j] = v[m + 1 + j];

    /* Merge the temp vectors back into v[l..r]*/
    i = 0; // Initial index of first subvector
    j = 0; // Initial index of second subvector
    k = l; // Initial index of merged subvector
    while (i < n1 && j < n2){
        if (L[i] <= R[j]){
            v[k] = L[i];
            i++;
            numSwap++;
        }
        else{
            v[k] = R[j];
            j++;
        }
        k++;
        drawList(v, k);
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1){
        v[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2){
        v[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
   sub-vector of v to be sorted */
void mergeSort(vector<int> &v, int l, int r)
{
    if (l < r){
        // Same as (l+r)/2, but avoids overflow for
        // large l and r
        int m = l+(r-l)/2;

        // Sort first and second halves
        mergeSort(v, l, m);
        mergeSort(v, m+1, r);
        merge(v, l, m, r);
    }
}
//Swaps two elements
void swap(int& a, int& b)
{
    int n = a;
    a = b;
    b = n;
    numSwap++;
}

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    vector, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition (vector<int> &v, int low, int high)
{
    int pivot = v[high];    // pivot
    int wall = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++){
        // If current element is smaller than or
        // equal to pivot
        if (v[j] <= pivot){
            wall++;    // increment index of smaller element
            swap(v[wall], v[j]);
            drawList(v, j);
        }
    }
    swap(v[wall + 1], v[high]);
    return (wall + 1);
}

/* The main function that implements QuickSort
  v --> vector to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quickSort(vector<int> &v, int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, v[p] is now
           at right place */
        int pi = partition(v, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(v, low, pi - 1);
        quickSort(v, pi + 1, high);
    }
}

//Function to sort a vector using insertion sort
void insertionSort(vector<int> &v, int n)
{
   int key, j;
   for (int i = 1; i < n; i++){
       key = v[i];
       j = i-1;

       /* Move elements of v[0..i-1] that are
          greater than key, to one position ahead
          of their current position */
       while (j >= 0 && v[j] > key){
           v[j+1] = v[j];
           j = j-1;
           drawList(v, j);
           numSwap++;
       }
       v[j+1] = key;

    }

}

//Prints out numbers in vector
void printVector(vector<int> &vector)
{

    for (int i = 0; i < vector.size(); i++){
        if(i == vector.size()-1)
        cout << vector[i];
        else
        cout << vector[i] << ", ";
    }

}

//Initializes allegro routines
bool initAllegro(const char* title) {

	//initialize display
	al_init();
	display = al_create_display(windowWidth_c, windowHeight_c);

	// Always check if your allegro routines worked successfully.
	if (!display) {
		std::cerr << "allegro error: failed to initialize display!"
				<< std::endl;
		return false;
	}

	ALLEGRO_BITMAP *screenBitmap = al_create_bitmap(windowWidth_c, windowHeight_c);

	if (!screenBitmap) {
		std::cerr << "allegro error: failed to create bitmap!" << std::endl;
		al_destroy_display(display);
		return false;
	}


	if (!al_init_primitives_addon()) {
		std::cerr << "allegro error: could not init primitives" << std::endl;
		//al_destroy_event_queue(event_queue);
		al_destroy_bitmap(screenBitmap);
		al_destroy_display(display);
		//al_destroy_timer(timer);
		return false;
	}

	al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon

    algorithmFont = al_load_ttf_font("font.ttf", 30, 0);
	if (!algorithmFont) {
		cerr << "fatal error: could not load 'font.ttf'" << endl;
		return false;
	}

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_set_window_title(display, title);
	al_flip_display();

	return true;
}

//Clears bitmap
void clearBitmap()  {
		al_clear_to_color(al_map_rgb(0,0,0));
	}

//Draws list onto screen
void drawList(vector<int> &v, int pos) {
	static float xratio = ((float)windowWidth_c) / v.size();
	static float yratio = ((float)windowHeight_c) / v.size();

	static ALLEGRO_COLOR green = al_map_rgb(0, 255, 0);

	clearBitmap();
	for (int r = 0; r < v.size(); r++) {
	    float ratio = v[r] / (float)200;

	    switch(c){

            case 'I': al_draw_textf(algorithmFont, al_map_rgb(255, 255, 255), 125, 50,
                              ALLEGRO_ALIGN_CENTER, "Insertion Sort");
                      break;

            case 'Q': al_draw_textf(algorithmFont, al_map_rgb(255, 255, 255), 125, 50,
                              ALLEGRO_ALIGN_CENTER, "Quick Sort");
                      break;

            case 'M': al_draw_textf(algorithmFont, al_map_rgb(255, 255, 255), 125, 50,
                              ALLEGRO_ALIGN_CENTER, "Merge Sort");
                      break;


	    }

		if (r == pos) {
			al_draw_filled_rounded_rectangle(r * xratio, windowHeight_c - v[r]* yratio, (r+1)* xratio, windowHeight_c, 0.25*xratio, 0.25*xratio, green);
			//if(xratio > 2) al_draw_rounded_rectangle(r * xratio, windowHeight_c - v[r]* yratio, (r+1)* xratio, windowHeight_c, 0.25*xratio, 0.25*xratio, green,(4<xratio/2)?4:(int)xratio/2);
		}
        else {
			al_draw_filled_rounded_rectangle(r * xratio, windowHeight_c - v[r]* yratio , (r+1)* xratio, windowHeight_c, 0.25*xratio, 0.25*xratio, al_map_rgb(160, 255*ratio/2, 255*ratio));
			//if(xratio > 2) al_draw_rounded_rectangle(r * xratio, windowHeight_c - v[r]* yratio, (r+1)* xratio, windowHeight_c, 0.25*xratio, 0.25*xratio, al_map_rgb(255, 255*ratio/2, 255*ratio),(4<xratio/2)?4:(int)xratio/2);
		}
	}

	al_flip_display();

}

void cleanUp() {
	al_destroy_bitmap(screenBitmap);
	al_destroy_display(display);
	al_destroy_font(algorithmFont);
}



