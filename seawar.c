#include <stdio.h>
#include <conio.h>

#define M 10
#define N 10

#define EMPTY_CELL ' '

char map_comp[M][N + 1];
char map_user[M][N + 1];

void printMaps()
{
    system("cls");
    printf("  computer      user   \n");
    printf(" ABCDEFGHIJ  ABCDEFGHIJ\n");
    for (int i = 0; i < M; ++i)
        printf("%d%s %d%s\n", i, map_comp[i], i, map_user[i]);
}

int main()
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
    printMaps();
    return 0;
}
