#include <stdio.h>

#include "core/bot_algorithm.h"
#include "core/tictactoe.h"
#include "utils/utils.h"

static void Human_Play(Board* _b, SquareType _square_type);

#define YES_MULTITHREAD 1
#define NO_MULTITHREAD  0

int main(void)
{
    Board       main_board = Init_Board();
    BoardStatus board_status;

    Print_Tutorial();
    while (true)
    {
        // your turn
        Print_Board(main_board);
        Human_Play(&main_board, SQUARETYPE_PLAYER_ONE);
        board_status = Check_Board_Status(main_board);
        if (board_status != BOARDSTATUS_ONGOING)
        {
            Print_Board(main_board);
            Print_Board_Status(board_status);
            return 0;
        }

        // computer's turn
        Print_Board(main_board);
        Make_Play(&main_board, Get_Best_Play(&main_board, YES_MULTITHREAD), SQUARETYPE_PLAYER_TWO);
        board_status = Check_Board_Status(main_board);
        if (board_status != BOARDSTATUS_ONGOING)
        {
            Print_Board(main_board);
            Print_Board_Status(board_status);
            return 0;
        }
    }

    return 0;
}

void Human_Play(Board* _b, SquareType _square_type)
{
    while (true)
    {
        printf("type the position of your \'%c\' play: ", Get_Squaretype_Char(_square_type));
        int square_pos = Get_User_Character_Input() - '0';
        if (Is_Play_Out_Of_Bounds(square_pos))
        {
            printf("invalid square! choose again. \n");
            continue;
        }
        if (Make_Play(_b, square_pos, _square_type))
        {
            printf("occupied square! choose again. \n");
            continue;
        }
        return;
    }
}
