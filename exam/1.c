#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define SCREEN_WIDTH 10     //�e
#define SCREEN_HEIGHT 10    //��
#define MAX_SNAKE_LENGTH 50    //�D������

// ��V
typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

// �y��
typedef struct {
    int x;
    int y;
} Node;

// ���鵲�c
typedef struct {
    Node* body;
    int length;
    Direction direction;
} Snake;

// ����
typedef struct {
    Node position;
    bool exists;
} Food;

void Game_Rule(void){
    printf("------�g�Y�D�C��------\n");
    printf("1. �ھ���L��W�BA�BS�BD �h������\n");
    printf("2. ���n���D������I��ۤv\n");
    printf("3. �H�̦��Ī��覡����̰���\n");
    printf("4. ���N�}�l�a!\n");
    printf("\n");
}

// ��l�D
void initializeSnake(Snake* snake) {
    snake->length = 1;
    snake->direction = RIGHT;
    snake->body = (Node*)malloc(sizeof(Node) * MAX_SNAKE_LENGTH);
    snake->body[0].x = SCREEN_WIDTH / 2;
    snake->body[0].y = SCREEN_HEIGHT / 2;
}

// ���񤺦s
void finalizeSnake(Snake* snake) {
    free(snake->body);
}

// ø�s�C������
void drawScreen(Snake* snake, Food* food) {
    system("cls");  //�M�ŵe��
    Game_Rule(); // �L�X�C���W�h
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            bool isSnakeBody = false;
            for (int k = 0; k < snake->length; k++) {
                if (snake->body[k].x == j && snake->body[k].y == i) {
                    printf(" O ");   //�D
                    isSnakeBody = true;
                    break;
                }
            }
            if (!isSnakeBody) {
                if (food->exists && food->position.x == j && food->position.y == i) {
                    printf(" * ");   //����
                }
                else {
                    printf(" - ");
                }

            }
        }
        printf("\n");
    }
    printf("\n");
}

// �D������
void moveSnake(Snake* snake) {
    Node newHead = snake->body[0];
    switch (snake->direction) {
        case UP:    //�W
            newHead.y--;
            break;
        case DOWN:  //�U
            newHead.y++;
            break;
        case LEFT:  //��
            newHead.x--;
            break;
        case RIGHT: //�k
            newHead.x++;
            break;
    }

    // �N�쥻���Y����m�]�����骺�@����
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }
    snake->body[0] = newHead;
    
}

// �O�_�o�͸I��
bool checkCollision(Snake* snake) {
    for (int i = 1; i < snake->length; i++) {
        if (snake->body[0].x == snake->body[i].x && snake->body[0].y == snake->body[i].y) {
            return true;
        }
    }
    return false;
}

// ��m����
void placeFood(Food* food, Snake* snake) {
    bool validPosition = false;
    while (!validPosition) {
        food->position.x = rand() % SCREEN_WIDTH;
        food->position.y = rand() % SCREEN_HEIGHT;
        validPosition = true;
        for (int i = 0; i < snake->length; i++) {
            if (food->position.x == snake->body[i].x && food->position.y == snake->body[i].y) {
                validPosition = false;
                break;
            }
        }
    }
    food->exists = true; // ���ܳo��
}


int main(void) {
    srand(time(NULL));

    // �L�X�C���W�h
    Game_Rule();

    Snake snake;
    Food food;

    initializeSnake(&snake);
    placeFood(&food, &snake);
    food.exists = false;

    char input;
    bool quit = false;
    while (!quit) {
        drawScreen(&snake, &food);
        printf("�� Enter �Ӱ���: ");
        input = getchar();
        while (getchar() != '\n'); // �M�ſ�J�w�İ�

        switch (input) {
            case 'w':   // ������L���W
                snake.direction = UP;
                break;
            case 's':   // ������L���U
                snake.direction = DOWN;
                break;
            case 'a':   // ������L����
                snake.direction = LEFT;
                break;
            case 'd':   // ������L���k
                snake.direction = RIGHT;
                break;
            case 'q':   // ���������C��
                quit = true;
                break;
        }

        moveSnake(&snake);

        // �O�_�P�C����ɩα�Ĳ��ۤv������
        if (snake.body[0].x < 0 || snake.body[0].x >= SCREEN_WIDTH || snake.body[0].y < 0 || snake.body[0].y >= SCREEN_HEIGHT || checkCollision(&snake)) {
            quit = true;
        }

        // �����O�_���Q�Y��
        if (!food.exists) {
            placeFood(&food, &snake);
        }

        // �D�Y���O�_�P�������X
        if (snake.body[0].x == food.position.x && snake.body[0].y == food.position.y) {
            snake.length++;
            food.exists = false;
        }

        printf("Score: %d\n", snake.length - 1);
        fflush(stdout);

        for (int i = 0; i < 10000000; i++) {} // ����C���t��
    }

    finalizeSnake(&snake);
    printf("Game Over! Your score is: %d\n", snake.length - 1);

    return 0;
}
