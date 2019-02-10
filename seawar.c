#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define M 10
#define N 10

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

#define EMPTY_CELL '\''
#define MISS_CELL '*'
#define HIT_CELL 'X'
#define SHIP_CELL 'O'

void printMapLegend()
{
    printf("\nMap legend:\n");
    printf("%c empty cell\n", EMPTY_CELL);
    printf("%c miss\n", MISS_CELL);
    printf("%c hit\n", HIT_CELL);
    printf("%c ship\n", SHIP_CELL);
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

int parseCoordinates(char *buf, int *i, int *j)
{
    if (strlen(buf) != 2)
        return 1;
    if (buf[0] < 'A' || buf[0] > 'J')
        return 2;
    if (buf[1] < '0' || buf[1] > '9')
        return 3;
    *i = buf[0] - 'A';
    *j = buf[1] - '0';
    return 0;
}

void putUserShip(int size)
{
    printf("Size: %d Coordinates:", size);
    char buf[64];
    int i1, j1;
    int i2, j2;
    do
    {
        do
        {
            printf("first end > ");
            fgets(buf, 64, stdin);
        }
        while (parseCoordinates(buf, &i1, &j1) != 0);

        do
        {
            printf("second end > ");
            fgets(buf, 64, stdin);
        }
        while (parseCoordinates(buf, &i2, &j2) != 0);
    }
    while (i1 != i2 && j1 != j2 || abs(i1 - i2) != size && abs(j1 - j2) != size);

    if (i1 == i2)
    {
        if (j1 < j2)
            for (int j = j1; j <= j2; ++j)
                map_user[i1][j] = SHIP_CELL;
        else
            for (int j = j1; j >= j2; --j)
                map_user[i1][j] = SHIP_CELL;
    }
    else
    {
        if (i1 < i2)
            for (int i = i1; i <= i2; ++i)
                map_user[i][j1] = SHIP_CELL;
        else
            for (int i = i1; i >= i2; --i)
                map_user[i][j1] = SHIP_CELL;
    }
}

void fillUserMap()
{
    printf("Put your ships on the map (print coordinates of ends for every ship)\n");
    for (int i = 0; i < 1; ++i) putUserShip(4);
    for (int i = 0; i < 2; ++i) putUserShip(3);
    for (int i = 0; i < 3; ++i) putUserShip(2);
    for (int i = 0; i < 4; ++i) putUserShip(1);
}

int main()
{
    startMenu();
    return 0;
}
