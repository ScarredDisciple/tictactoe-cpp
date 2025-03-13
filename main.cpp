#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <chrono>
#include <limits>

using namespace std;
char spaces[3][3] = {{' ',' ',' '},{' ',' ',' '},{' ',' ',' '}};
bool isUserX = true;
bool isXMove = true;
bool isFirstRound = true;

void printBoard();
void playerMove();
void compyMove();
bool evaluate();
bool strikeCheck(bool player);
bool spotCheck();
void eenieMeanie();
bool gameOver(bool tieGame);

int main()
{
    string choice = "X";

    cout << "Let's play Tic-Tac-Toe!" << endl;
    cout << "=======================" << endl;

    while (!evaluate())
    {
        if (isFirstRound)
        {
            bool validChoice = false;
            do {
                cout << "Do you want to be X or O?" << endl;
                cin >> choice;
                validChoice = (choice == "X" || choice == "x" || choice == "O" || choice == "o");
                if (!validChoice)
                {
                    cout << "INVALID SELECTION" << endl;
                }
            } while(!validChoice);
            isUserX = choice == "X" || choice == "x";
        }

        printBoard();
        if ((isUserX && isXMove) || (!isUserX && !isXMove && !isFirstRound))
        {
            playerMove();
        }
        else
        {
            compyMove();
        }
        isFirstRound = false;
    }
}

void printBoard()
{
    system("cls");
    cout << endl;
    for (int i = 0; i < 3; i++)
    {
        cout << " " << spaces[i][0] << " | " << spaces[i][1] << " | " << spaces[i][2] << endl;
        if (i < 2)
        {
            cout << "-----------" << endl;
        }
    }
    cout << endl;
}

void playerMove()
{
    int row = 0;
    int col = 0;

    do {
        do {
            cout << "What row do you want (1-3)?" << endl;
            cin >> row;
            if (!cin || row < 1 || row > 3)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "INVALID SELECTION" << endl;
            }
        } while (row < 1 || row > 3);
        do {
            cout << "What column do you want (1-3)?" << endl;
            cin >> col;
            if (!cin || col < 1 || col > 3)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "INVALID SELECTION" << endl;
            }
        } while (col < 1 || col > 3);
        if (spaces[--row][--col] != ' ')
        {
            cout << "INVALID SELECTION" << endl;
        }
    } while (spaces[row][col] != ' ');
    spaces[row][col] = (isUserX ? 'X' : 'O');
    isXMove = !isXMove;
}

void compyMove()
{
    cout << "Let me think..." << endl;
    if (!strikeCheck(false) && !strikeCheck(true) && !spotCheck())
    {
        eenieMeanie();
    }
    isXMove = !isXMove;
}

bool strikeCheck(bool player)
{
    bool goForStop = isUserX && player;
    char check = goForStop ? 'X' : 'O';
    int vCheck = 0;
    int hCheck = 0;
    int sCheck = 0;
    int bCheck = 0;
    int hBlank = -1;
    int vBlank = -1;
    int sBlank = -1;
    int bBlank = -1;

    for (int i = 0; i < 3; i++)
    {
        vCheck = 0;
        vBlank = -1;
        hCheck = 0;
        hBlank = -1;
        for (int j = 0; j < 3; j++)
        {

            if (spaces[i][j] == check)
            {
                hCheck++;
                if (i + j == 2)
                {
                    sCheck++;
                }
                if (i == j)
                {
                    bCheck++;

                }
            }
            else if (spaces[i][j] == ' ')
            {
                hBlank = j;

                if (i + j == 2)
                {
                    sBlank = i;
                }
                if (i == j)
                {
                    bBlank = i;
                }
            }

            if (spaces[j][i] == check)
            {
                vCheck++;
            }
            else if (spaces[j][i] == ' ')
            {
                vBlank = j;

                if (i + j == 2)
                {
                    sBlank = j;
                }
                if (i == j)
                {
                    bBlank = j;
                }
            }
        }
        if (hCheck == 2 && hBlank >= 0)
        {
            spaces[i][hBlank] = isUserX ? 'O' : 'X';
            return true;
        }
        else if (vCheck == 2 && vBlank >= 0)
        {
            spaces[vBlank][i] = isUserX ? 'O' : 'X';
            return true;
        }
        else if (sCheck == 2 && sBlank >= 0)
        {
            spaces[sBlank][2 - sBlank] = isUserX ? 'O' : 'X';
            return true;
        }
        else if (bCheck == 2 && bBlank >= 0)
        {
            spaces[bBlank][bBlank] = isUserX ? 'O' : 'X';
            return true;
        }
    }
    return false;
}

bool spotCheck()
{
    char letter = isUserX ? 'O' : 'X';

    for (int i = 0; i < 9; i++)
    {
        int row1 = i/3;
        int col1 = i%3;

        if (spaces[row1][col1] == letter)
        {
            for (int j = 0; j < 9; j++)
            {
                int row2 = j/3;
                int col2 = j%3;

                if (row1 == row2 && col1 == col2)
                {
                    continue;
                }
                else if (spaces[row1][col2] == ' ')
                {
                    spaces[row1][col2] = letter;
                    return true;
                }
                else if (spaces[row2][col1] == ' ')
                {
                    spaces[row2][col1] = letter;
                    return true;
                }
                else if (row1 == row2 && spaces[row1][3-(col1+col2)] == ' ')
                {
                    spaces[row1][3-(col1+col2)] = letter;
                    return true;
                }
                else if (col1 == col2 && spaces[3-(row1+row2)][col1] == ' ')
                {
                    spaces[3-(row1+row2)][col1] = letter;
                    return true;
                }
            }
        }
    }
    return false;
}

void eenieMeanie()
{
    char letter = isUserX ? 'O' : 'X';
    bool found = false;

    do
    {
        uint64_t seed = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
        cerr << "Seed: " << seed << endl;
        srand(seed);
        int rando = rand() % 9;

        if (spaces[rando/3][rando%3] == ' ')
        {
            spaces[rando/3][rando%3] = letter;
            found = true;
        }
    } while (!found);
}

bool evaluate()
{
    char letter = isXMove ? 'O' : 'X';
    int playedSquares = 0;

    for (int i = 0; i < 9; i++)
    {
        int row1 = i/3;
        int col1 = i%3;

        playedSquares += spaces[row1][col1] != ' ';
        if (spaces[row1][col1] == letter)
        {
            int rowCount = 0;
            int colCount = 0;
            int slashCount = 0;
            int backCount = 0;

            for (int j = 0; j < 3; j++)
            {
                rowCount += (spaces[row1][j] == letter);
                colCount += (spaces[j][col1] == letter);
                slashCount += (spaces[j][2-j] == letter);
                backCount += (spaces[j][j] == letter);
            }

            if (rowCount >= 3 || colCount >= 3 || slashCount == 3 || backCount == 3)
            {
                return gameOver(false);
            }
        }
    }
    if (playedSquares == 9)
    {
        return gameOver(true);
    }
    else
    {
        return false;
    }
}

bool gameOver(bool tieGame) 
{
    string choice = "";
    bool validChoice = false;
    bool winner = (isUserX && !isXMove);
    string message = tieGame ? "CAT GAME!!" : (winner ? "YOU WIN!!!" : "YOU LOSE!!");

    isXMove = true;
    isFirstRound = true;
    for (int i = 0; i < 9; i++)
    {
        spaces[i/3][i%3] = ' ';
    }

    cout << "==========" << endl;
    cout << message << endl;
    cout << "==========" << endl;
    do {
        cout << "Want to play again (y or n)?" << endl;
        cin >> choice;
        validChoice = (choice == "Y" || choice == "y" || choice == "N" || choice == "n");
        if (!validChoice)
        {
            cout << "INVALID SELECTION" << endl;
        }
    } while (!validChoice);
    return (choice == "N" || choice == "n");
}
