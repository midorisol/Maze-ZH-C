#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10
#define MIN_SIZE 5
#define MAX_NAME 50
#define FAL '#'
#define URES ' '
#define JATEKOS 'P'

typedef struct {
    char** maze;
    int rows;
    int cols;
} LabyrinthData;


void display_labyrinth(char** maze, int N, int M) {
    if (maze == NULL) {
        printf("Nincs megjelenithetö labirintus!\n");
        return;
    }

    // Keret kirajzolása (felső és alsó)
    char border[M + 3];
    sprintf(border, "+%.*s+\n", M, "--------------------");

    printf("%s", border);
    for (int i = 0; i < N; i++) {
        printf("|");
        for (int j = 0; j < M; j++)
            printf("%c", maze[i][j] == '1' ? '#' : ' ');
        printf("|\n");
    }
    printf("%s", border);
}


LabyrinthData create_labyrinth() {
    LabyrinthData data = {NULL, 0, 0};
    int N, M;
    char buffer[11];

    // Méretek beolvasása
    printf("Add meg a labirintus mereteit (5-10):\n");
    do {
        printf("Sorok szama: ");
        scanf("%d", &N);
        printf("Oszlopok szama: ");
        scanf("%d", &M);
        while (getchar() != '\n');
    } while (N < 5 || N > 10 || M < 5 || M > 10);

    // Memória foglalás
    data.maze = (char**)malloc(N * sizeof(char*));
    if (!data.maze) return data;

    for (int i = 0; i < N; i++) {
        if (!(data.maze[i] = (char*)malloc(M + 1))) {
            for (int j = 0; j < i; j++) free(data.maze[j]);
            free(data.maze);
            data.maze = NULL;
            return data;
        }
    }

    // Labirintus feltöltése
    printf("\nAdd meg a labirintust soronkent (1 - fal, 0 - ut):\n");
    for (int i = 0; i < N; i++) {
        printf("%d. sor: ", i + 1);
        if (fgets(buffer, M + 2, stdin) == NULL || strlen(buffer) - 1 != M) {
            printf("Hibas bemenet!\n");
            goto cleanup;
        }

        for (int j = 0; j < M; j++) {
            if (buffer[j] != '0' && buffer[j] != '1') {
                printf("Csak 0 es 1 karakterek hasznalhatok!\n");
                goto cleanup;
            }
        }

        strncpy(data.maze[i], buffer, M);
        data.maze[i][M] = '\0';
        display_labyrinth(data.maze, i + 1, M);
    }

    data.rows = N;
    data.cols = M;
    return data;

    cleanup:
        for (int i = 0; i < N; i++) free(data.maze[i]);
    free(data.maze);
    data.maze = NULL;
    return data;
}


void free_labyrinth(char*** maze, int N) {
    if (*maze != NULL) {
        for (int i = 0; i < N; i++) {
            if ((*maze)[i] != NULL) {
                free((*maze)[i]);
            }
        }
        free(*maze);
        *maze = NULL;  // Set the pointer to NULL after freeing
    }
}

void save_labyrinth(const LabyrinthData* labyrinth) {
    if (labyrinth->maze == NULL) {
        printf("Nincs menthetö labirintus!\n");
        return;
    }

    char filename[MAX_SIZE];
    printf("Add meg a file nevet: ");
    scanf("%s", filename);

    FILE* f = fopen(filename, "w");
    if (!f) {
        printf("Mentesi hiba!\n");
        return;
    }

    fprintf(f, "%d %d\n", labyrinth->rows, labyrinth->cols);
    for (int i = 0; i < labyrinth->rows; i++)
        fprintf(f, "%s\n", labyrinth->maze[i]);

    fclose(f);
    printf("Labirintus mentve!\n");
}


void load_labyrinth(LabyrinthData* labyrinth) {
    char filename[MAX_NAME], line[MAX_SIZE + 2];
    int rows, cols;

    printf("Add meg a betoltendo fajl nevet: ");
    scanf("%s", filename);

    FILE* f = fopen(filename, "r");
    if (!f || fscanf(f, "%d %d\n", &rows, &cols) != 2 ||
        rows < MIN_SIZE || rows > MAX_SIZE ||
        cols < MIN_SIZE || cols > MAX_SIZE) {
        printf("Fajl vagy meret hiba!\n");
        if (f) fclose(f);
        return;
        }

    // Új memória foglalás
    char** new_maze = (char**)malloc(rows * sizeof(char*));
    if (!new_maze) {
        fclose(f);
        return;
    }

    // Sorok beolvasása
    for (int i = 0; i < rows; i++) {
        if (!(new_maze[i] = (char*)malloc(cols + 1)) ||
            !fgets(line, sizeof(line), f) ||
            strlen(line) - 1 < cols) {
            for (int j = 0; j <= i; j++)
                free(new_maze[j]);
            free(new_maze);
            fclose(f);
            return;
            }

        strncpy(new_maze[i], line, cols);
        new_maze[i][cols] = '\0';
    }

    // Régi labirintus felszabadítása
    if (labyrinth->maze) {
        for (int i = 0; i < labyrinth->rows; i++)
            free(labyrinth->maze[i]);
        free(labyrinth->maze);
    }

    labyrinth->maze = new_maze;
    labyrinth->rows = rows;
    labyrinth->cols = cols;
    fclose(f);

    printf("Labirintus betoltve!\n");
    display_labyrinth(labyrinth->maze, rows, cols);
}


void menu() {
    printf("Welcome to the Game of Life\n");
    printf("Jatek szabaly 123\n");
    printf("1) Kirajzolás – az aktuális labirintus megjelenítése a sztenderd kimenten\n");
    printf("2) Készítés – a játékosnak lehetősége van új labirintusok készítésére\n");
    printf("3) Mentés – az aktuális labirintus fájlba mentése\n");
    printf("4) Betöltés – a korábban fájlba mentett labirintusok betöltése\n");
    printf("5) Játék - az aktuális labirintusban elindul a játék\n");
    printf("6, Kilépés – a program befejezése\n");
}

int main(void) {
    int choice;
    LabyrinthData data = {NULL, 0, 0};

    while (1) {
        menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                display_labyrinth(data.maze, data.rows, data.cols);
            break;
            case 2: {
                if (data.maze != NULL) {
                    free_labyrinth(&data.maze, data.rows);
                }
                data = create_labyrinth();
                break;
            }

            case 3:
                save_labyrinth(&data);
            break;
            case 4:
                load_labyrinth(&data);
            break;
            case 5:
                printf("Kaki5\n");
            break;
            case 6:
                if (data.maze != NULL) {
                    free_labyrinth(&data.maze, data.rows);
                }
            printf("Adios\n");
            return 0;
            default:
                printf("Valassz valami rendes szamot te\n");
        }
    }
    return 0;
}
