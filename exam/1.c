#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define SCREEN_WIDTH 10     //寬
#define SCREEN_HEIGHT 10    //長
#define MAX_SNAKE_LENGTH 50    //蛇的長度

// 方向
typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

// 座標
typedef struct {
    int x;
    int y;
} Node;

// 身體結構
typedef struct {
    Node* body;
    int length;
    Direction direction;
} Snake;

// 食物
typedef struct {
    Node position;
    bool exists;
} Food;

void Game_Rule(void){
    printf("------貪吃蛇遊戲------\n");
    printf("1. 根據鍵盤的W、A、S、D 去做移動\n");
    printf("2. 不要讓蛇的身體碰到自己\n");
    printf("3. 以最有效的方式獲取最高分\n");
    printf("4. 那就開始吧!\n");
    printf("\n");
}

// 初始蛇
void initializeSnake(Snake* snake) {
    snake->length = 1;
    snake->direction = RIGHT;
    snake->body = (Node*)malloc(sizeof(Node) * MAX_SNAKE_LENGTH);
    snake->body[0].x = SCREEN_WIDTH / 2;
    snake->body[0].y = SCREEN_HEIGHT / 2;
}

// 釋放內存
void finalizeSnake(Snake* snake) {
    free(snake->body);
}

// 繪製遊戲介面
void drawScreen(Snake* snake, Food* food) {
    system("cls");  //清空畫面
    Game_Rule(); // 印出遊戲規則
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            bool isSnakeBody = false;
            for (int k = 0; k < snake->length; k++) {
                if (snake->body[k].x == j && snake->body[k].y == i) {
                    printf(" O ");   //蛇
                    isSnakeBody = true;
                    break;
                }
            }
            if (!isSnakeBody) {
                if (food->exists && food->position.x == j && food->position.y == i) {
                    printf(" * ");   //食物
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

// 蛇的移動
void moveSnake(Snake* snake) {
    Node newHead = snake->body[0];
    switch (snake->direction) {
        case UP:    //上
            newHead.y--;
            break;
        case DOWN:  //下
            newHead.y++;
            break;
        case LEFT:  //左
            newHead.x--;
            break;
        case RIGHT: //右
            newHead.x++;
            break;
    }

    // 將原本的頭部位置設為身體的一部分
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }
    snake->body[0] = newHead;
    
}

// 是否發生碰撞
bool checkCollision(Snake* snake) {
    for (int i = 1; i < snake->length; i++) {
        if (snake->body[0].x == snake->body[i].x && snake->body[0].y == snake->body[i].y) {
            return true;
        }
    }
    return false;
}

// 放置食物
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
    food->exists = true; // 移至這裡
}


int main(void) {
    srand(time(NULL));

    // 印出遊戲規則
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
        printf("按 Enter 來執行: ");
        input = getchar();
        while (getchar() != '\n'); // 清空輸入緩衝區

        switch (input) {
            case 'w':   // 控制鍵盤往上
                snake.direction = UP;
                break;
            case 's':   // 控制鍵盤往下
                snake.direction = DOWN;
                break;
            case 'a':   // 控制鍵盤往左
                snake.direction = LEFT;
                break;
            case 'd':   // 控制鍵盤往右
                snake.direction = RIGHT;
                break;
            case 'q':   // 直接結束遊戲
                quit = true;
                break;
        }

        moveSnake(&snake);

        // 是否與遊戲邊界或接觸到自己的身體
        if (snake.body[0].x < 0 || snake.body[0].x >= SCREEN_WIDTH || snake.body[0].y < 0 || snake.body[0].y >= SCREEN_HEIGHT || checkCollision(&snake)) {
            quit = true;
        }

        // 食物是否有被吃掉
        if (!food.exists) {
            placeFood(&food, &snake);
        }

        // 蛇頭部是否與食物重合
        if (snake.body[0].x == food.position.x && snake.body[0].y == food.position.y) {
            snake.length++;
            food.exists = false;
        }

        printf("Score: %d\n", snake.length - 1);
        fflush(stdout);

        for (int i = 0; i < 10000000; i++) {} // 控制遊戲速度
    }

    finalizeSnake(&snake);
    printf("Game Over! Your score is: %d\n", snake.length - 1);

    return 0;
}
