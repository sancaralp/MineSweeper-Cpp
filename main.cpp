#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <windows.h>
#include <ctime>


using std::vector;
using std::cin; using std::cout;
using std::string;
using std::stringstream;

#define ADDREDCLRTOSTR(strOrigin, strAddition) strOrigin+=string("\033[31m") + (strAddition) + string("\033[37m");
#define ADDCYANCLRTOSTR(strOrigin, strAddition) strOrigin+=string("\033[36m") + (strAddition) + string("\033[37m");
#define ADDGREENCLRTOSTR(strOrigin, strAddition) strOrigin+=string("\033[32m") + (strAddition) + string("\033[37m");


class MineSweeper {
private:
    int row, col, mineCount, open, maxMineCount;

    vector<vector<int>> neighbourMineCounts;
    vector<vector<bool>> mines;
    vector<vector<bool>> flags;
public:
    vector<vector<char>> canvas;
    bool isGameOver;
    int flagCount;
private:
    void placeMines() {
        srand((unsigned) time(0));
        while (mineCount < maxMineCount) {
            int i = rand() % row;
            int j = rand() % col;
            if (!mines[i][j]) {
                mineCount++;
                mines[i][j] = true;
                for (int a = i - 1; a < i + 2; a++) {
                    for (int b = j - 1; b < j + 2; b++) {
                        if (a < 0 || a >= row || b < 0 || b >= col)
                            continue;
                        neighbourMineCounts[a][b]++;
                    }
                }

            }

        }
    }
    static void setFinalCanvas(const MineSweeper *game, string& finalText) {
        string outputCanvas;
        cout<<std::flush;
        system("CLS");
        for (int x = 0; x < game->canvas[0].size() * 2 + 3; x++) {
            outputCanvas.push_back('#');
        }

        outputCanvas.push_back('\n');
        for (int y = 0; y < game->canvas.size(); y++) {
            outputCanvas.push_back('#');
            outputCanvas.push_back(' ');
            for (int x = 0; x < game->canvas[0].size(); x++) {
                if(game->canvas[y][x]=='F')
                    ADDREDCLRTOSTR(outputCanvas, game->canvas[y][x])

                else if(game->canvas[y][x]=='*')
                    ADDGREENCLRTOSTR(outputCanvas,game->canvas[y][x])

                else if(game->canvas[y][x]!='-'&&game->canvas[y][x]!=' ')
                    ADDCYANCLRTOSTR(outputCanvas, game->canvas[y][x])

                else
                    outputCanvas.push_back(game->canvas[y][x]);
                outputCanvas.push_back(' ');
            }
            outputCanvas.push_back('#');
            outputCanvas.push_back('\n');
        }

        for (int x = 0; x < game->canvas[0].size() * 2 + 3; x++) {
            outputCanvas.push_back('#');
        }

        outputCanvas.push_back('\n');

        cout << outputCanvas << finalText;
    }

    void openZero(int currRow, int currCol) {
        for (int i = currRow - 1; i <= currRow + 1; i++) {
            for (int j = currCol - 1; j <= currCol + 1; j++) {
                if (i < 0 || j < 0 || i >= row || j >= col)
                    continue;
                if (canvas[i][j] == '-') {
                    canvas[i][j] = (neighbourMineCounts[i][j] == 0) ? ' ' : (char) (neighbourMineCounts[i][j] + 48);
                    open++;
                    if (neighbourMineCounts[i][j] == 0)
                        openZero(i, j);
                }
            }
        }
    }

    void Lose() {
        isGameOver = true;
        for(int i = 0; i < row; i++){
            for(int j = 0; j < col; j++){
                if(mines[i][j]) canvas[i][j] = '*';
            }
        }
        string outputSt;
        string lost =" __   _____  _   _   _    ___  ___ _____ _ \n"
                     " \\ \\ / / _ \\| | | | | |  / _ \\/ __|_   _| |\n"
                     "  \\ V / (_) | |_| | | |_| (_) \\__ \\ | | |_|\n"
                     "   |_| \\___/ \\___/  |____\\___/|___/ |_| (_)\n"
                     "                                           ";
        ADDREDCLRTOSTR(outputSt,lost);


        setFinalCanvas(this,outputSt);
    }

    void Win() {
        isGameOver = true;
        string won =
                " __   _____  _   _  __      _____  _  _ _ \n"
                " \\ \\ / / _ \\| | | | \\ \\    / / _ \\| \\| | |\n"
                "  \\ V / (_) | |_| |  \\ \\/\\/ / (_) | .` |_|\n"
                "   |_| \\___/ \\___/    \\_/\\_/ \\___/|_|\\_(_)\n"
                "                                          ";
        string outputSt;
        ADDGREENCLRTOSTR(outputSt, won);
        setFinalCanvas(this,outputSt);
    }


public:
    void sweep(int currRow, int currCol) {
        if (mines[currRow][currCol]) {
            canvas[currRow][currCol] = '*';
            Lose();
            return;
        }
        canvas[currRow][currCol] = (neighbourMineCounts[currRow][currCol] == 0) ? ' ' : (char) (
                neighbourMineCounts[currRow][currCol] + 48);
        open++;
        if (!neighbourMineCounts[currRow][currCol]) {
            openZero(currRow, currCol);
        }
        if (open == row * col - maxMineCount) {
            Win();
        }
    }

    void flag(int currRow, int currCol) {
        if (flags[currRow][currCol]) {
            flagCount++;
            flags[currRow][currCol] = false;
            canvas[currRow][currCol] = '-';
            return;
        }
        if (flagCount <= 0)return;
        flagCount--;
        flags[currRow][currCol] = true;
        canvas[currRow][currCol] = 'F';
    }

    MineSweeper(int rows, int columns, int mmCount) {
        isGameOver = false;
        mineCount = open = 0;
        flagCount = mmCount;
        this->row = rows, this->col = columns, this->maxMineCount = mmCount;
        for (int i = 0; i < row; i++) {
            vector<int> neighbourElement;
            vector<bool> mineElement;
            vector<bool> flagElement;
            vector<char> canvasElement;
            for (int j = 0; j < col; j++) {
                neighbourElement.push_back(0);
                mineElement.push_back(false);
                flagElement.push_back(false);
                canvasElement.push_back('-');
            }
            neighbourMineCounts.push_back(neighbourElement);
            mines.push_back(mineElement);
            flags.push_back(flagElement);
            canvas.push_back(canvasElement);
        }
        placeMines();
    }
};

void setCanvas(string &outputCanvas, const MineSweeper *game);


enum E_rotation {
    STOP, RIGHT, LEFT, UP, DOWN, CHOOSE, FLAG
};
E_rotation rotation;
MineSweeper *game;
int cursorX = 0, cursorY = 0;
string outputCanvas;

void Input() {

    if (GetAsyncKeyState(VK_LEFT)) {
        if (rotation != RIGHT)
            rotation = LEFT;
    } else if (GetAsyncKeyState(VK_RIGHT)) {
        if (rotation != LEFT)
            rotation = RIGHT;
    } else if (GetAsyncKeyState(VK_UP)) {
        if (rotation != DOWN)
            rotation = UP;
    } else if (GetAsyncKeyState(VK_DOWN)) {
        if (rotation != UP)
            rotation = DOWN;
    } else if (GetAsyncKeyState(0x46)) {//F
        rotation = FLAG;
    } else if (GetAsyncKeyState(VK_SPACE)) {
        rotation = CHOOSE;
    }

}

void Loop() {
    while (!game->isGameOver) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        Input();
        if (rotation != STOP) {
            switch (rotation) {
                case STOP:
                    break;
                case RIGHT:
                    if (cursorX < game->canvas[0].size() - 1)
                        cursorX++;
                    break;
                case LEFT:
                    if (cursorX > 0)
                        cursorX--;
                    break;
                case UP:
                    if (cursorY > 0)
                        cursorY--;
                    break;
                case DOWN:
                    if (cursorY < game->canvas.size() - 1)
                        cursorY++;
                    break;
                case CHOOSE:
                    game->sweep(cursorY, cursorX);
                    break;
                case FLAG:
                    game->flag(cursorY, cursorX);
                    break;
            }
            rotation = STOP;
            if(game->isGameOver)
                break;

            setCanvas(outputCanvas, game);
        }
    }
}
void StartLoop(){
    string outpArr[6];
    outpArr[0]="Enter difficulty:\n";
    outpArr[1]=">\033[36mEasy\n";
    outpArr[2]=" \033[32mNormal\n";
    outpArr[3]=" \033[33mHard\n";
    outpArr[4]=" \033[31mVery Hard\n";
    outpArr[5]=" \033[35mCustom\n";



    int x, y, c;
    short difficulty =0;
    bool difficultyChosen;
    bool moved;
    while (!difficultyChosen) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        Input();
        switch (rotation) {
            case UP:
                if(difficulty>0)difficulty--;
                rotation = STOP;
                moved = true;
                break;
            case DOWN:
                if(difficulty<4)difficulty++;
                rotation = STOP;
                moved = true;
                break;
            case CHOOSE:
                difficultyChosen=true;
                rotation = STOP;
                break;
            default:
                break;
        }
        if(moved) {
            for (int i = 1; i < 6; i++) {
                if (outpArr[i][0] == '>') outpArr[i][0] = ' ';
            }
            outpArr[difficulty + 1][0] = '>';
            outputCanvas.clear();
            for (auto & i : outpArr) {
                outputCanvas += i;
            }
            outputCanvas += "\n\n \033[37mUse arrow keys to move around, space to reveal the tile and F to Flag the tile";
            cout << std::flush;
            system("CLS");
            cout << outputCanvas;
            moved = false;
        }
    }
    switch (difficulty) {
        case 0:
            x = y = 9, c = 10;
            break;
        case 1:
            x = y = 16, c = 40;
            break;
        case 2:
            x = 24, y = 16, c = 70;
            break;
        case 3:
            x = 24, y = 20, c = 99;
            break;
        default:
            cout << std::flush;
            system("CLS");
            cout << "Please Enter The Number of Rows: ";
            cout << "\033[36m";
            cin >> x;
            cout << "\033[37m";

            cout << "Please Enter The Number of Columns: ";
            cout << "\033[36m";
            cin >> y;
            cout << "\033[37m";

            c = x*y/5;
            break;
    }
    game = new MineSweeper(x, y, c);
}

int main() {
    byte x;
    StartLoop();
    setCanvas(outputCanvas, game);
    Loop();
    cin>>x;
    return 0;
}

void setCanvas(string &outputCanvas, const MineSweeper *game) {
    cout<<std::flush;
    system("CLS");
    outputCanvas.clear();
    for (int x = 0; x < game->canvas[0].size() * 2 + 3; x++) {
        outputCanvas.push_back('#');
    }

    outputCanvas.push_back('\n');
    for (int y = 0; y < game->canvas.size(); y++) {
        outputCanvas.push_back('#');
        if (0 == cursorX && y == cursorY)
            outputCanvas.push_back('[');
        else
            outputCanvas.push_back(' ');
        for (int x = 0; x < game->canvas[0].size(); x++) {
            if(game->canvas[y][x]=='F')
                ADDREDCLRTOSTR(outputCanvas, game->canvas[y][x])
            else if(game->canvas[y][x]!='-'&&game->canvas[y][x]!=' ')
                ADDCYANCLRTOSTR(outputCanvas, game->canvas[y][x])
            else
                outputCanvas.push_back(game->canvas[y][x]);

            if (x == cursorX - 1 && y == cursorY)
                outputCanvas.push_back('[');
            else if (x == cursorX && y == cursorY)
                outputCanvas.push_back(']');
            else
                outputCanvas.push_back(' ');
        }
        outputCanvas.push_back('#');
        outputCanvas.push_back('\n');
    }

    for (int x = 0; x < game->canvas[0].size() * 2 + 3; x++) {
        outputCanvas.push_back('#');
    }

    outputCanvas.push_back('\n');

    cout << outputCanvas;
    cout <<"Flags left: "<< game->flagCount;
}
