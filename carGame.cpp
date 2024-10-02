#include<iostream>
#include<windows.h>
#include<stdlib.h>
#include<ctime>
#include<conio.h>



struct GameStatements
{
  bool m_exit;
  bool m_gOver;
  bool m_isMoving;
  bool m_crash;
  bool m_dead;
};

struct GameVars
{
  int m_height;
  int m_width;
  int m_life{5};
  int m_speed;
  int m_score;
  int m_carX;
  int m_carY;
  
};

struct Enemy
{
  int m_enemyX[4];
  int m_enemyY[4];
  int m_ePosX;
  int m_ePosY;
  int m_eCount;
};

std::string gExplode1[4]{"o   o", " ooo ", " ooo ", "o   o"};
std::string gExplode2[4]{" ooo ", "o   o", "o   o", " ooo "};

std::string gPlayerCar[4]{"===",
                          "|-|",
                          " + ",
                          "|-|"};

//устанавливаю курсор в нужную позицию
void moveCursor(int x, int y) { 
    COORD cordinate;
    cordinate.X = x;
    cordinate.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cordinate);
}

//прячу курсор
void hideCursor() { 
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 100;
    cursor.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

//подготовка игры(Инициализирую переменные для игры)
void gameInitialisation(GameVars &gVars, GameStatements &gStat, Enemy &enemy) {
#ifndef FIRSTCALL
#define FIRSTCALL
    srand(time(NULL));
    rand();
#endif

    gStat.m_exit = false;
    gStat.m_isMoving = false;
    gStat.m_gOver = false;
    gStat.m_dead = false;

    gVars.m_height = 30;
    gVars.m_width = 19;
    gVars.m_carX = 8;
    gVars.m_carY = 16;
    gVars.m_speed = 1;
    gVars.m_score = 0;
    
    enemy.m_eCount = 4;

    for(int i{0}; i < enemy.m_eCount; ++i) {
        enemy.m_ePosX = rand() % 3;
        if(enemy.m_ePosX == 0)
            enemy.m_enemyX[i] = 2;

        else if(enemy.m_ePosX == 1)
            enemy.m_enemyX[i] = 8;

        else if(enemy.m_ePosX == 2)
            enemy.m_enemyX[i] = 14;
    }

    enemy.m_enemyY[0] = -8;
    enemy.m_enemyY[1] = -18;
    enemy.m_enemyY[2] = -28;
    enemy.m_enemyY[0] = -38;

}

//рисую игровое поле
void drawGameField(const GameVars& gVars, const GameStatements& gStat) {
    for(int i{0}; i < gVars.m_height; ++i) {
        moveCursor(0, i);
        std::cout << "|                  |";

        if(i % 2 != 0 && gStat.m_isMoving) {
            moveCursor(6, i);
            std::cout << "|      |";

        } else if(i % 2 != 0 && !gStat.m_isMoving) {
            moveCursor(6, i);
            std::cout << "|      |";
        }
    }

    moveCursor(25, 19);
    std::cout << "A - left: D - right";
    moveCursor(25, 20);
    std::cout << "W - up: S - down";

    moveCursor(25, 21);
    std::cout << "Life: " << gVars.m_life;

    moveCursor(25, 22);
    std::cout << "Score: " << gVars.m_score;

}

//машинка игрока
void drawPlayerCar(const GameVars &gVar, const GameStatements &gStat) {
    if(!gStat.m_dead) {
        for(int i{0}; i < 4; ++i) {
            moveCursor(gVar.m_carX, gVar.m_carY + i);
            std::cout << gPlayerCar[i];
        }
    } else {
        for(int i{0}; i < 4; ++i) {
            moveCursor(gVar.m_carX, gVar.m_carY + i);
            std::cout << "   ";
        }
    }
}

//отрисовка врагов
void drawEnemyCar(const Enemy &eCars) {
    for(int i{0}; i < eCars.m_eCount; ++i) {
        if(eCars.m_enemyY[i] + 3 > 0) {
            moveCursor(eCars.m_enemyX[i], eCars.m_enemyY[i] + 3);
            std::cout << "+++";
        }
        if(eCars.m_enemyY[i] + 2 > 0) {
            moveCursor(eCars.m_enemyX[i], eCars.m_enemyY[i] + 2);
            std::cout << "[-]";
        }
        if(eCars.m_enemyY[i] + 1 > 0) {
            moveCursor(eCars.m_enemyX[i], eCars.m_enemyY[i] + 1);
            std::cout << " + ";
        }
        if(eCars.m_enemyY[i] > 0) {
            moveCursor(eCars.m_enemyX[i], eCars.m_enemyY[i]);
            std::cout << "[-]";
        }

        if(eCars.m_enemyY[i] + 3 >= 40) {
            moveCursor(eCars.m_enemyX[i], eCars.m_enemyY[i] + 3);
            std::cout << "   ";
        }
        if(eCars.m_enemyY[i] + 2 >= 40) {
            moveCursor(eCars.m_enemyX[i], eCars.m_enemyY[i] + 2);
            std::cout << "   ";
        }
        if(eCars.m_enemyY[i] + 1 >= 40) {
            moveCursor(eCars.m_enemyX[i], eCars.m_enemyY[i] + 1);
            std::cout << "   ";
        }
        if(eCars.m_enemyY[i] >= 40) {
            moveCursor(eCars.m_enemyX[i], eCars.m_enemyY[i]);
            std::cout << "   ";
        }
    }
}

//нажатие кнопок
void keyboardPush(GameVars &gVar, const GameStatements &gStat) {
    if(_kbhit()) {
        switch(_getch()) {
            case 119:
                if(gVar.m_carY > 0 && !gStat.m_dead)
                    gVar.m_carY -= gVar.m_speed;
                break;
            case 97:
                if(gVar.m_carX > 2 && !gStat.m_dead)
                    gVar.m_carX -= 6;
                break;
            case 100:
                if(gVar.m_carX < 14 && !gStat.m_dead)
                    gVar.m_carX += 6;
                break;
            case 115:
                if(gVar.m_carY < 26 && !gStat.m_dead)
                    gVar.m_carY += gVar.m_speed;
                break;
        }
    }
}

//игровая механика
void runGame(GameVars &gVars, GameStatements &gStat, Enemy &enemy) {
    if(gStat.m_isMoving)
        gStat.m_isMoving = false;
    else
        gStat.m_isMoving = true;

    //двигаю врага
    for(int i{0}; i < enemy.m_eCount; ++i)
        ++enemy.m_enemyY[i];

    for(int i{0}; i < enemy.m_eCount; ++i) {
        
        if(enemy.m_enemyY[i] > 21) {
            enemy.m_ePosX = rand() % 3;
            
            if(enemy.m_ePosX == 0)
                enemy.m_enemyX[i] = 2;
            
            else if(enemy.m_ePosX == 1)
                enemy.m_enemyX[i] = 8;
            
            else if(enemy.m_ePosX == 2)
                enemy.m_enemyX[i] = 14;

            enemy.m_enemyY[i] = -8;
            ++gVars.m_score;
        }
    }

    //проверка на столкновение
    for(int i{0}; i < enemy.m_eCount; ++i) {
        if((gVars.m_carY <= enemy.m_enemyY[i] + 3
           && gVars.m_carY >= enemy.m_enemyY[i]
           || enemy.m_enemyY[i] >= gVars.m_carY
           && enemy.m_enemyY[i] <= gVars.m_carY + 3)
           && gVars.m_carX == enemy.m_enemyX[i]) {
            
            gStat.m_dead = true;
        }
    }
}

void carCrush(GameVars &gVars, GameStatements &gStat) {
    if(gStat.m_crash) {
        for(int i{0}; i < 4; ++i) {
            moveCursor(gVars.m_carX - 1, gVars.m_carY - 1 + i);
            std::cout << gExplode1[i] << '\n';
        }
        gStat.m_crash = false;
    } else {
        for(int i{0}; i < 4; ++i) {
            moveCursor(gVars.m_carX - 1, gVars.m_carY - 1 + i);
            std::cout << gExplode2[i] << '\n';
        }
        gStat.m_crash = true;
    }
    Sleep(200);
}

//когда попал в аварию
void died(GameVars &gVars, GameStatements &gStat, Enemy &enemy) {
    
    if(gStat.m_dead) {
        
        --gVars.m_life;
        int count{0};
        while(count != 10) {
            keyboardPush(gVars, gStat);
            carCrush(gVars, gStat);
            moveCursor(2, 22);
            std::cout << "Your score " << gVars.m_score << " points";
            ++count;
        }
        moveCursor(2, 22);
        std::cout << "                      ";
        gameInitialisation(gVars, gStat, enemy);
    }
}

void transition() {
    for(int i{29}; i >= 0; --i) {
        moveCursor(2, i);
        std::cout << "#################";
        Sleep(15);
    }
    for(int i{1}; i < 30; ++i) {
        moveCursor(1, i);
        std::cout << "                  ";
        Sleep(15);
    }
}

void gameOver(GameVars &gVars, GameStatements &gStat) {
    if(gVars.m_life == 0) {
        gStat.m_gOver = true;
        do {
            moveCursor(0, 0); std::cout << "*******************";
            moveCursor(0, 1); std::cout << "*                 *";
            moveCursor(0, 2); std::cout << "*    Game Over!   *";
            moveCursor(0, 3); std::cout << "*                 *";
            moveCursor(0, 4); std::cout << "*      Score      *";
            moveCursor(0, 5); std::cout << "*                 *";
            moveCursor(0, 6); std::cout << "*       "<<gVars.m_score<<"       *";
            moveCursor(0, 7); std::cout << "*                 *";
            moveCursor(0, 8); std::cout << "* Press'x'to exit *";
            moveCursor(0, 9); std::cout << "*******************";
        } while(_getch() != 'x');
        exit(1);
    }
}

void spiralEffect(const GameVars &gVars) {
    int row{1};
    int col{1};
    int lastRow = gVars.m_height - 2;
    int lastCol = gVars.m_width - 2;

    while(row <= lastRow && col <= lastCol) {
        
        for(int i{col}; i <= lastCol; ++i) {
            moveCursor(i, row);
            std::cout << '#';
            Sleep(1);
        }
        
        ++row;
        
        for(int i{row}; i <= lastRow; ++i) {
            moveCursor(lastCol, i);
            std::cout << '#';
            Sleep(1);
        }

        --lastCol;

        if(row <= lastRow) {
            for(int i{lastCol}; i >= col; --i) {
                moveCursor(i, lastRow);
                std::cout << '#';
                Sleep(1);
            }

            --lastRow;
        }

        if(col <= lastCol) {
            for(int i{lastRow}; i >= row; --i) {
                moveCursor(col, i);
                std::cout << '#';
                Sleep(1);
            }
            ++col;
        }

    }

    row = 1;
    col = 1;
    lastRow = gVars.m_height - 2;
    lastCol = gVars.m_width - 2;

    while(row <= lastRow && col <= lastCol) {

        for(int i{col}; i <= lastCol; ++i) {
            moveCursor(i, row);
            std::cout << ' ';
            Sleep(2);
        }

        ++row;

        for(int i{row}; i <= lastRow; ++i) {
            moveCursor(lastCol, i);
            std::cout << ' ';
            Sleep(1);
        }

        --lastCol;

        if(row <= lastRow) {
            for(int i{lastCol}; i >= col; --i) {
                moveCursor(i, lastRow);
                std::cout << ' ';
                Sleep(1);
            }

            --lastRow;
        }

        if(col <= lastCol) {
            for(int i{lastRow}; i >= row; --i) {
                moveCursor(col, i);
                std::cout << ' ';
                Sleep(1);
            }
            ++col;
        }

    }
}

void title() {
    do {
        
        moveCursor(0, 1); std::cout << "*     RACING       *";
        moveCursor(0, 9); std::cout << "Press'Space'to start";
    } while(_getch() != 32);
}

int main() {
  
    
    GameStatements gStat;
    GameVars gVars;
    Enemy enemy;

    hideCursor();
    gameInitialisation(gVars, gStat, enemy);
    spiralEffect(gVars);
    transition();
    title();
    while(!gStat.m_dead) {
        drawGameField(gVars, gStat);
        keyboardPush(gVars, gStat);
        runGame(gVars, gStat, enemy);
        drawPlayerCar(gVars, gStat);
        drawEnemyCar(enemy);
        died(gVars, gStat, enemy);
        gameOver(gVars, gStat);
        Sleep(50);
    }
    system("cls");

    return 0;
}