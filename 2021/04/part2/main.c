#include <stdio.h>

#if 0
#define INPUT "input.txt"
#define DRAW_SIZE 27 
#define BOARDS 3
#else
#define INPUT "input2.txt"
#define DRAW_SIZE 100
#define BOARDS 100
#endif

#define BOARD_ROWS 5
#define BOARD_COLS 5
#define BOARD_SIZE (BOARD_ROWS * BOARD_COLS)

typedef enum {
    FALSE,
    TRUE
} boolean;

typedef struct {
    int value;
    boolean mark;
} entry;

typedef struct {
    boolean bingo;
    int board;
    int score;
} game;

void parse_file(char *filename, int *draw_number, int *drawCount, entry *board_array, int *boardCount);
game bingoCheck(entry *board_array, int length, int draw, game *result, int *winCount, int boardCount);

int main(void)
{
    int draw_number[DRAW_SIZE];
    entry board_array[BOARD_SIZE * BOARDS];
    int drawCount = 0;
    int boardCount = 0;

    parse_file(INPUT, draw_number, &drawCount, board_array, &boardCount);


    int winCount = 0;
    game result[BOARDS] = {0}; 
    int length = boardCount * BOARD_SIZE;

    for (int i = 0; i < drawCount; i++)
    {
        int draw = draw_number[i];

        game lastWinner = bingoCheck(board_array, length, draw, result, &winCount, boardCount);

        if (lastWinner.bingo == TRUE)
        {
            if (winCount == boardCount)
            {
                printf("Board %d won last with score %d!\n", lastWinner.board, lastWinner.score);
                return 0;
            }
        }
    }
}

void parse_file(char *filename, int *draw_number, int *drawCount, entry *board_array, int *boardCount)
{
    
    FILE *file = fopen(filename, "r");
    char c;
    int value = 0;
    int boardIndex = 0;

    boolean DRAW_NUMBER_PARSE = TRUE;
    boolean BOARD_PARSE = FALSE;
    boolean PARSING_NUMBER = FALSE;

    while ((c = getc(file)) != EOF)
    {
        if (DRAW_NUMBER_PARSE)
        {
            if (c != ',' && c != '\n')
            {
                value = value * 10 + (c - '0');        
                draw_number[*drawCount] = value;
            }
            else
            {
                value = 0;
                (*drawCount)++;
            }
        }

        if (c == '\n' && DRAW_NUMBER_PARSE)
        {
            DRAW_NUMBER_PARSE = FALSE;
            BOARD_PARSE = TRUE;
        }

        if (BOARD_PARSE)
        {
            // read boards into one array 
            // each board starts at boardIndex - boardIndex % BOARD_SIZE 
            if (c != ' ' && c != '\n')
            {
                if (!PARSING_NUMBER)
                {
                    PARSING_NUMBER = TRUE;
                }
                value = value * 10 + (c - '0');        
            }
            // PARSING_NUMBER for catching double whitespace
            else if ((c == ' ' || c == '\n') && PARSING_NUMBER)
            {
                PARSING_NUMBER = FALSE;
                board_array[boardIndex].value = value;
                board_array[boardIndex].mark = FALSE;
                boardIndex++;
                value = 0;
            }
        }
    }
    *boardCount = boardIndex / BOARD_SIZE;
}

game bingoCheck(entry *board_array, int length, int draw, game *result, int *winCount, int boardCount)
{
    int lastWinnerBoardIndex = 0;
    for (int i = 0; i < length; i++)
    {
        if (board_array[i].value == draw)
        {
            board_array[i].mark = TRUE;

            // check current board for bingo
            int col = i % BOARD_COLS; // relative column
            int index = i - (i % BOARD_SIZE); // absolute index for current board
            int row = (i - col) - index; // relative row

            entry *board = &board_array[index]; 
            int rowChecksum = 0;
            int colChecksum = 0;
            for (int j = 0; j < BOARD_ROWS; j++)
            {
                // check column for BINGO 
                if (board[col + BOARD_COLS * j].mark == TRUE)
                {
                    colChecksum += 1;
                }

                if (colChecksum == 5)
                {
                    int score = 0;
                    for (int k = 0; k < BOARD_SIZE; k++)
                    {
                        if (board[k].mark == FALSE)
                        {
                            score += board[k].value;
                        }
                    }
                    score *= draw;
                    int winnerBoard = i / 25 + 1;
                    game currentWinner = {TRUE, winnerBoard, score};
                    if (result[winnerBoard - 1].bingo == FALSE)
                    {
                        result[winnerBoard - 1] = currentWinner;
                        (*winCount)++;
                        lastWinnerBoardIndex = winnerBoard - 1;
                        if (*winCount == boardCount)
                        {
                            return result[lastWinnerBoardIndex];
                        }
                    }
                }
            }

            // check row for BINGO
            for (int j = 0; j < BOARD_COLS; j++)
            {
                if (board[row + j].mark == TRUE)
                {
                    rowChecksum += 1;
                }

                if (rowChecksum == 5)
                {
                    int score = 0;
                    for (int k = 0; k < BOARD_SIZE; k++)
                    {
                        if (board[k].mark == FALSE)
                        {
                            score += board[k].value;
                        }
                    }
                    score *= draw;
                    int winnerBoard = i / 25 + 1;
                    game currentWinner = {TRUE, winnerBoard, score};
                    if (result[winnerBoard - 1].bingo == FALSE)
                    {
                        result[winnerBoard - 1] = currentWinner;
                        (*winCount)++;
                        lastWinnerBoardIndex = winnerBoard - 1;
                        if (*winCount == boardCount)
                        {
                            return result[lastWinnerBoardIndex];
                        }
                    }
                }
            }
        }
    }
    return result[lastWinnerBoardIndex];
}
