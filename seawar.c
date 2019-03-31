#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define M 10
#define N 10

#define clearScreen() system("cls")

struct Point
{
    int i, j;
};

char **map_comp = NULL;
char **map_user = NULL;

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
void fillUserMap();
void fillCompMap();
int parseCoordinates();

#define EMPTY_CELL '\''
#define MISS_CELL '*'
#define HIT_CELL 'X'
#define SHIP_CELL 'O'

void printMaps()
{
    clearScreen();
    printf("  computer      user   \n");
    printf(" ABCDEFGHIJ  ABCDEFGHIJ\n");
    for (int i = 0; i < M; ++i)
    {
        printf("%d", i);
        for (int j = 0; j < N; ++j)
        {
            if (map_comp[i][j] == SHIP_CELL)
                printf("%c", EMPTY_CELL);
            else
                printf("%c", map_comp[i][j]);
        }
        printf(" %d%s\n", i, map_user[i]);
    }
}

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

int isCoordOnMap(int i, int j)
{
    return 0 <= i && i < M && 0 <= j && j < N;
}

int isCellOutOrEmpty(char **map, int i, int j)
{
    return !isCoordOnMap(i, j) || map[i][j] == EMPTY_CELL;
}

int isCellStroken(char **map, struct Point p)
{
    return isCoordOnMap(p.i, p.j) && (map[p.i][p.j] == MISS_CELL || map[p.i][p.j] == HIT_CELL);
}

int isCellShip(char **map, int i, int j)
{
    return isCoordOnMap(i, j) && (map[i][j] == SHIP_CELL || map[i][j] == HIT_CELL);
}

void findShip(char **map, struct Point p, struct Point *p1, struct Point *p2)
{
    if (isCellShip(map_comp, p.i, p.j - 1) || isCellShip(map_comp, p.i, p.j + 1))
    {
        *p1 = p;
        while (isCellShip(map_comp, p1->i, p1->j - 1))
            --p1->j;
            
        *p2 = p;
        while (isCellShip(map_comp, p2->i, p2->j + 1))
            ++p2->j;
    }
    else
    {
        *p1 = p;
        while (isCellShip(map_comp, p1->i - 1, p1->j))
            --p1->i;
            
        *p2 = p;
        while (isCellShip(map_comp, p2->i + 1, p2->j))
            ++p2->i;
    }
}

void swapInts(int *x, int *y)
{
    int t = *x;
    *x = *y;
    *y = t;
}

int isShipDefeated(char **map, struct Point p1, struct Point p2)
{
    if (p1.i == p2.i)
    {
        int i = p1.i;
        if (p1.j > p2.j)
            swapInts(&p1.j, &p2.j);
        for (int j = p1.j; j <= p2.j; ++j)
            if (map[i][j] == SHIP_CELL)
                return 0;
    }
    else
    {
        if (p1.i > p2.i)
            swapInts(&p1.i, &p2.i);
        int j = p1.j;
        for (int i = p1.i; i <= p2.i; ++i)
            if (map[i][j] == SHIP_CELL)
                return 0;
    }
    return 1;
}

void roundDefeatedShip(char **map, struct Point p1, struct Point p2)
{
    if (p1.i == p2.i)
    {
        int i = p1.i;
        if (p1.j > p2.j)
            swapInts(&p1.j, &p2.j);

        if (isCoordOnMap(p1.i, p1.j - 1))
            map[p1.i][p1.j - 1] = MISS_CELL;
        if (isCoordOnMap(p2.i, p2.j + 1))
            map[p2.i][p2.j + 1] = MISS_CELL;

        for (int j = p1.j - 1; j <= p2.j + 1; ++j)
            if (isCoordOnMap(i - 1, j))
                map[i - 1][j] = MISS_CELL;
        for (int j = p1.j - 1; j <= p2.j + 1; ++j)
            if (isCoordOnMap(i + 1, j))
                map[i + 1][j] = MISS_CELL;
    }
    else
    {
        if (p1.i > p2.i)
            swapInts(&p1.i, &p2.i);
        int j = p1.j;
        
        if (isCoordOnMap(p1.i - 1, p1.j))
            map[p1.i - 1][p1.j] = MISS_CELL;
        if (isCoordOnMap(p2.i + 1, p2.j))
            map[p2.i + 1][p2.j] = MISS_CELL;

        for (int i = p1.i - 1; i <= p2.i + 1; ++i)
            if (isCoordOnMap(i, j - 1))
                map[i][j - 1] = MISS_CELL;
        for (int i = p1.i - 1; i <= p2.i + 1; ++i)
            if (isCoordOnMap(i, j + 1))
                map[i][j + 1] = MISS_CELL;
    }
}

void userMove()
{
    char buf[64];
    struct Point p;
    do
    {
        printf("your strike > ");
        fgets(buf, 64, stdin);
    }
    while (parseCoordinates(buf, &p) != 0 || isCellStroken(map_comp, p));

    if (map_comp[p.i][p.j] == EMPTY_CELL)
        map_comp[p.i][p.j] = MISS_CELL;
    else if (map_comp[p.i][p.j] == SHIP_CELL)
    {
        map_comp[p.i][p.j] = HIT_CELL;

        struct Point p1, p2;
        findShip(map_comp, p, &p1, &p2);

        if (isShipDefeated(map_comp, p1, p2))
            roundDefeatedShip(map_comp, p1, p2);
    }
}

int isFinished(char **map)
{
    for(int i = 0; i < M; ++i)
        for(int j = 0; j < N; ++j)
            if (map[i][j] == SHIP_CELL)
                return 0;
    return 1;
}

void compMove()
{
    // ...
}
int isGameFinished()
{
    return isFinished(map_user) || isFinished(map_comp);
}
void printResult()
{
    printMaps();
    if (isFinished(map_comp))
        printf("You win!");
    else
        printf("You lose!");
    printf("\nPress any key to continue...\n");
    scanf("%*c");
}

void gameSession()
{
    // TODO: implement the game session

    program_state = GAME_SESSION;

    initMaps();

    clearScreen();
    fillUserMap();
    fillCompMap();

    while (!isGameFinished())
    {
        printMaps();
        userMove();
        compMove();
    }

    printResult();
    startMenu();
}

void initMaps()
{
    map_comp = (char**)malloc(M * sizeof(char*));
    map_user = (char**)malloc(M * sizeof(char*));
    for (int i = 0; i < M; ++i)
    {
        map_comp[i] = (char*)malloc((N + 1) * sizeof(char));
        map_user[i] = (char*)malloc((N + 1) * sizeof(char));
        for (int j = 0; j < N; ++j)
        {
            map_comp[i][j] = EMPTY_CELL;
            map_user[i][j] = EMPTY_CELL;
        }
        map_comp[i][N] = '\0';
        map_user[i][N] = '\0';
    }
}

int parseCoordinates(char *buf, struct Point *p)
{
    if (strlen(buf) != 3)
        return 1;
    if (buf[0] < 'A' || buf[0] > 'J')
        return 2;
    if (buf[1] < '0' || buf[1] > '9')
        return 3;
    (*p).j = buf[0] - 'A';
    (*p).i = buf[1] - '0';
    return 0;
}

int isCellAvailible(char **map, struct Point p)
{
    int i = p.i;
    int j = p.j;
    return isCellOutOrEmpty(map, i - 1, j - 1) &&
           isCellOutOrEmpty(map, i - 1, j) &&
           isCellOutOrEmpty(map, i - 1, j + 1) &&
           isCellOutOrEmpty(map, i, j - 1) &&
           isCoordOnMap(i, j) && map[i][j] == EMPTY_CELL &&
           isCellOutOrEmpty(map, i, j + 1) &&
           isCellOutOrEmpty(map, i + 1, j - 1) &&
           isCellOutOrEmpty(map, i + 1, j) &&
           isCellOutOrEmpty(map, i + 1, j + 1);
           
}

int isShipCoordAvailible(char **map, struct Point p1, struct Point p2, int size)
{
    if (p1.i != p2.i && p1.j != p2.j ||
        abs(p1.i - p2.i) != size - 1 &&
        abs(p1.j - p2.j) != size - 1)
        return 0;

    int result = 1;

    struct Point p = p1;
    if (p1.i == p2.i)
    {
        if (p1.j < p2.j)
        {
            for (; p.j <= p2.j; ++p.j)
                if (!isCellAvailible(map, p))
                    result = 0;
        }
        else
        {
            for (; p.j >= p2.j; --p.j)
                if (!isCellAvailible(map, p))
                    result = 0;
        }
    }
    else
    {
        if (p1.i < p2.i)
        {
            for (; p.i <= p2.i; ++p.i)
                if (!isCellAvailible(map, p))
                    result = 0;
        }
        else
        {
            for (; p.i >= p2.i; --p.i)
                if (!isCellAvailible(map, p))
                    result = 0;
        }
    }

    return result;
}

void putUserShip(int size)
{
    printMaps();
    printf("Size: %d Coordinates:\n", size);
    char buf[64];
    struct Point p1, p2;
    do
    {
        do
        {
            printf("first end > ");
            fgets(buf, 64, stdin);
        }
        while (parseCoordinates(buf, &p1) != 0);

        do
        {
            printf("second end > ");
            fgets(buf, 64, stdin);
        }
        while (parseCoordinates(buf, &p2) != 0);
    }
    while (!isShipCoordAvailible(map_user, p1, p2, size));

    if (p1.i == p2.i)
    {
        if (p1.j < p2.j)
            for (int j = p1.j; j <= p2.j; ++j)
                map_user[p1.i][j] = SHIP_CELL;
        else
            for (int j = p1.j; j >= p2.j; --j)
                map_user[p1.i][j] = SHIP_CELL;
    }
    else
    {
        if (p1.i < p2.i)
            for (int i = p1.i; i <= p2.i; ++i)
                map_user[i][p1.j] = SHIP_CELL;
        else
            for (int i = p1.i; i >= p2.i; --i)
                map_user[i][p1.j] = SHIP_CELL;
    }
}

void putCompShip(int size)
{
    char buf[64];
    struct Point p1, p2;
    do
    {
        p1.i = rand() % M;
        p1.j = rand() % N;
        if (rand()%2)
        {
            p2.i = p1.i;
            p2.j = p2.j + size - 1;
        }
        else
        {
            p2.i = p1.i + size - 1;
            p2.j = p1.j;
        }
    }
    while (!isShipCoordAvailible(map_comp, p1, p2, size));

    if (p1.i == p2.i)
    {
        if (p1.j < p2.j)
            for (int j = p1.j; j <= p2.j; ++j)
                map_comp[p1.i][j] = SHIP_CELL;
        else
            for (int j = p1.j; j >= p2.j; --j)
                map_comp[p1.i][j] = SHIP_CELL;
    }
    else
    {
        if (p1.i < p2.i)
            for (int i = p1.i; i <= p2.i; ++i)
                map_comp[i][p1.j] = SHIP_CELL;
        else
            for (int i = p1.i; i >= p2.i; --i)
                map_comp[i][p1.j] = SHIP_CELL;
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

void fillCompMap()
{
    for (int i = 0; i < 1; ++i) putCompShip(4);
    for (int i = 0; i < 2; ++i) putCompShip(3);
    for (int i = 0; i < 3; ++i) putCompShip(2);
    for (int i = 0; i < 4; ++i) putCompShip(1);
}

int main()
{
    startMenu();
    return 0;
}
