#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <string>
#include <cmath>
#include <fstream>
#include <thread>

using namespace std;

struct Player {  // a struct for player data
	string name;
	int score;
	int n;
	int** board;
};

//////////////////////////
// Timer
//////////////////////////

struct Timer{
    int h;
    int m;
    int s;
    bool TSU; // time is up!
};

Timer t;

// h:houre  m:minute s:second 
void timer(){
    while (true)
    {
        Sleep(999);
    
        if (t.s > 0)
            t.s--;
        else{
            if (t.m > 0){
                t.m--;
                t.s = 59;
            }
            else{
                if (t.h > 0){
                    t.h--;
                    t.m = 59;
                    t.s = 59;
                }
                else{
                    t.TSU = true;
                    return;
                }
            }
        }
    }
}

string remaningGameTime(){
    string str = "";
    str = str + to_string(t.m) + ":";
    str = str + to_string(t.s);
    return str;
}

Player pl;
int bestScore = 0;

bool helpMenu = false; // show how to use menu in last line
void printMenu(char ch); // print menu
void printMenuButton(string str, bool isOn); // print a button of menu
void changeChoice(char &ch, char upDown); // change the selected button
void pressEnter(char ch); // which button is selected?
void game(); // main function for game (new game)

int main(){
    srand(static_cast<unsigned int>(time(nullptr)));
    char choice = 'n';
    while (true){
        system("cls");
        printMenu(choice);
        char ch = getch();
        switch (ch)
        {
        case 72:
            changeChoice(choice, 'u');
            Beep(260, 500);
            break;
        case 119:
            changeChoice(choice, 'u');
            Beep(260, 500);
            break;
        case 80:
            changeChoice(choice, 'd');
            Beep(260, 500);
            break;
        case 115:
            changeChoice(choice, 'd');
            Beep(260, 500);
            break;
        case 13:
            Beep(300, 500);
            if (choice == 'e')
                return 0;
            pressEnter(choice);
            Beep(260, 500);
            break;
        case 32:
            Beep(300, 500);
            if (choice == 'e')
                return 0;
            pressEnter(choice);
            Beep(260, 500);
            break;
        default:
            helpMenu = true;
            break;
        }                                                                                
    }
}

//////////////////////////////////////////
// menu
//////////////////////////////////////////

// change the selected button
void changeChoice(char &ch, char upDown){
    if (upDown == 'u'){
        switch (ch)
        {
        case 'n':
            ch = 'e';
            break;
        case 'l':
            ch = 'n';
            break;
        case 'e':
            ch = 'l';
            break;
        }
    }
    else{
        switch (ch)
        {
        case 'n':
            ch = 'l';
            break;
        case 'l':
            ch = 'e';
            break;
        case 'e':
            ch = 'n';
            break;
        }
    }
}

// print a new empty line in menu
void printNewMenuLine(){
    cout << "\u001b[46m\u001b[93m\u2551";
    for (int i = 0; i < 68; i++)
        cout << " ";
    cout << "\u2551\u001b[0m" << endl;
}

// print the first and last line of a button
void printMenuButton_UpDown(bool isUp, bool isOn){
    cout << "\u001b[46m\u001b[93m\u2551";
    for (int i = 0; i < 26; i++)
        cout << " ";

    if (isUp){
        cout << "\u001b[97m";
        if (isOn)
            cout << "\u001b[106m";
        cout << "\u250F";
    }
    else{
        cout << "\u001b[97m";
        if (isOn)
            cout << "\u001b[106m";
        cout << "\u2517";
    }
    for (int i = 0; i < 14; i++)
	    cout << "\u2501";
    if (isUp)
        cout << "\u2513\u001b[46m\u001b[93m";
    else
        cout << "\u251B\u001b[46m\u001b[93m";

    for (int i = 0; i < 26; i++)
        cout << " ";
    cout << "\u2551\u001b[0m" << endl;
}

// print a button o menu
void printMenuButton(string str, bool isOn){
    int space = (14 - str.length())/2;

    printMenuButton_UpDown(true, isOn);

    cout << "\u001b[46m\u001b[93m\u2551";
    if (isOn){
        for (int i = 0; i < 24; i++)
            cout << " ";
        cout << ">>";
    }
    else
        for (int i = 0; i < 26; i++)
            cout << " ";
    
    cout << "\u001b[97m";
    if (isOn)
            cout << "\u001b[106m";
    cout << "\u2502";

    for (int i = 0; i < space; i++)
        cout << " ";
    cout << str;
    for (int i = 0; i < space; i++)
        cout << " ";
    cout << "\u2502\u001b[93m\u001b[46m";

    if (isOn){
        cout << "<<";
        for (int i = 0; i < 24; i++)
            cout << " ";
    }
    else
        for (int i = 0; i < 26; i++)
            cout << " ";
    cout << "\u2551\u001b[0m" << endl;

    printMenuButton_UpDown(false, isOn);
}

// print menu
void printMenu(char ch){
    cout << "\u001b[46m\u001b[93m\u2554";
    for (int i = 0; i < 68; i++)
        cout << "\u2550";
    cout << "\u2557\u001b[0m" << endl;

    printNewMenuLine();

    cout << "\u001b[46m\u001b[93m\u2551          " << "  _______   ________   ___   ___   ________               \u2551\u001b[0m" << endl;
    cout << "\u001b[46m\u001b[93m\u2551          " <<  " /  ___  \\\\|\\   __  \\ |\\  \\ |\\  \\ |\\   __  \\              \u2551\u001b[0m" << endl;
    cout << "\u001b[46m\u001b[93m\u2551          " << "/__/|_/  //\\ \\  \\|\\  \\\\ \\  \\\\_\\  \\\\ \\  \\|\\  \\             \u2551\u001b[0m" << endl;
    cout << "\u001b[46m\u001b[93m\u2551          " << "|__|//  //  \\ \\  \\\\\\  \\\\ \\______  \\\\ \\   __  \\            \u2551\u001b[0m" << endl;
    cout << "\u001b[46m\u001b[93m\u2551          " << "    /  //___ \\ \\  \\\\\\  \\\\|_____|\\  \\\\ \\  \\|\\  \\           \u2551\u001b[0m" << endl;
    cout << "\u001b[46m\u001b[93m\u2551          " << "   |\\________\\\\ \\_______\\      \\ \\__\\\\ \\_______\\          \u2551\u001b[0m" << endl;
    cout << "\u001b[46m\u001b[93m\u2551          " << "    \\|_______| \\|_______|       \\|__| \\|_______|          \u2551\u001b[0m" << endl;

    printNewMenuLine();
    printNewMenuLine();
    
    switch (ch)
    {
    case 'n':
        printMenuButton("New Game", true);
        printNewMenuLine();
        printMenuButton("Leader Board", false);
        printNewMenuLine();
        printMenuButton("Exit", false);
        printNewMenuLine();
        break;
    case 'l':
        printMenuButton("New Game", false);
        printNewMenuLine();
        printMenuButton("Leader Board", true);
        printNewMenuLine();
        printMenuButton("Exit", false);
        printNewMenuLine();
        break;
    case 'e':
        printMenuButton("New Game", false);
        printNewMenuLine();
        printMenuButton("Leader Board", false);
        printNewMenuLine();
        printMenuButton("Exit", true);
        printNewMenuLine();
        break;
    }
    
    if (helpMenu){
        cout << "\u001b[46m\u001b[93m\u2551 \u001b[97m Use w s  or  arrow up  arrow down\u001b[46m\u001b[93m";
        for (int i = 0; i < 33; i++)
            cout << " ";
        cout << "\u2551\u001b[0m" << endl;

        cout << "\u001b[46m\u001b[93m\u255A";
        for (int i = 0; i < 68; i++)
            cout << "\u2550";
        cout << "\u255D\u001b[0m";
    }
    else{
        cout << "\u001b[46m\u001b[93m\u255A";
        for (int i = 0; i < 68; i++)
            cout << "\u2550";
        cout << "\u255D\u001b[0m";
    }                                            
}

void preparePlayer();
void save(); // save player data to a file
void printLeaderBoard();

// which button is selected?
void pressEnter(char ch){
    if (ch == 'n'){
        preparePlayer();
        save();
        delete[] pl.board;
    }
    else if (ch == 'l'){
        printLeaderBoard();
    }
}

//////////////////////////////////////////
// Game
//////////////////////////////////////////

// print the player score and the best score
void printScore() {
    cout << "\u001b[36m";
    for (int i = 0; i < 23; i++)
        cout << "\u2550";
    cout << endl << "Score: \u001b[96m" << pl.score << endl;
    cout << "\u001b[36mBest Score: \u001b[96m" << bestScore << endl;
    cout << "\u001b[36m";
    for (int i = 0; i < 23; i++)
        cout << "\u2550";
    cout << endl << endl << endl;
}

// how many zeros are there in the game board
int zeroFinder() {
	int count = 0;
	for (int i = 0; i < pl.n; i++)
		for (int j = 0; j < pl.n; j++)
			if (pl.board[i][j] == 0)
				count++;
	return count;
}

// put a random 2 or 4 in game
void randNumGen() {
    int empNum = zeroFinder(); // number of empty cells
    int* I = new int[empNum];  // an array for saving the i of empty cells
    int* J = new int[empNum];  // an array for saving the j of empty cells

    int index = 0;
    for (int i = 0; i < pl.n; i++)
        for (int j = 0; j < pl.n; j++)
            if (pl.board[i][j] == 0) {
                I[index] = i;
                J[index] = j;
                index++;
            }

    int randCell = rand() % empNum;

    int randNum = rand() % 4;
    switch (randNum)
    {
    case 0:
        randNum = 2;
        break;
    case 1:
        randNum = 2;
        break;
    case 2:
        randNum = 2;
        break;
    case 3:
        randNum = 4;
        break;
    }

    pl.board[I[randCell]][J[randCell]] = randNum;
    delete[] I;
    delete[] J;
}

// create game matrix and put two random number
void prepareGameBoard() {
	pl.board = new int* [pl.n];
	for (int i = 0; i < pl.n; i++) {
		pl.board[i] = new int[pl.n];
		for (int j = 0; j < pl.n; j++)
			pl.board[i][j] = 0;
	}
    randNumGen();
    randNumGen();
}
int recNum();

// getting player's name and game size
void preparePlayer(){
    system("cls");
    
    pl.score = 0;

    while (true)
    {
        cout << "\u001b[93m >>\u001b[36m Please Enter your name: \u001b[96m"; // Entering the name of player
        string name;
        getline(cin, name);
        int len = name.length();
        Beep(260, 500);
        if (len != 0){
            pl.name = name;
            break;
        }
        else
            cout << "\u001b[91m INVALID!" << endl;
    }
    
    int n = 0;
    while (n < 2)
    {
        cout << endl << "\u001b[93m >>\u001b[36m Please Enter the game board size: \u001b[96m"; // Entering n (n is the size of game matrix)
        cin >> n;
        Beep(260, 500);
        if (n < 2)
            cout << "\u001b[91m INVALID!" << endl;
    }
    pl.n = n;
    
    
    // How to play?
    system("cls");
    cout << "\u001b[36mHow to play?" << endl;
    cout << "--------------------------------------------------------------" << endl;
    cout << "\u001b[96mYou will see a nxn screen with some numbers in some houses." << endl;
    cout << "In each turn, you can move up, down, left and right with the wasd keys." << endl;
    cout << "The game will continue until it is no longer possible to move!" << endl << endl;
    cout << "(Press b to return to the menu)" << endl << endl;
    cout << "Press any key to continue...";
    getch();
    Beep(260, 500);
    cout << endl << endl;

    // loading...
    cout << "\u001b[92m Loading";
    for (int i = 0; i < 3; i++) {
        Sleep(200);
        cout << ".";
    }

    Sleep(300);
    system("cls");

    prepareGameBoard();

    int count = recNum();
    ifstream bestScoreFileInput("rec.txt", ios::in);
    if (!bestScoreFileInput.fail())
    {
        Player* players = new Player[count];
        for (int i = 0; i < count; i++){
                getline(bestScoreFileInput, players[i].name);
                string score, n;
                getline(bestScoreFileInput, score);
                getline(bestScoreFileInput, n);
                players[i].score = stoi(score);
                players[i].n = stoi(n);
        }

        bestScore = players[0].score;
        for (int i = 0; i < count; i++)
            if (players[i].score > bestScore && players[i].n == pl.n)
                bestScore = players[i].score;
    }

    game();
}

// find the biggest number of game board
int findBiggest() {
    int biggest = pl.board[0][0];
    for (int i = 0; i < pl.n; i++)
        for (int j = 0; j < pl.n; j++)
            if (pl.board[i][j] > biggest)
                biggest = pl.board[i][j];
    return biggest;
}

// return true when there is no 0.
bool fullBoard() {
    for (int i = 0; i < pl.n; i++)
        for (int j = 0; j < pl.n; j++)
            if (pl.board[i][j] == 0)
                return false;
    return true;
}

// return true when the player can't move anymore
bool cantMove() {
    // check up
    for (int j = 0;j < pl.n;j++)
        for (int i = 0;i < pl.n - 1; i++)
            if (pl.board[i][j] != 0 && pl.board[i][j] == pl.board[i + 1][j])
                return false;
    
    // check down
    for (int j = 0;j < pl.n;j++)
        for (int i = pl.n - 1; i > 0; i--)
            if (pl.board[i][j] != 0 && pl.board[i][j] == pl.board[i - 1][j])
                return false;

    // check right
    for (int i = 0;i < pl.n;i++)
        for (int j = pl.n - 1; j > 0; j--)
            if (pl.board[i][j] != 0 && pl.board[i][j] == pl.board[i][j - 1])
                return false;

    // check left
    for (int i = 0;i < pl.n;i++)
        for (int j = 0; j < pl.n - 1; j++)
            if (pl.board[i][j] != 0 && pl.board[i][j] == pl.board[i][j + 1])
                return false;

    return true;
}

// find the number of digits in a number
int dig(int number) {
    int digs = 0;
    if (number == 0)
        return 1;
    while (number > 0) {
        number /= 10;
        digs++;
    }
    return digs;
}

// return true when two int** are the same
bool isEqual(int** a, int** b) {
    for (int i = 0; i < pl.n; i++)
        for (int j = 0; j < pl.n; j++)
            if (b[i][j] != a[i][j])
                return false;
    return true;
}

// copy an int** to another one
void copyBoard(int** main, int** copy) {
    for (int i = 0; i < pl.n; i++)
        for (int j = 0; j < pl.n; j++)
            copy[i][j] = main[i][j];
}

void refresh(int l, int n, int** board); // refresh the game each turn
void mUp(int** board, int& score, int n); // move up and merge
void mDown(int** board, int& score, int n); // move Down and merge
void mRight(int** board, int& score, int n); // move right and merge
void mLeft(int** board, int& score, int n); // move left and merge

// game loop
void game(){
    t.h = 0;
    t.m = 5;
    t.s = 0;
    t.TSU = false;
    thread timerThread(timer);

    int c = 0; // one time say "Do you want to continue playing?"
    while (true){
        system("cls");
        printScore();

        if (c == 0) {
            if (findBiggest() >= 2048) {
                refresh(dig(findBiggest()), pl.n, pl.board);
                cout << endl << "\u001b[92m Winner!" << endl << endl;
                cout << "\u001b[93m >>\u001b[36m Do you want to continue playing? (y/n)\u001b[96m" << endl << " >> ";
                char choose = getch();
                if (choose == 'n'){
                    TerminateThread(timerThread.native_handle(), 1);
                    timerThread.detach();
                    return;
                }
                else {
                    system("cls");
                    printScore();
                }
            }
            c++;
        }

        if (fullBoard() && cantMove()) {
            refresh(dig(findBiggest()), pl.n, pl.board);
            cout << endl << "\u001b[91m Game Over" << endl << endl;
            cout << "\u001b[93m >>\u001b[36m press any key to continue...";
            getch();
            TerminateThread(timerThread.native_handle(), 1);
            timerThread.detach();
            return;
        }

        refresh(dig(findBiggest()), pl.n, pl.board); // print the board each turn

        if (t.TSU){
            system("cls");
            cout << "\u001b[91m Game Over";
            Sleep(1000);
            TerminateThread(timerThread.native_handle(), 1);
            timerThread.detach();
            return;
        }

        cout << endl << " " << remaningGameTime();
        cout << endl << endl << "  ";
        char move = getch();
        int** temp = new int*[pl.n];
        for (int i = 0; i < pl.n; i++) {
            temp[i] = new int[pl.n];
            for (int j = 0; j < pl.n; j++)
                temp[i][j] = 0;
        }
        copyBoard(pl.board, temp);
        switch (move)
        {
        case 'w':
            mUp(pl.board, pl.score, pl.n);
            Beep(260, 300);
            if (zeroFinder() > 0 && !isEqual(pl.board, temp))
                randNumGen();
            break;
        case 's':
            mDown(pl.board, pl.score, pl.n);
            Beep(260, 300);
            if (zeroFinder() > 0 && !isEqual(pl.board, temp))
                randNumGen();
            break;
        case 'a':
            mLeft(pl.board, pl.score, pl.n);
            Beep(260, 300);
            if (zeroFinder() > 0 && !isEqual(pl.board, temp))
                randNumGen();
            break;
        case 'd':
            mRight(pl.board, pl.score, pl.n);
            Beep(260, 300);
            if (zeroFinder() > 0 && !isEqual(pl.board, temp))
                randNumGen();
            break;
        case 'b':
            TerminateThread(timerThread.native_handle(), 1);
            timerThread.detach();
            return;
        }
    }
}

//////////////////////////////////////////
// Refresh Game
//////////////////////////////////////////

int len;
int** b;

int digNum(int number) {
	int digs = 0;
	if (number == 0)
		return 1;
	while (number > 0) {
		number /= 10;
		digs++;
	}
	return digs;
}

void printLenLine() {
	for (int j = 0; j < len + 2; j++)
		cout << "\u2501";
}

void printLastLine(int n) {
	int x = 0;
	for (int i = 0; i < n; i++) {
		if (x == 0) {
			cout << "\u2517";
			printLenLine();
		}
		else {
			cout << "\u253B";
			printLenLine();
		}
		x++;
	}
	cout << "\u251B" << endl;
}

void printFirstLine(int n) {
	int x = 0;
	for (int i = 0; i < n; i++) {
		if (x == 0) {
			cout << "\u250F";
			printLenLine();
		}
		else {
			cout << "\u2533";
			printLenLine();
		}
		x++;
	}
	cout << "\u2513" << endl;
}

void printMiddleLine(int n) {
	int x = 0;
	for (int i = 0; i < n; i++) {
		if (x == 0) {
			cout << "\u2523";
			printLenLine();
		}
		else {
			cout << "\u254B";
			printLenLine();
		}
		x++;
	}
	cout << "\u252B" << endl;
}

void printData(int i, int j) {
	int d = digNum(b[i][j]);
	double tmp = (len - d) / 2.0;
	cout << " ";
	for (int i = 0; i < floor(tmp); i++)
		cout << " ";
    cout << "\u001b[96m";
	if (b[i][j] != 0)
		cout << b[i][j];
	else
		cout << " ";
    cout << "\u001b[36m";
	for (int i = 0; i < ceil(tmp); i++)
		cout << " ";
	cout << " ";
}

void printDataLine(int n, int i) {
	for (int j = 0; j < n; j++) {
		cout << "\u2503";
		printData(i, j);
	}
	cout << "\u2503" << endl;
}

void printBlocks(int n) {
	int x = 0;
	for (int i = 0; i < n; i++) {
		if (x == 0) {
			printFirstLine(n);
			printDataLine(n, i);
		}
		else {
			printMiddleLine(n);
			printDataLine(n, i);
		}
		x++;
	}
}

// refresh the game each turn
void refresh(int l, int n, int** board) {
	len = l;
	b = board;
	printBlocks(n);
	printLastLine(n);
}

//////////////////////////////////////////
// Move
//////////////////////////////////////////

// move up and merge
void mUp(int** board, int& score, int n) {
    for (int j = 0;j < n;j++) // columns
    {
        // move up
        for (int x = 0;x < n-1;x++)
            for (int i = 0;i < n-1; i++) // rows
                if (board[i][j] == 0 && board[i + 1][j] != 0) {
                    board[i][j] = board[i + 1][j];
                    board[i + 1][j] = 0;
                }

        // merge
        for (int i = 0;i < n-1; i++) {
            if (board[i][j] != 0 && board[i][j] == board[i + 1][j]) {
                board[i][j] *= 2;
                score += board[i][j];
                for (int I = 1; i + I <= n - 1; I++) {
                    if (i + I == n - 1)
                        board[i + I][j] = 0;
                    else
                        board[i + I][j] = board[i+I + 1][j];
                }
            }
        }
    }
}

// move Down and merge
void mDown(int** board, int& score, int n) {
    for (int j = 0;j < n;j++) // columns
    {
        // move down
        for (int x = 0;x < n-1;x++)
            for (int i = n-1;i > 0;i--) // rows
                if (board[i][j] == 0 && board[i - 1][j] != 0) {
                    board[i][j] = board[i - 1][j];
                    board[i - 1][j] = 0;
                }

        // merge
        for (int i = n-1; i > 0; i--) {
            if (board[i][j] != 0 && board[i][j] == board[i - 1][j]) {
                board[i][j] *= 2;
                score += board[i][j];
                for (int I = 1; i - I >= 0; I++) {
                    if (i-I == 0)
                        board[i - I][j] = 0;
                    else
                        board[i - I][j] = board[i-I -1][j];
                }
            }
        }
    }
}

// move right and merge
void mRight(int** board, int& score, int n) {
    for (int i = 0;i < n;i++) // rows
    {
        // move right
        for (int x = 0;x < n-1;x++)
            for (int J = n-1;J > 0;J--)
                if (board[i][J] == 0 && board[i][J-1] != 0) {
                    board[i][J] = board[i][J - 1];
                    board[i][J - 1] = 0;
                }

        // merge
        for (int j = n-1; j > 0; j--) {
            if (board[i][j] != 0 && board[i][j] == board[i][j - 1]) {
                board[i][j] *= 2;
                score += board[i][j];
                for (int J = 1; j - J >= 0; J++) {
                    if (j - J == 0)
                        board[i][j - J] = 0;
                    else
                        board[i][j - J] = board[i][j-J - 1];
                }
            }
        }
    }
}

// move left and merge
void mLeft(int** board, int& score, int n) {
    for (int i = 0;i < n;i++) // rows
    {
        // move left
        for (int x = 0;x < n - 1;x++)
            for (int j = 0;j < n - 1;j++) // columns
                if (board[i][j] == 0 && board[i][j + 1] != 0) {
                    board[i][j] = board[i][j + 1];
                    board[i][j + 1] = 0;
                }

        // merge
        for (int j = 0; j < n - 1; j++) {
            if (board[i][j] != 0 && board[i][j] == board[i][j + 1]) {
                board[i][j] *= 2;
                score += board[i][j];
                for (int J = 1; j + J <= n - 1; J++) {
                    if (j + J == n - 1)
                        board[i][j + J] = 0;
                    else
                        board[i][j + J] = board[i][j + J + 1];
                }
            }
        }
    }
}

//////////////////////////////////////////
// Save and Leader Board
//////////////////////////////////////////

int maxN; // The largest game board ever played size.
void maxNFinder(Player* players, int count){
    maxN =  players[0].n;
    for (int i = 0; i < count; i++)
        if (players[i].n > maxN)
            maxN = players[i].n;
}

void save(){
    FILE* fileO;
    fileO = fopen("rec.txt", "r");
    ofstream file("rec.txt", ios::app);
    if (fileO != NULL){
        file << endl << pl.name << endl << pl.score << endl << pl.n;
    }
    else
        file << pl.name << endl << pl.score << endl << pl.n;
    file.close();
}

int recNum(){
    int count = 0; // how many records?
    ifstream file("rec.txt", ios::in);
    if (file.is_open()){
        string tmp;
        while (getline(file, tmp))
        {
            count++;
        } 
    }
    file.close();
    count /= 3;
    return count;
}

void bubbleSort_Score(Player arr[], int n)
{
    int i, j;
    bool swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = false;
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j].score < arr[j + 1].score) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (swapped == false)
            break;
    }
}

// sort rec.txt by n and score
void sortRecFile(){
    int count = recNum();
    ifstream file("rec.txt", ios::in);
    Player* players = new Player[count];
    for (int i = 0; i < count; i++){
        if (file.is_open()){
            getline(file, players[i].name);
            string score, n;
            getline(file, score);
            getline(file, n);
            players[i].score = stoi(score);
            players[i].n = stoi(n);
        }
    }
    file.close();

    ofstream fileEmp("rec.txt", ios::out);
    fileEmp << "";
    fileEmp.close();

    maxNFinder(players, count);
    int x = maxN;
    int c = 0;
    for (int i = 2; i <= maxN; i++){
        ofstream fileSort("rec.txt", ios::app);
        Player playersNSort[200];
        int playersNSort_i = 0;
        for (int j = 0; j < count; j++){
            if (players[j].n == i){
                playersNSort[playersNSort_i] = players[j];
                playersNSort_i++;
            }
        }
        bubbleSort_Score(playersNSort, playersNSort_i);
        for (int j = 0; j < playersNSort_i; j++){
            if (c != 0)
                fileSort << endl;
            fileSort << playersNSort[j].name << endl << playersNSort[j].score << endl << playersNSort[j].n;
            c++;
        }
        fileSort.close();
    }
}

// print the list of any n ever played and return the selected n
int printLeaderBoardMenu(Player* players, int count){
    system("cls");
    cout << "\u001b[36mLeader Board" << endl;
    for (int i = 0; i < 23; i++)
        cout << "\u2550";
    cout << endl << endl;
    cout << "\u001b[93m >>\u001b[36m Please select game board size" << endl;
    for (int i = 2; i <= maxN; i++){
        int c = 0;
        for (int j = 0; j < count; j++){
            if (c == 0 && players[j].n == i){
                cout << "\u001b[93m     >>\u001b[36m " << i << " x " << i << endl;
                c++;
            }
        }
    }
    cout << endl << "\u001b[93m >> ";
    string ch;
    cin >> ch;
    Beep(300, 500);
    return stoi(ch);
}

void printLeaderBoard(){
    sortRecFile();
    int count = recNum();
    ifstream file("rec.txt", ios::in);
    Player* players = new Player[count];
    for (int i = 0; i < count; i++){
        if (file.is_open()){
            getline(file, players[i].name);
            string score, n;
            getline(file, score);
            getline(file, n);
            players[i].score = stoi(score);
            players[i].n = stoi(n);
        }
    }
    file.close();
    int selectedN = printLeaderBoardMenu(players, count);
    system("cls");

    cout << "\u001b[93m Name";
    for (int i = 0; i < 46; i++)
        cout << " ";
    cout << "Score";
    for (int i = 0; i < 15; i++)
        cout << " ";
    cout << "n x n" << endl;

    for (int i = 0; i < count; i++){
        if (players[i].n == selectedN){
            cout << "\u001b[96m " << players[i].name;
            int len = players[i].name.length();
            for (int i = 0; i < 50 - len; i++)
                cout << " ";
            cout << players[i].score;
            len = to_string(players[i].score).length();
            for (int i = 0; i < 20 - len; i++)
                cout << " ";
            cout << players[i].n << " x " << players[i].n << endl;
        }
    }

    cout << endl << "Press any key to continue...";
    getch();
}
