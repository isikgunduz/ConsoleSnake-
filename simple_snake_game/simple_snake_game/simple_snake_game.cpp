#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define MAX_LENGTH 150
#define X 30
#define Y 15



void gotoxy(int x, int y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//create frame
void frame() {
    for (int i = 0; i <= Y; i++) {
        gotoxy(0, i);  printf("|");
        gotoxy(X, i); printf("|");
    }

    for (int i = 0; i <= X; i++) {
        gotoxy(i, 0);  printf("-");
        gotoxy(i, Y); printf("-");
    }
}

int main() {
    int restart;
    int score = 0;
    static int max_score = 0;

    //write file
    FILE* file = fopen("score_board.txt", "a+");
    if (file == NULL) {
        gotoxy(X + 3, 3);
        printf("File could not found!");
    }
    else {
        fprintf(file, "%d\n", score);

        rewind(file);

        int temp_score;

        while (fscanf(file, "%d", &temp_score) == 1) {
            if (temp_score > max_score) {
                max_score = temp_score;
            }
        }

    }
    fclose(file);
    do {
        // reset everything
        int snake_x[MAX_LENGTH] = { 0 };
        int snake_y[MAX_LENGTH] = { 0 };
        int length_of_snake = 1;
        char direction = 'R';
        int food_x, food_y;
        int game_over = 0;
        int max_x = X - 1, min_x = 1;
        int max_y = Y - 1, min_y = 1;

        system("cls");  // clean the screen
        frame();        // draw the frame

        // first food
        do {
            food_x = rand() % (max_x - min_x + 1) + min_x;
            food_y = rand() % (max_y - min_y + 1) + min_y;
        } while (food_x == max_x / 2 && food_y == max_y / 2);
        gotoxy(food_x, food_y);
        printf("*");

        // starting point
        snake_x[0] = max_x / 2;
        snake_y[0] = max_y / 2;
        do {

            //set directions
            if (_kbhit()) {
                char key = _getch();
                if (key == 'w' && direction != 'D')
                    direction = 'U';
                else if (key == 's' && direction != 'U')
                    direction = 'D';
                else if (key == 'd' && direction != 'L')
                    direction = 'R';
                else if (key == 'a' && direction != 'R')
                    direction = 'L';
            }

            //remove tail
            int tail_x = snake_x[length_of_snake - 1];
            int tail_y = snake_y[length_of_snake - 1];
            gotoxy(tail_x, tail_y);
            printf(" ");

            //move body
            for (int i = length_of_snake - 1; i > 0;i--) {
                snake_x[i] = snake_x[i - 1];
                snake_y[i] = snake_y[i - 1];
            }

            //update head
            if (direction == 'U')
                snake_y[0]--;
            else if (direction == 'D')
                snake_y[0]++;
            else if (direction == 'L')
                snake_x[0]--;
            else if (direction == 'R')
                snake_x[0]++;

            //move head
            gotoxy(snake_x[0], snake_y[0]);
            printf("0");

            //move tail
            for (int i = 1; i < length_of_snake;i++) {
                gotoxy(snake_x[i], snake_y[i]);
                printf("o");
            }


            //game over
            if (snake_x[0] == 0 || snake_x[0] == X || snake_y[0] == 0 || snake_y[0] == Y) {
                game_over = 1;
            }
            for (int i = 1; i < length_of_snake;i++) {
                if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i]) {
                    game_over = 1;
                }
            }

            //eating the food
            if (snake_x[0] == food_x && snake_y[0] == food_y) {
                length_of_snake++;
                score++;

                //create food
                int valid;
                do {
                    valid = 1;
                    food_x = rand() % (max_x - min_x + 1) + min_x;
                    food_y = rand() % (max_y - min_y + 1) + min_y;
                    for (int i = 0; i < length_of_snake; i++) {
                        if (food_x == snake_x[i] && food_y == snake_y[i]) {
                            valid = 0;
                            break;
                        }
                    }
                } while (valid != 1);
                gotoxy(food_x, food_y);
                printf("*");
            }
            //score board
            gotoxy(X + 3, 2);
            printf("Score: %d", score);
            gotoxy(X + 3, 3);
            printf("Highest score: %d", max_score);

            //Restart screen
            if (game_over == 1) {
                for (int i = 0; i < length_of_snake;i++) {
                    gotoxy(snake_x[i], snake_y[i]);
                    printf(" ");
                }
                gotoxy(food_x, food_y);
                printf(" ");
                gotoxy(8, 6);
                printf("GAME OVER!");
                gotoxy(8, 7);
                printf("Score: %d", score);
                gotoxy(8, 8);
                printf("'R' for restart!");
            }

            Sleep(100);
        } while (game_over == 0);

        //read file
        char re;
        do {
            re = _getch();
        } while (re == 'w' && re == 'a' && re == 's' && re == 'd');
        if (re == 'r') {
            restart = 0;
        }
        else {
            restart = 1;
        }
        file = fopen("score_board.txt", "a");
        if (file != NULL) {
            fprintf(file, "%d\n", score);
            fclose(file);
        }
        score = 0;
    } while (restart == 0);


    return 0;
}
