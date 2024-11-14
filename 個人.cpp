#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define FIELD_WIDTH  (14)
#define FIELD_HEIGHT (FIELD_WIDTH * 2)
#define FPS   (10)
#define INTERVAL (1000 / FPS)
#define PADDLE_WIDTH (4)

// ボールとパドルの座標と速度
int ballX, ballY, ballVelocityX, ballVelocityY;
int paddleX, paddleY;

// ゲームフィールド
int field[FIELD_HEIGHT][FIELD_WIDTH];

// 画面を描画する関数
void DrawScreen()
{
    // 画面をクリア
    system("cls");

    // フィールドの上下の枠を描画
    for (int x = 0; x < FIELD_WIDTH + 2; x++) 
    {
        printf("■");
    }
    printf("\n");

    // フィールドの中央部分を描画
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        printf("■");
        for (int x = 0; x < FIELD_WIDTH; x++) {
            if ((x == ballX) && (y == ballY)) {
                printf("●"); // ボール
            }
            else if ((y == paddleY) && (x >= paddleX) && (x < paddleX + PADDLE_WIDTH)) {
                printf("回"); // 移動する台
            }
            else if (field[y][x]) {
                printf("□"); // ブロック
            }
            else {
                printf("　"); // 何もないスペース
            }
        }
        printf("■");
        printf("\n");
    }

    // フィールドの下の枠を描画
    for (int x = 0; x < FIELD_WIDTH + 2; x++)
    {
        printf("■");
    }
}

// ゲームをリセットする関数
void Reset()
{
    // ボールの初期位置と速度をランダムに設定
    ballX = rand() % FIELD_WIDTH;
    ballY = FIELD_HEIGHT / 3;
    ballVelocityX = (rand() % 2) ? 1 : -1;
    ballVelocityY = 1;

    // パドルの初期位置を設定
    paddleX = (FIELD_WIDTH - PADDLE_WIDTH) / 2;
    paddleY = FIELD_HEIGHT - 3;

    // フィールドの上部にブロックを設置
    for (int y = 0; y < FIELD_HEIGHT / 4; y++) 
    {
        for (int x = 0; x < FIELD_WIDTH; x++) 
        {
            field[y][x] = 1;
        }
    }

    DrawScreen();
}

int main() 
{
    srand((unsigned int)time(NULL));
    Reset();

    clock_t lastClock = clock();
    while (1) 
    {
        clock_t nowClock = clock();
        if (nowClock >= lastClock + INTERVAL)
        {
            // ボールの位置を更新
            ballX += ballVelocityX;
            ballY += ballVelocityY;

            // 壁との衝突判定
            if (ballX <= 0 || ballX >= FIELD_WIDTH - 1) 
            {
                ballVelocityX *= -1; // ボールの方向を反転
            }
            if (ballY <= 0) 
            {
                ballVelocityY *= -1; // ボールの方向を反転
            }

            // パドルとの衝突判定
            if (ballY == paddleY - 1 && ballX >= paddleX && ballX < paddleX + PADDLE_WIDTH)
            {
                ballVelocityY *= -1; // ボールの方向を反転
                // ボールがパドルのどの位置に当たったかで、ボールの水平方向の速度を調整
                if (ballX < paddleX + PADDLE_WIDTH / 2) 
                {
                    ballVelocityX = -1;
                }
                else
                {
                    ballVelocityX = 1;
                }
            }

            // ブロックとの衝突判定
            for (int x = ballX - 1; x <= ballX + 1; x++) 
            {
                int y = ballY - 1;
                if (y >= 0 && x >= 0 && x < FIELD_WIDTH) 
                {
                    if (field[y][x]) 
                    {
                        field[y][x] = 0; // ブロックの消去
                        ballVelocityY *= -1; // ボールの方向を反転
                    }
                }
            }

            DrawScreen();
            lastClock = nowClock;

            // ゲームオーバー判定
            if (ballY >= FIELD_HEIGHT - 1) 
            {
                Reset(); // ゲームをリセット
            }
        }

        // ユーザーの入力による台の移動
        if (_kbhit()) 
        {
            switch (_getch()) 
            {
            case 'a': paddleX--; break;
            case 'd': paddleX++; break;
            }
            // 台をフィールド内に収める
            if (paddleX < 0)
            {
                paddleX = 0;
            }
            else if (paddleX > FIELD_WIDTH - PADDLE_WIDTH)
            {
                paddleX = FIELD_WIDTH - PADDLE_WIDTH;
            }
            DrawScreen();
        }
    }
    _getch();
}