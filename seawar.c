#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define M 10
#define N 10

#define EMPTY_CELL ' '

#define clearScreen() system("cls")

char map_comp[M][N + 1];
char map_user[M][N + 1];

#define DEFAULT_STATE 0
#define START_MENU 1
#define GAME_SESSION 2

int program_state = DEFAULT_STATE;

void printMaps();
void printMapLegend();
void printCommandList();
void startMenu();
void gameSession();
void initMaps();

void printMaps()
{
    clearScreen();
    printf("  computer      user   \n");
    printf(" ABCDEFGHIJ  ABCDEFGHIJ\n");
    for (int i = 0; i < M; ++i)
        printf("%d%s %d%s\n", i, map_comp[i], i, map_user[i]);
}

void printMapLegend()
{
    printf("\nMap legend:\n");
    // TODO: describe map legend
}

void printCommandList()
{
    printf("\nCommand list:\n");
    switch (program_state)
    {
    case START_MENU:
        printf("game - start the game\n");
        printf("quit - quit from the program\n");
    }
}

void startMenu()
{
    program_state = START_MENU;

    clearScreen();
    printf("START MENU\n");
    printMapLegend();
    printCommandList();

    printf("\n");
    char buf[32];
    while (1)
    {
        printf("> ");
        fgets(buf, 32, stdin);
        if (strcmp(buf, "game\n") == 0)
        {
            gameSession();
            break;
        }
        else if (strcmp(buf, "quit\n") == 0)
        {
            printf("You have quit from the program!\n");
            exit(0);
        }
    }
}

void gameSession()
{
    program_state = GAME_SESSION;

    clearScreen();
    printf("You have launched the game!");
    scanf("%*c");
    startMenu();
    // TODO: implement the game session
}

void initMaps()
{
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            map_comp[i][j] = EMPTY_CELL;
            map_user[i][j] = EMPTY_CELL;
        }
        map_comp[i][N] = '\0';
        map_user[i][N] = '\0';
    }
}

int main()
{
    startMenu();
    return 0;
}
