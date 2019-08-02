/*****************************************
*****************************
Jerry Yan, Matthew Wang ICS3U
Summative project : Go (board game)
This is the project main which runs the game
******************************************
****************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro.h>

void init_all();
int show_menu();
void show_options();
void show_rules();
void input_names();
bool load_data();
int start_game();
void returntomain();
int click_check(int clickX, int clickY);
void group_stones();
void join_groups();
void add_liberties();
void check_liberties();
void board_draw();
void remove_white();
void remove_black();
bool save_game();
void draw_savescreen();
bool save_confirm();
void count_score();
void end_screen(int gameOutcome);

void wait();
bool replay = true;

struct Board
{
	int stoneType;
	//stores 0 (blank), 1 (black), 2 (white)
	int groupAddress;
	int liberties;
	int placeX;
	int placeY;
	int bb_top;
	int bb_bottom;
	int bb_left;
	int bb_right;
};

//Files
FILE *fptr;

//Global
Board gameboard[9][9];
int totalLiberties[200];
int stoneCounter;
int rowTemp;
int columnTemp;
int whiteTurn = 1;
int loadedGame = 0;
bool loaded = false;
int winner = 0;
bool gameEnd = false;
char player1[17];
char player2[17];
char fileName[3][16];
int whiteScore = 0;
int blackScore = 0;
int choice = 0;

//Bitmaps
BITMAP *game_board = NULL;
BITMAP *blackstone = NULL;
BITMAP *whitestone = NULL;

//Dialog object using allegro
DIALOG player_name1[3] = 
{
	//dialog proc)       (x)   (y)   (w)   (h)  (fg) (bg) (key) (flags)     (d1) (d2)    (dp)                   (dp2) (dp3) */
	{ d_edit_proc,       410,  283,  138,   16,   0,  0,    0,      0,      16,   0,    (void*)player1,         NULL, NULL },
	{ d_button_proc,     417,  364,  126,   27,   0,  0,    0, D_EXIT,       0,   0,    (void*)"Submit",    	 NULL, NULL },
	{ d_yield_proc,        0,    0,    0,    0,   0,  0,    0,      0,       0,   0,     NULL,             		 NULL, NULL }
};

DIALOG player_name2[3] = 
{
	//dialog proc)       (x)   (y)   (w)   (h)  (fg) (bg) (key) (flags)     (d1) (d2)    (dp)                   (dp2) (dp3) */
	{ d_edit_proc,       410,  283,  138,   16,   0,  0,    0,      0,      16,   0,    (void*)player2,         NULL, NULL },
	{ d_button_proc,     417,  364,  126,   27,   0,  0,    0, D_EXIT,       0,   0,    (void*)"Submit",    	 NULL, NULL },
	{ d_yield_proc,        0,    0,    0,    0,   0,  0,    0,      0,       0,   0,     NULL,             		 NULL, NULL }
};

int main(int argc, char *argv[])
{
	init_all();
	int gameOutcome = 0;
	while(replay)
	{
		
		while(choice != 1)
		{
			choice = show_menu();
			if(key[KEY_ESC]){
			break;
			
		}
			if(choice == 2)
			{
				show_rules();
			}
		}
		
		if(key[KEY_ESC]){
			break;
		}
		
		if(choice == 1)
		{
			gameEnd = false;
			gameOutcome = start_game();
			end_screen(gameOutcome);
		}
	
	}
	
	destroy_bitmap(game_board);	
	destroy_bitmap(blackstone);
	destroy_bitmap(whitestone);
	return 0;
}
END_OF_MAIN()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void init_all()
{
	int i = 0;
	int j = 0;
	stoneCounter = 0;
	rowTemp = 0;
	columnTemp = 0;
	allegro_init();
	install_keyboard();
	install_mouse();
	set_color_depth(desktop_color_depth());
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 960, 720, 0 , 0);
	
	game_board = load_bitmap("GoGameBoard.bmp", NULL);
	blackstone = load_bitmap("BlackGoStone.bmp", NULL);
	whitestone = load_bitmap("WhiteGoStone.bmp", NULL);
	
	strcpy(fileName[0], "save_file_1.txt");
	strcpy(fileName[1], "save_file_2.txt");
	strcpy(fileName[2], "save_file_3.txt");
	
	//Initialize board variable
	for(i = 0 ; i < 9 ; i++)
	{
		for(j = 0 ; j < 9 ; j++)
		{
			gameboard[i][j].stoneType = 0;
			gameboard[i][j].groupAddress = 0;
			gameboard[i][j].liberties = 0;
			totalLiberties[200] = 0;
			
			gameboard[i][j].placeX = 148 + j * 62;
			gameboard[i][j].placeY = 133 + i * 62;
			gameboard[i][j].bb_bottom = gameboard[i][j].placeY + 15;
			gameboard[i][j].bb_left = gameboard[i][j].placeX - 15;
			gameboard[i][j].bb_right = gameboard[i][j].placeX + 15;
			gameboard[i][j].bb_top = gameboard[i][j].placeY - 15;
	//		rect(game_board, gameboard[i][j].placeX - 120- 15, gameboard[i][j].placeY -60-15, gameboard[i][j].placeX-120 + 15 ,gameboard[i][j].placeY -60+ 15, makecol(0,0,0));
		}
	}
//	allegro_message("done");
	
	//Dialog color
	set_dialog_color(player_name1, makecol(0, 0, 0), makecol(255, 255, 255));
	set_dialog_color(player_name2, makecol(0, 0, 0), makecol(255, 255, 255));
}
END_OF_FUNCTION(init_all);

int show_menu()
{
	BITMAP *main_menu = NULL;
	main_menu = load_bitmap("MainMenu.bmp", NULL);
	
	int userChoice = 0;
	bool loadgame = false;
	
//	if(main_menu == NULL)
//	{
//		allegro_message("fail");
//	}
//	else
//	{
//		allegro_message("done");
//	}
	
	blit(main_menu, screen, 0, 0, 0, 0, 960, 720);
	show_mouse(screen);
	while(!key[KEY_ESC])
	{
		if(mouse_b & 1){
			if(mouse_x >= 353 && mouse_x <= 604 && mouse_y >= 385 && mouse_y <= 431)
			{
				clear(screen);
				input_names();
				userChoice = 1;
				break;
			}
			else if(mouse_x >= 353 && mouse_x <= 604 && mouse_y >= 469 && mouse_y <= 515)
			{
				clear(screen);
				loadgame = load_data();
				if(loadgame)
				{
					loaded = true;
					userChoice = 1;
				}
				break;
			}
			else if(mouse_x >= 353 && mouse_x <= 604 && mouse_y >= 552 && mouse_y <= 598)
			{
				clear(screen);
				userChoice = 2;
				break;
			}
			else if(mouse_x >= 353 && mouse_x <= 604 && mouse_y >= 628 && mouse_y <= 674)
			{
				clear(screen);
				
				userChoice = 3;
				break;
			}
		}
	}
	destroy_bitmap(main_menu);
	return userChoice;
}
END_OF_FUNCTION(show_menu);

void show_rules()
{	
	BITMAP *rules = NULL;
	rules = load_bitmap("Rules page.bmp", NULL);
	FILE *fileptr;
	int i = 0;
	bool leave = true;
	char string[15] = "";
	int ruleX = 100;
	int ruleY = 150;
	fileptr = fopen("Rules.txt", "r");
	if(fileptr == NULL){
		allegro_message("fileptr not opening stuffs");
	}
	
	blit(rules, screen, 0, 0, 0, 0, 960, 720);
	while(leave){
		while(fscanf(fileptr, "%s", string) != EOF){
			
			if(i == 15){
			ruleY += 20;
			i = 0;
			ruleX = 100;	
			}
			textprintf_ex(screen, font, ruleX, ruleY, makecol(0, 0, 0), makecol(208, 224, 227), "%s", string);
			i++;
			ruleX += (strlen(string)+1)*8;
		}
		rect(screen, 715, 650, 920, 690, makecol(0, 0, 0));
		if(mouse_b & 1){
			if(mouse_x >= 714 && mouse_x <= 921 && mouse_y >=649 && mouse_y <= 691){
				returntomain();
				leave = false;
			}
		}
	}
	fclose(fileptr);
	destroy_bitmap(rules);
	
		
}
END_OF_FUNCTION(show_rules)

bool load_data()
{
	int fileNum = 0;
	int i = 0;
	int j = 0;
	char name1[17];
	char name2[17];
	bool load = false;
	bool selected = false;
	BITMAP *load_screen = NULL;
	load_screen = load_bitmap("LoadScreen.bmp", NULL);
	
	strcpy(name1, "");
	strcpy(name2, "");
	
	draw_sprite(screen, load_screen, 230, 100);
	for(i = 0 ; i < 3 ; i++)
	{
		fptr = fopen(fileName[i], "r");
		fscanf(fptr, "%s %s", name1, name2);
		fclose(fptr);
		textprintf_centre_ex(screen, font, 480, 190 + i * 54, makecol(0, 0, 0), makecol(255, 255, 255), "---%s vs %s---[save %d]", name1, name2, i + 1);
		strcpy(name1, "");
		strcpy(name2, "");
	}
	while(!selected)
	{
		if(mouse_b & 1){
			if(mouse_x >= 378 && mouse_x <= 581 && mouse_y >= 178 && mouse_y <= 214)
			{
				fileNum = 0;
				selected = true;
			}
			else if(mouse_x >= 378 && mouse_x <= 581 && mouse_y >= 232 && mouse_y <= 268)
			{
				fileNum = 1;
				selected = true;
			}
			else if(mouse_x >= 378 && mouse_x <= 581 && mouse_y >= 286 && mouse_y <= 322)
			{
				fileNum = 2;
				selected = true;
			}
		}
		if(key[KEY_ESC])
		{
			break;
		}
	}
	
	if(selected)
	{
		fptr = fopen(fileName[fileNum], "r");
		fscanf(fptr, "%s %s", player1, player2);
		fscanf(fptr, "%d %d", &whiteTurn, &stoneCounter);
		
		for(i = 0 ; i < 200 ; i++)
		{
			fscanf(fptr, "%d", &totalLiberties[i]);
		}
		
		for(i = 0 ; i < 9 ; i++)
		{
			for(j = 0 ; j < 9 ; j++)
			{
				fscanf(fptr, "%d %d %d", &gameboard[i][j].stoneType, &gameboard[i][j].groupAddress, &gameboard[i][j].liberties);
			}
		}
		loadedGame = fileNum;
		load = true;
		fclose(fptr);
	}
	
	return load;
}

void input_names()
{
	BITMAP *name_screen = NULL;
	name_screen = load_bitmap("WaitScreen.bmp", NULL);
	draw_sprite(screen, name_screen, 0, 0);
	
	//player 1 name
	textprintf_ex(screen, font, 358, 194, makecol(0, 0 ,0), makecol(255, 255, 255), "Player 1, please enter your name");
	do_dialog(player_name1, 0);
	if(strcmp(player1, "") == 0)
	{
		strcpy(player1, "Player 1");
	}
	allegro_message("%s", player1);
	
	clear(screen);
	//player 2 name
	draw_sprite(screen, name_screen, 0, 0);
	textprintf_ex(screen, font, 358, 194, makecol(0, 0 ,0), makecol(255, 255, 255), "Player 2, please enter your name");
	do_dialog(player_name2, 0);
	if(strcmp(player2, "") == 0)
	{
		strcpy(player2, "Player 2");
	}
	allegro_message("%s", player2);
	clear(screen);
	destroy_bitmap(name_screen);
}

int start_game()
{
	int i = 0;
	int clickX = 0;
	int clickY = 0;
	int stonePlaced = 0;
	int gameResult = 0;
	// 0 means not finished (quit), 1 means finished, 2 means saved
	bool save = false;
	
	board_draw();
	textprintf_ex(screen, font, 146, 85, makecol(0, 0, 0), makecol(224, 160, 64), "%s's (black) turn", player1);
	while(!gameEnd)
	{
		while(mouse_b & 1)
		{
			clickX = mouse_x;
			clickY = mouse_y;
		}
		stonePlaced = click_check(clickX, clickY);
		if(stonePlaced == 1)
		{
			group_stones();
			join_groups();
	//		printf("bleh3");
		//	printf("done");
			check_liberties();
		//	printf("done");
			add_liberties();
		//	printf("done");
			if(whiteTurn % 2 == 0)
			{
				remove_black();
			}
			else
			{
				remove_white();
			}
		//	printf("done");
			board_draw();
		//	printf("clicked");			//debugging
			stoneCounter++;
		//	printf("added");
			whiteTurn++;
		//	printf("added");
		}
		else if(stonePlaced == 2)
		{
			if(whiteTurn % 2 == 0)
			{
				winner = 2;
			}
			else if(whiteTurn % 2 == 1)
			{
				winner = 1;
			}
			
			clear(screen);
			gameResult = 1;
			gameEnd = true;
		}
		else if(stonePlaced == 3)
		{
			clear(screen);
			save = save_game();
			if(save)
			{
				gameEnd = true;
			}
			gameResult = 2;
			board_draw();
		}
		else if(stonePlaced == 4)
		{
			clear(screen);
			count_score();
			gameResult = 1;
			gameEnd = true;
		}
		// resetting and setting values
		clickX = 0;
		clickY = 0;
		stonePlaced = 0;
	}
	clear(screen);
	return gameResult;
}
END_OF_FUNCTION(start_game)

void returntomain(){
	clear(screen);
	choice = 0;
}

int click_check(int clickX, int clickY)
{
	int i = 0;
	int j = 0;
	int clicked = 0;
//	printf("bleh");
	for(i = 0 ; i < 9 ; i++)
	{
		for(j = 0 ; j < 9 ; j++)
		{
			if(clickX >= gameboard[i][j].bb_left && clickX <= gameboard[i][j].bb_right && clickY >= gameboard[i][j].bb_top && clickY <= gameboard[i][j].bb_bottom && gameboard[i][j].stoneType == 0)
			{
				gameboard[i][j].groupAddress = stoneCounter;
				rowTemp = i;
				columnTemp = j;
				if(whiteTurn % 2 == 0)
				{
					gameboard[i][j].stoneType = 2;
				}
				else
				{
					gameboard[i][j].stoneType = 1;
				}
				clicked = 1;
			}
		}
	}
	if(clickX >= 669 && clickY >= 501 && clickX <= 830 && clickY <= 563)
	{
		clicked = 2;
	}
	else if(clickX >= 669 && clickY >= 427 && clickX <= 830 && clickY <= 489)
	{
		clicked = 3;
	}
	else if(clickX >= 669 && clickY >= 576 && clickX <= 830 && clickY <= 638)
	{
		clicked = 4;
	}
	return clicked;
}
END_OF_FUNCTION(click_check);

void group_stones()
{
	int i = rowTemp;
	int j = columnTemp;
	//groups stones when placed
	if(gameboard[i][j].stoneType == gameboard[i][j + 1].stoneType && j + 1 < 9)
	{
		gameboard[i][j].groupAddress = gameboard[i][j + 1].groupAddress;
	}
//check down
	else if(gameboard[i][j].stoneType == gameboard[i + 1][j].stoneType && i + 1 < 9)
	{
		gameboard[i][j].groupAddress = gameboard[i + 1][j].groupAddress;
	}
//check left

	else if(gameboard[i][j].stoneType == gameboard[i][j - 1].stoneType && j - 1 >= 0)
	{
		gameboard[i][j].groupAddress = gameboard[i][j - 1].groupAddress;
	}
//check up

	else if(gameboard[i][j].stoneType == gameboard[i - 1][j].stoneType && i - 1 >= 0)
	{
		gameboard[i][j].groupAddress = gameboard[i - 1][j].groupAddress;
	}
}
END_OF_FUNCTION(group_stones)

void join_groups()
{
	int i = rowTemp;
	int j = columnTemp;
	int k = 0;
	int l = 0;
	int group1 = 0;
	int group2 = 0;
	int group3 = 0;
	int group4 = 0;
	//groups other groups that are next to a certain group together
	
	//checks for nearby groups
	if(gameboard[i][j].stoneType == gameboard[i][j + 1].stoneType && j + 1 < 9 && gameboard[i][j].groupAddress != gameboard[i][j + 1].groupAddress)
	{
		group1 = gameboard[i][j + 1].groupAddress;
	}
//check down
	if(gameboard[i][j].stoneType == gameboard[i + 1][j].stoneType && i + 1 < 9 && gameboard[i][j].groupAddress != gameboard[i + 1][j].groupAddress)
	{
		group2 = gameboard[i + 1][j].groupAddress;
	}
//check left
	if(gameboard[i][j].stoneType == gameboard[i][j - 1].stoneType && j - 1 >= 0 && gameboard[i][j].groupAddress != gameboard[i][j - 1].groupAddress)
	{
		group3 = gameboard[i][j - 1].groupAddress;
	}
//check up
	if(gameboard[i][j].stoneType == gameboard[i - 1][j].stoneType && i - 1 >= 0 && gameboard[i][j].groupAddress != gameboard[i - 1][j].groupAddress)
	{
		group4 = gameboard[i - 1][j].groupAddress;
	}
	//joins nearby groups
	for(k = 0 ; k < 9 ; k++)
	{
		for(l = 0 ; l < 9 ; l++)
		{
			if(gameboard[k][l].groupAddress == group1 && group1 != 0)
			{
				gameboard[k][l].groupAddress = gameboard[i][j].groupAddress;
			}
			else if(gameboard[k][l].groupAddress == group2 && group2 != 0)
			{
				gameboard[k][l].groupAddress = gameboard[i][j].groupAddress;
			}
			else if(gameboard[k][l].groupAddress == group3 && group3 != 0)
			{
				gameboard[k][l].groupAddress = gameboard[i][j].groupAddress;
			}
			else if(gameboard[k][l].groupAddress == group4 && group4 != 0)
			{
				gameboard[k][l].groupAddress = gameboard[i][j].groupAddress;
			}
		}
	}
	
}
END_OF_FUNCTION(join_groups)

void add_liberties()
{
	int i = 0;
	int j = 0;
	int k = 0;
	
	for(k = 0 ; k < stoneCounter ; k++)
	{
		totalLiberties[k] = 0;
	}
	
	for(i = 0 ; i < 9 ; i++)
	{
		for(j = 0 ; j < 9 ; j++)
		{
			if(gameboard[i][j].stoneType > 0)
			{
				totalLiberties[gameboard[i][j].groupAddress] += gameboard[i][j].liberties;
			}
		}
	}
//	printf("bleh4");
}
END_OF_FUNCTION(add_liberties)

void check_liberties()
{
	int i = 0;
	int j = 0;
	//this loop counts all the liberties of the group/single stones
	for(i = 0 ; i < 9 ; i++)
	{
		for(j = 0 ; j < 9 ; j++)
		{
			gameboard[i][j].liberties = 0;
			if(gameboard[i][j].stoneType > 0)
			{
				//check right
				if(j + 1 < 9)
				{
					if(gameboard[i][j + 1].stoneType == 0)
					{
						gameboard[i][j].liberties++;
					}
				}
				//check down
				if(i + 1 < 9)
				{
					if(gameboard[i + 1][j].stoneType == 0)
					{
						gameboard[i][j].liberties++;
					}
				}
				//check up
				if(i - 1 >= 0)
				{
					if(gameboard[i - 1][j].stoneType == 0)
					{
						gameboard[i][j].liberties++;
					}
				}
				//check left
				if(j - 1 >= 0)
				{
					if(gameboard[i][j - 1].stoneType == 0)
					{
						gameboard[i][j].liberties++;
					}
				}
			}
		}
	}
	
}
END_OF_FUNCTION(check_liberties)

void board_draw()
{
	int i = 0;
	int j = 0;
	
	draw_sprite(screen, game_board, 120, 60);
	for(i = 0 ; i < 9 ; i++)
	{
		for(j = 0 ; j < 9 ; j++)
		{
			if(gameboard[i][j].stoneType == 1)
			{
				draw_sprite(screen, blackstone, (gameboard[i][j].placeX - blackstone->w / 2), (gameboard[i][j].placeY - blackstone->h / 2));
			}
			else if(gameboard[i][j].stoneType == 2)
			{
				draw_sprite(screen, whitestone, (gameboard[i][j].placeX - whitestone->w / 2), (gameboard[i][j].placeY - whitestone->h / 2));
			}
		}
	}
	if(whiteTurn % 2 == 0)
	{
		textprintf_ex(screen, font, 146, 85, makecol(0, 0, 0), makecol(224, 160, 64), "%s's (black) turn", player1);
	}
	else
	{
		textprintf_ex(screen, font, 146, 85, makecol(0, 0, 0), makecol(224, 160, 64), "%s's (white) turn", player2);
	}
	//reloads the board and the newly placed/removed stones at the end of each turn
}
END_OF_FUNCTION(board_draw)

void remove_white()
{
	int i = 0;
	int j = 0;	

	for(i = 0 ; i < 9 ; i++)
	{
		for(j = 0 ; j < 9 ; j++)
		{
			if(totalLiberties[gameboard[i][j].groupAddress] == 0 && gameboard[i][j].stoneType == 2)
			{
				gameboard[i][j].stoneType = 0;
			}
		}
	}
	//checks for stone groups with 0 liberties and removes them by resetting their values to blank
}    
END_OF_FUNCTION(remove_white)

void remove_black()
{
	int i = 0;
	int j = 0;	

	for(i = 0 ; i < 9 ; i++)
	{
		for(j = 0 ; j < 9 ; j++)
		{
			if(totalLiberties[gameboard[i][j].groupAddress] == 0 && gameboard[i][j].stoneType == 1)
			{
				gameboard[i][j].stoneType = 0;
			}
		}
	}
}
END_OF_FUNCTION(remove_black)

bool save_game()
{	
	bool saved = false;
	bool selected = false;
	int fileNum = 0;
	int i = 0;
	int j = 0;
	
	draw_savescreen();
	
	while(!selected)
	{
		if(mouse_b & 1){
			if(mouse_x >= 378 && mouse_x <= 581 && mouse_y >= 178 && mouse_y <= 214)
			{
				fileNum = 0;
				selected = save_confirm();
				draw_savescreen();
			}
			else if(mouse_x >= 378 && mouse_x <= 581 && mouse_y >= 232 && mouse_y <= 268)
			{
				fileNum = 1;
				selected = save_confirm();
				draw_savescreen();
			}
			else if(mouse_x >= 378 && mouse_x <= 581 && mouse_y >= 286 && mouse_y <= 322)
			{
				fileNum = 2;
				selected = save_confirm();
				draw_savescreen();
			}
		}
		if(key[KEY_ESC])
		{
			break;
		}
	}
	
	if(selected)
	{
		fptr = fopen(fileName[fileNum], "w");
		fprintf(fptr, "%s %s\n", player1, player2);
		fprintf(fptr, "%d %d\n", whiteTurn, stoneCounter);
		for(i = 0 ; i < 200 ; i++)
		{
			fprintf(fptr, "%d\n", totalLiberties[i]);
		}
		for(i = 0 ; i < 9 ; i++)
		{
			for(j = 0 ; j < 9 ; j++)
			{
				fprintf(fptr, "%d %d %d\n", gameboard[i][j].stoneType, gameboard[i][j].groupAddress, gameboard[i][j].liberties);
			}
		}
		saved = true;
	}
	clear(screen);
	return saved;
}
END_OF_FUNCTION(save_game)

void draw_savescreen()
{
	BITMAP *save_popup = NULL;
	save_popup = load_bitmap("SaveScreen.bmp", NULL);
	
	int i = 0;
	char name1[17];
	char name2[17];
	strcpy(name1, "");
	strcpy(name2, "");
	
	draw_sprite(screen, save_popup, 230, 100);
	for(i = 0 ; i < 3 ; i++)
	{
		fptr = fopen(fileName[i], "r");
		fscanf(fptr, "%s %s", name1, name2);
		fclose(fptr);
		textprintf_centre_ex(screen, font, 480, 190 + i * 54, makecol(0, 0, 0), makecol(255, 255, 255), "---%s vs %s---[save %d]", name1, name2, i + 1);
		strcpy(name1, "");
		strcpy(name2, "");
	}
	destroy_bitmap(save_popup);
}
END_OF_FUNCTION(draw_savescreen)

bool save_confirm()
{
	BITMAP *confirm_popup = NULL;
	confirm_popup = load_bitmap("ConfirmScreen.bmp", NULL);
	
	bool yes = false;
	
	draw_sprite(screen, confirm_popup, 330, 180);
	while(!key[KEY_ESC])
	{
		if(mouse_b & 1){
			if(mouse_x >= 359 && mouse_x <= 443 && mouse_y >= 302 && mouse_y <= 339)
			{
				yes = true;
				break;
			}
			else if(mouse_x >= 515 && mouse_x <= 599 && mouse_y >= 303 && mouse_y <= 340)
			{
				yes = false;
				break;
			}
		}
	}
	clear(screen);
	destroy_bitmap(confirm_popup);
	return yes;
}
END_OF_FUNCTION(save_confirm)


void count_score()
{
	int i = 0;
	int j = 0;
	
	for(i = 0 ; i < 9 ; i++)
	{
		for(j = 0 ; j < 9 ; j++)
		{
			if(gameboard[i][j].stoneType == 1)
			{
				blackScore++;
			}
			else if(gameboard[i][j].stoneType == 2)
			{
				whiteScore++;
			}
		}
	}
	
	if(blackScore > whiteScore)
	{
		winner = 1;
	}
	else if(blackScore < whiteScore)
	{
		winner = 2;
	}
	else if(blackScore == whiteScore)
	{
		winner = 3;
	}
	clear(screen);
}
END_OF_FUNCTION(count_score)


void end_screen(int gameOutcome)
{
	
	BITMAP *last_screen = NULL;
	BITMAP *buffer = create_bitmap(960, 720);
	last_screen = load_bitmap("EndScreen.bmp", NULL);
	//shows when the game is over
	
		draw_sprite(screen, last_screen, 0, 0);
		
		if(gameOutcome == 1)
		{
			if(loaded)
			{
				fptr = fopen(fileName[loadedGame], "w");
				fprintf(fptr, "");
				fclose(fptr);
			}
			if(winner == 1)
			{
				textprintf_centre_ex(buffer, font, 480, 202, makecol(0, 0, 0), makecol(200, 191, 231), "Congratulations %s (black)!", player1);
				textprintf_centre_ex(screen, font, 480, 238, makecol(0, 0, 0), makecol(200, 191, 231), "You beat %s (white)", player2);
				textprintf_centre_ex(screen, font, 480, 274, makecol(0, 0, 0), makecol(200, 191, 231), "Black %d - %d White", blackScore, whiteScore);
			}
			else if(winner == 2)
			{
				textprintf_centre_ex(screen, font, 480, 202, makecol(0, 0, 0), makecol(200, 191, 231), "Congratulations %s (white)!", player2);
				textprintf_centre_ex(screen, font, 480, 238, makecol(0, 0, 0), makecol(200, 191, 231), "You beat %s (black)", player1);
				textprintf_centre_ex(screen, font, 480, 274, makecol(0, 0, 0), makecol(200, 191, 231), "Black %d - %d White", blackScore, whiteScore);
			}
			else if(winner == 3)
			{
				textprintf_centre_ex(buffer, font, 480, 238, makecol(0, 0, 0), makecol(200, 191, 231), "The match was a tie...");
				textprintf_centre_ex(screen, font, 480, 238, makecol(0, 0, 0), makecol(200, 191, 231), "The match was a tie...");
			}
		}
		else if(gameOutcome == 2)
		{
			textprintf_centre_ex(screen, font, 480, 202, makecol(0, 0, 0), makecol(200, 191, 231), "Game Saved successfuly!");
			textprintf_centre_ex(screen, font, 480, 238, makecol(0, 0, 0), makecol(200, 191, 231), "Come back again next time!");
		}
		strcpy(player1, "");
		strcpy(player2, "");
		for(int i = 0 ; i < 9 ; i++)
		{
			for(int j = 0 ; j < 9 ; j++)
			{
				gameboard[i][j].stoneType = 0;
				gameboard[i][j].groupAddress = 0;
				gameboard[i][j].liberties = 0;
				totalLiberties[200] = 0;
			}
		}
		wait();
		returntomain();
}
END_OF_FUNCTION(end_screen)



void wait()
{
	int i = 0;
	while(!mouse_b)
	{
		i++;
		i--;
	}
}
