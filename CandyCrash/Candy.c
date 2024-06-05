/*遊戲設計說明：
  1. 遊戲範圍大小為 5X5
  2. 共有三種顏色 (0, 1, 2)
  3. 將三個一樣配對在一起則會消除

  程式設計說明：
  1. 首先先創建一個 5X5 的範圍
  2. 再初始化遊戲棋盤
     以隨機函數srand(time(NULL));
     將棋盤的範圍內填滿三種顏色 (0, 1, 2)
  3. 根據使用者輸入的座標來進行移動ss
  4. 過程中將三個重複的消除，並將上方為消除
     的降下來以填滿底下空格
  5. 當棋盤內沒有相同的顏色時，則會跳出遊戲
     並代表遊戲結束
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_WIDTH 5  // y最大為5
#define BOARD_HEIGHT 5 // x最大為5
#define COLORS 3       // 三種顏色 0, 1, 2
#define MIN_MATCH 3    // 最少三個連線

// 定義顏色
typedef struct
{
    int color;
} Block;

// 定義遊戲棋盤結構
typedef struct
{
    int width;
    int height;
    Block **board;
} GameBoard;

// 創建遊戲棋盤
GameBoard *createGameBoard(int width, int height)
{
    GameBoard *board = (GameBoard *)malloc(sizeof(GameBoard));
    board->width = width;
    board->height = height;
    board->board = (Block **)malloc(sizeof(Block *) * width);
    for (int i = 0; i < width; i++)
    {
        board->board[i] = (Block *)malloc(sizeof(Block) * height);
    }
    return board;
}

// 釋放遊戲棋盤佔用的內存
void destroyGameBoard(GameBoard *board)
{
    for (int i = 0; i < board->width; i++)
    {
        free(board->board[i]);
    }
    free(board->board);
    free(board);
}

// 初始化遊戲棋盤
void initGameBoard(GameBoard *board)
{
    srand(time(NULL));
    for (int y = 0; y < board->height; y++)
    {
        for (int x = 0; x < board->width; x++)
        {
            board->board[x][y].color = rand() % COLORS;
        }
    }
}

// 印出遊戲棋盤
void printGameBoard(GameBoard *board)
{
    for (int y = 0; y < board->height; y++)
    {
        for (int x = 0; x < board->width; x++)
        {
            if (board->board[x][y].color == -1)
            {
                printf("  "); // 已消除的方塊
            }
            else
            {
                printf("%d ", board->board[x][y].color);
            }
        }
        printf("\n");
    }
}

// 交換輸入的兩個方塊的位置
void swapBlocks(Block *a, Block *b)
{
    Block temp = *a;
    *a = *b;
    *b = temp;
}

// 檢查遊戲棋盤中是否有可以消除的方塊
int checkMatches(GameBoard *board)
{
    int matches = 0;
    // 檢查每一行是否有符合消除條件的方塊連線
    for (int y = 0; y < board->height; y++)
    {
        int count = 1;
        for (int x = 1; x < board->width; x++)
        {
            if (board->board[x][y].color == board->board[x - 1][y].color)
            {
                count++;
                if (count >= MIN_MATCH)
                {
                    // 標記要消除的方塊
                    for (int i = x - count + 1; i <= x; i++)
                    {
                        board->board[i][y].color = -1;
                    }
                    matches++;
                }
            }
            else
            {
                count = 1;
            }
        }
    }
    // 檢查每一列是否有符合消除條件的方塊連線
    for (int x = 0; x < board->width; x++)
    {
        int count = 1;
        for (int y = 1; y < board->height; y++)
        {
            if (board->board[x][y].color == board->board[x][y - 1].color)
            {
                count++;
                if (count >= MIN_MATCH)
                {
                    // 標記要消除的方塊
                    for (int i = y - count + 1; i <= y; i++)
                    {
                        board->board[x][i].color = -1;
                    }
                    matches++;
                }
            }
            else
            {
                count = 1;
            }
        }
    }

    // 掉落方塊並填滿空格
    for (int x = 0; x < board->width; x++)
    {
        int fillIndex = board->height - 1;
        for (int y = board->height - 1; y >= 0; y--)
        {
            if (board->board[x][y].color != -1)
            {
                board->board[x][fillIndex--].color = board->board[x][y].color;
            }
        }
        for (; fillIndex >= 0; fillIndex--)
        {
            board->board[x][fillIndex].color = -1;
        }
    }

    return matches;
}

// 檢查遊戲是否結束（即棋盤中是否有相鄰的顏色可以消除）
int checkGameEnd(GameBoard *board)
{
    for (int x = 0; x < board->width; x++)
    {
        for (int y = 0; y < board->height; y++)
        {
            if ((x > 0 && board->board[x][y].color == board->board[x - 1][y].color) ||
                (x < board->width - 1 && board->board[x][y].color == board->board[x + 1][y].color) ||
                (y > 0 && board->board[x][y].color == board->board[x][y - 1].color) ||
                (y < board->height - 1 && board->board[x][y].color == board->board[x][y + 1].color))
            {
                return 0; // 遊戲還未結束
            }
        }
    }
    return 1; // 遊戲結束
}

int main()
{
    GameBoard *board = createGameBoard(BOARD_WIDTH, BOARD_HEIGHT);
    initGameBoard(board);
    printf("歡迎來到消消樂遊戲!\n");
    printf("嘗試將 3 個或更多相同顏色的區塊連在一起!\n");
    printf("輸入 (x, y) 座標來移動, 或按q結束遊戲\n");
    printf("初始棋盤:\n");
    printGameBoard(board);

    int moves = 0;

    // 進行移動方塊
    while (1)
    {
        int x1, y1, x2, y2;
        printf("第%d次移動, 輸入座標 (x1 y1 x2 y2): ", moves + 1);
        char input[10];
        fgets(input, sizeof(input), stdin); // 讀取玩家輸入的座標
        if (input[0] == 'q')
        {
            printf("遊戲結束!");
            break;
        }
        sscanf(input, "%d %d %d %d", &x1, &y1, &x2, &y2); // 輸入的座標設為整數

        // 座標檢查
        if (abs(x1 - x2) + abs(y1 - y2) != 1)
        {
            printf("Invalid move. Please enter again.\n");
            continue;
        }

        // 交換座標上的方塊
        swapBlocks(&board->board[x1][y1], &board->board[x2][y2]);

        moves++;
        printf("移動 %d:\n", moves);
        printGameBoard(board); // 顯示移動後的座標狀態

        int matches = checkMatches(board); // 檢查是否有符合消除條件的方塊連線
        if (matches > 0)
        {
            printf("消除個數: %d\n", matches / 2); // 如果有符合消除條件的方塊連線，則顯示匹配數量
            printGameBoard(board);                 // 顯示更新後的遊戲棋盤狀態
            matches = checkMatches(board);         // 再次檢查是否還有符合消除條件的方塊連線
        }

        // 檢查遊戲是否結束
        if (checkGameEnd(board))
        {
            printf("無法再消除方塊，遊戲結束。\n");
            break;
        }
    }

    destroyGameBoard(board);
    return 0;
}
