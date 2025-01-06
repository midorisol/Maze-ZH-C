#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10
#define MIN_SIZE 5

typedef struct {
    char** maze;
    int rows;
    int cols;
} Labyrinth;

void display(char** maze, int rows, int cols) {
    if (!maze) {
        printf("No maze to display!\n");
        return;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c", maze[i][j] == '1' ? '#' : ' ');
        }
        printf("\n");
    }
}

Labyrinth create_maze() {
    Labyrinth lab = {NULL, 0, 0};
    int rows, cols;

    printf("Enter maze size (5-10):\nRows: ");
    scanf("%d", &rows);
    printf("Columns: ");
    scanf("%d", &cols);

    if (rows < MIN_SIZE || rows > SIZE || cols < MIN_SIZE || cols > SIZE) {
        printf("Invalid size!\n");
        return lab;
    }

    lab.maze = malloc(rows * sizeof(char*));
    for (int i = 0; i < rows; i++) {
        lab.maze[i] = malloc(cols + 1);
        printf("Enter row %d (0s and 1s): ", i + 1);
        scanf("%s", lab.maze[i]);
    }
    display(lab.maze, rows, cols);
    lab.rows = rows;
    lab.cols = cols;
    return lab;
}

void save_maze(Labyrinth lab) {
    if (!lab.maze) return;

    char filename[50];
    printf("Enter filename: ");
    scanf("%s", filename);

    FILE* f = fopen(filename, "w");
    if (!f) return;

    fprintf(f, "%d %d\n", lab.rows, lab.cols);
    for (int i = 0; i < lab.rows; i++)
        fprintf(f, "%s\n", lab.maze[i]);

    fclose(f);
}

Labyrinth load_maze() {
    Labyrinth lab = {NULL, 0, 0};
    char filename[50];
    printf("Enter filename: ");
    scanf("%s", filename);

    FILE* f = fopen(filename, "r");
    if (!f) return lab;

    fscanf(f, "%d %d", &lab.rows, &lab.cols);
    lab.maze = malloc(lab.rows * sizeof(char*));

    for (int i = 0; i < lab.rows; i++) {
        lab.maze[i] = malloc(lab.cols + 1);
        fscanf(f, "%s", lab.maze[i]);
    }

    fclose(f);
    return lab;
}

void free_maze(Labyrinth* lab) {
    if (!lab->maze) return;

    for (int i = 0; i < lab->rows; i++)
        free(lab->maze[i]);
    free(lab->maze);
    lab->maze = NULL;
}

int main() {
    Labyrinth lab = {NULL, 0, 0};
    int choice;

    while (1) {
        printf("\n1. Display\n2. Create\n3. Save\n4. Load\n5. Exit\nChoice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                display(lab.maze, lab.rows, lab.cols);
                break;
            case 2:
                free_maze(&lab);
                lab = create_maze();
                break;
            case 3:
                save_maze(lab);
                break;
            case 4:
                free_maze(&lab);
                lab = load_maze();
                break;
            case 5:
                free_maze(&lab);
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
}
