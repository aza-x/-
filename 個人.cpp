#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define FIELD_WIDTH  (14)
#define FIELD_HEIGHT (FIELD_WIDTH * 2)
#define FPS   (10)
#define INTERVAL (1000 / FPS)
#define PADDLE_WIDTH (4)

// �{�[���ƃp�h���̍��W�Ƒ��x
int ballX, ballY, ballVelocityX, ballVelocityY;
int paddleX, paddleY;

// �Q�[���t�B�[���h
int field[FIELD_HEIGHT][FIELD_WIDTH];

// ��ʂ�`�悷��֐�
void DrawScreen()
{
    // ��ʂ��N���A
    system("cls");

    // �t�B�[���h�̏㉺�̘g��`��
    for (int x = 0; x < FIELD_WIDTH + 2; x++) 
    {
        printf("��");
    }
    printf("\n");

    // �t�B�[���h�̒���������`��
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        printf("��");
        for (int x = 0; x < FIELD_WIDTH; x++) {
            if ((x == ballX) && (y == ballY)) {
                printf("��"); // �{�[��
            }
            else if ((y == paddleY) && (x >= paddleX) && (x < paddleX + PADDLE_WIDTH)) {
                printf("��"); // �ړ������
            }
            else if (field[y][x]) {
                printf("��"); // �u���b�N
            }
            else {
                printf("�@"); // �����Ȃ��X�y�[�X
            }
        }
        printf("��");
        printf("\n");
    }

    // �t�B�[���h�̉��̘g��`��
    for (int x = 0; x < FIELD_WIDTH + 2; x++)
    {
        printf("��");
    }
}

// �Q�[�������Z�b�g����֐�
void Reset()
{
    // �{�[���̏����ʒu�Ƒ��x�������_���ɐݒ�
    ballX = rand() % FIELD_WIDTH;
    ballY = FIELD_HEIGHT / 3;
    ballVelocityX = (rand() % 2) ? 1 : -1;
    ballVelocityY = 1;

    // �p�h���̏����ʒu��ݒ�
    paddleX = (FIELD_WIDTH - PADDLE_WIDTH) / 2;
    paddleY = FIELD_HEIGHT - 3;

    // �t�B�[���h�̏㕔�Ƀu���b�N��ݒu
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
            // �{�[���̈ʒu���X�V
            ballX += ballVelocityX;
            ballY += ballVelocityY;

            // �ǂƂ̏Փ˔���
            if (ballX <= 0 || ballX >= FIELD_WIDTH - 1) 
            {
                ballVelocityX *= -1; // �{�[���̕����𔽓]
            }
            if (ballY <= 0) 
            {
                ballVelocityY *= -1; // �{�[���̕����𔽓]
            }

            // �p�h���Ƃ̏Փ˔���
            if (ballY == paddleY - 1 && ballX >= paddleX && ballX < paddleX + PADDLE_WIDTH)
            {
                ballVelocityY *= -1; // �{�[���̕����𔽓]
                // �{�[�����p�h���̂ǂ̈ʒu�ɓ����������ŁA�{�[���̐��������̑��x�𒲐�
                if (ballX < paddleX + PADDLE_WIDTH / 2) 
                {
                    ballVelocityX = -1;
                }
                else
                {
                    ballVelocityX = 1;
                }
            }

            // �u���b�N�Ƃ̏Փ˔���
            for (int x = ballX - 1; x <= ballX + 1; x++) 
            {
                int y = ballY - 1;
                if (y >= 0 && x >= 0 && x < FIELD_WIDTH) 
                {
                    if (field[y][x]) 
                    {
                        field[y][x] = 0; // �u���b�N�̏���
                        ballVelocityY *= -1; // �{�[���̕����𔽓]
                    }
                }
            }

            DrawScreen();
            lastClock = nowClock;

            // �Q�[���I�[�o�[����
            if (ballY >= FIELD_HEIGHT - 1) 
            {
                Reset(); // �Q�[�������Z�b�g
            }
        }

        // ���[�U�[�̓��͂ɂ���̈ړ�
        if (_kbhit()) 
        {
            switch (_getch()) 
            {
            case 'a': paddleX--; break;
            case 'd': paddleX++; break;
            }
            // ����t�B�[���h���Ɏ��߂�
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