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

//Merge sort from: https://www.geeksforgeeks.org/merge-sort/
//Quick sort from: https://www.geeksforgeeks.org/quick-sort/
//Insertion sort from: https://www.geeksforgeeks.org/insertion-sort/
ALLEGRO_DISPLAY *display;
ALLEGRO_TIMER *timer;
ALLEGRO_BITMAP *screenBitmap;
ALLEGRO_EVENT_QUEUE *event_queue;

using namespace std;
void merge(int arr[], int l, int m, int r);
void mergeSort(int arr[], int l, int r);
void printArray(int A[], int size);
void swap(int *a, int *b);
int partition(int arr[], int low, int high);
void quickSort(int arr[], int low, int high);
void insertionSort(int arr[], int n);
void drawList(int data[], int data_size, int pos);
void randomNum(int arr[]);
char chooseOption();
char chooseSort();
void drawList(int arr[], int size, int pos);
bool allegroEventLoop();
bool initAllegro(const char* title, const int windowWidth_c,
		const int windowHeight_c);
void cleanUp();
int windowWidth;
int windowHeight;
void clearBitmap();

struct Person {

    apstring Name;
    apstring Citizenship;
    apstring Rank;
    apstring Worth;
    int age;

};

struct Billionaire {

    Person person;
    apstring companyName;
};

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
    initAllegro("Sort", 640, 480);
    int arr[200];
    //vector<Billionaire> bList;
    int arr_size = sizeof(arr)/ sizeof(arr[0]);

    randomNum(arr);

    cout << "Given: " << endl;
    printArray(arr, arr_size);

    timer.start();
    mergeSort(arr, 0, arr_size-1);
    timer.stop();

    cout << "Sorted: " << endl;
    printArray(arr, arr_size);

    std::cout << "Seconds: " << timer.elapsedSeconds() << std::endl;
    std::cout << "Milliseconds: " << timer.elapsedMilliseconds() << std::endl;


}

/*char chooseOption()
{
    char choice;
    cout << "(L)oad Data" << endl;
    cout << "(S)ave Data" << endl;
    cout << " Sort (D)ata " << endl;

    while(choice != 'L' || choice != 'S' || choice != 'D')
    {
        cout << "Choose what you want to do: ";
        cin >> choice;
        cout << endl;
    }

    return choice;
}

void selectCommand(char choice)
{
    switch (choice){

    case 'L': readFile();
              break;

    case 'S': break;

    case 'D': break;

    default: break;
    }
}*/

char chooseSort()
{
    char choice;
    cout << "(I)nsertion Sort" << endl;
    cout << "(Q)uick Sort" << endl;
    cout << "(M)erge Sort" << endl;

    while(choice != 'I' || choice != 'Q' || choice != 'M')
    {
        cout << "Choose a sorting algorithm: ";
        cin >> choice;
        cout << endl;
    }
    return choice;
}

/*void loadFile(vector<Billionaire> list)
{
    apstring fileName;
    ifstream fileIn;
    cout << "Enter file name (include .csv): " << endl;
    cin >> fileName;
    fileIn.open(fileName);


}*/

/*void printBillionaire(Billionaire p)
{
    cout << "Name of Billionaire: " << p.person.Name << endl;
         << "Rank: " << p.person.Rank << endl
         << "Worth: " << p.person.Worth << " billion" << endl
         << "Company Name: " << p.companyName << endl
         << "Age: " << p.person.age << endl
         << "Citizenship: "<< p.person.Citizenship << endl;

}*/

void randomNum(int arr[])
{
    int count = 0;
    for(int i = 0; i < 200; i++){
        count++;
        arr[i] = rand() % 200 + 1;
    }
    cout << "Count: " << count << endl;
}

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    int L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2){
        drawList(arr, 200, k);
        if (L[i] <= R[j]){
            arr[k] = L[i];
            i++;
        }
        else{
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1){
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2){
        arr[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
    if (l < r){
        // Same as (l+r)/2, but avoids overflow for
        // large l and r
        int m = l+(r-l)/2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);

        merge(arr, l, m, r);
    }
}
//Swaps two elements
void swap(int& a, int& b)
{
    int n = a;
    a = b;
    b = n;
}

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition (int arr[], int low, int high)
{
    int pivot = arr[high];    // pivot
    int wall = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++){
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot){
            wall++;    // increment index of smaller element
            swap(arr[wall], arr[j]);
        }
    }
    swap(arr[wall + 1], arr[high]);
    return (wall + 1);
}

/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

    /* Function to sort an array using insertion sort*/
void insertionSort(int arr[], int n)
{
   int key, j;
   for (int i = 1; i < n; i++){
       key = arr[i];
       j = i-1;

       /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
       while (j >= 0 && arr[j] > key){
           arr[j+1] = arr[j];
           j = j-1;
       }
       arr[j+1] = key;
    }

}

void printArray(int A[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        cout << A[i] << " " << endl;
}

bool initAllegro(const char* title, const int windowWidth_c,
		const int windowHeight_c) {

	//initialize display
	al_init();
	display = al_create_display(windowWidth_c, windowHeight_c);
	windowWidth = windowWidth_c;
	windowHeight = windowHeight_c;

	// Always check if your allegro routines worked successfully.
	if (!display) {
		std::cerr << "allegro error: failed to initialize display!"
				<< std::endl;
		return false;
	}
    int fps = 60;
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / fps);

	if (!timer) {
		std::cerr << "allegro error: failed to create timer!" << std::endl;
		al_destroy_display(display);
		return false;
	}

	ALLEGRO_BITMAP *screenBitmap = al_create_bitmap(windowWidth_c, windowHeight_c);

	if (!screenBitmap) {
		std::cerr << "allegro error: failed to create bitmap!" << std::endl;
		al_destroy_display(display);
		al_destroy_timer(timer);
		return false;
	}

	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	if (!event_queue) {
		std::cerr << "allegro error: failed to create allegro event queue!"
				<< std::endl;
		al_destroy_bitmap(screenBitmap);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return false;
	}

	if(!al_install_mouse()) {
		std::cerr << "allegro error:  could not init mouse'" << std::endl;
		cleanUp();
		return false;
	}

	if (!al_init_primitives_addon()) {
		std::cerr << "allegro error: could not init primitives" << std::endl;
		al_destroy_event_queue(event_queue);
		al_destroy_bitmap(screenBitmap);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return false;
	}

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_start_timer(timer);

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_set_window_title(display, title);
	al_flip_display();

	return true;
}

bool allegroEventLoop() {

	while (true) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {

		} else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {

			return false;
		}
		if (ev.type == ALLEGRO_EVENT_MOUSE_AXES
				|| ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {


		} else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

		}

	}
	return true;
}
void clearBitmap()  {
		//this should clear the bitmap
		al_clear_to_color(al_map_rgb(0,0,0));
	}
void drawList(int arr[], int size, int pos) {
	static float xratio = ((float)windowWidth) / size;
	static float yratio = ((float)windowHeight) / size;

	static ALLEGRO_COLOR green = al_map_rgb(0, 0xff, 0);
	static ALLEGRO_COLOR red = al_map_rgb(0xff, 0, 0);

	clearBitmap();
	for (int r = 0; r < size; r++) {
		if (r == pos) {
            al_draw_rounded_rectangle(r * xratio, windowHeight - arr[r]* yratio, (r+1)* xratio, windowHeight , 0.25*xratio, 0.25*xratio, red,(4<xratio/2)?4:(int)xratio/2);
		} else {
            al_draw_rounded_rectangle(r * xratio, windowHeight - arr[r]* yratio, (r+1)* xratio, windowHeight , 0.25*xratio, 0.25*xratio, green,(4<xratio/2)?4:(int)xratio/2);
		}
	}

	al_flip_display();
}

void cleanUp() {
	al_destroy_bitmap(screenBitmap);
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
}


