#include <iostream>

#include "carFunctions.h"

int main()
{

    GameStatements gStat;
    GameVars gVars;
    Enemy enemy;

    hideCursor();
    gameInitialisation(gVars, gStat, enemy);
    spiralEffect(gVars);
    transition();
    title();

    while (!gStat.m_dead)
    {
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