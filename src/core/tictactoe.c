#include "tictactoe.h"

#include <stdio.h>

#include "utils/utils.h"

void Print_Tutorial(void)
{
    printf("\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf(" 0 | 1 | 2                                             \n");
    printf(" --+---+--   ->  valid positions                       \n");
    printf(" 3 | 4 | 5                                             \n");
    printf(" --+---+--     type the number of the position         \n");
    printf(" 6 | 7 | 8     of the square you want to play on.      \n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("\n");
}

char Get_Squaretype_Char(const SquareType _square_type)
{
    if (_square_type == SQUARETYPE_PLAYER_ONE)
    {
        return 'X';
    }
    if (_square_type == SQUARETYPE_PLAYER_TWO)
    {
        return 'O';
    }
    if (_square_type == SQUARETYPE_EMPTY)
    {
        return ' ';
    }

    return 'e';
}

void Print_Board(const Board _b)
{
    printf("\n");
    printf(" %c | %c | %c \n", Get_Squaretype_Char(_b.squares[0]),
           Get_Squaretype_Char(_b.squares[1]), Get_Squaretype_Char(_b.squares[2]));
    printf(" --+---+-- \n");
    printf(" %c | %c | %c \n", Get_Squaretype_Char(_b.squares[3]),
           Get_Squaretype_Char(_b.squares[4]), Get_Squaretype_Char(_b.squares[5]));
    printf(" --+---+-- \n");
    printf(" %c | %c | %c \n", Get_Squaretype_Char(_b.squares[6]),
           Get_Squaretype_Char(_b.squares[7]), Get_Squaretype_Char(_b.squares[8]));
    printf("\n");
}

void Print_Board_Status(const BoardStatus _status)
{
    if (_status == BOARDSTATUS_ONGOING)
    {
        printf("Game is still ongoing. \n");
        return;
    }
    if (_status == BOARDSTATUS_PLAYER_ONE_VICTORY)
    {
        printf("Player one won! \n");
        return;
    }
    if (_status == BOARDSTATUS_PLAYER_TWO_VICTORY)
    {
        printf("Player two won! \n");
        return;
    }
    if (_status == BOARDSTATUS_DRAW)
    {
        printf("Game is a draw. \n");
        return;
    }
}

Board Init_Board(void)
{
    Board result;
    for (int i = 0; i < 9; i++)
    {
        result.squares[i] = SQUARETYPE_EMPTY;
    }
    return result;
}

BoardStatus Check_Board_Status(const Board _b)
{
    // check horizontal
    for (int i = 0; i < 3; i++)
    {
        if ((_b.squares[(i * 3) + 0] == _b.squares[(i * 3) + 1])
            && (_b.squares[(i * 3) + 1] == _b.squares[(i * 3) + 2])
            && (_b.squares[i * 3] != SQUARETYPE_EMPTY))
        {
            if (_b.squares[i * 3] == SQUARETYPE_PLAYER_ONE)
            {
                return BOARDSTATUS_PLAYER_ONE_VICTORY;
            }
            else
            {
                return BOARDSTATUS_PLAYER_TWO_VICTORY;
            }
        }
    }

    // check vertical
    for (int i = 0; i < 3; i++)
    {
        if ((_b.squares[i + (0 * 3)] == _b.squares[i + (1 * 3)])
            && (_b.squares[i + (1 * 3)] == _b.squares[i + (2 * 3)])
            && (_b.squares[i] != SQUARETYPE_EMPTY))
        {
            if (_b.squares[i] == SQUARETYPE_PLAYER_ONE)
            {
                return BOARDSTATUS_PLAYER_ONE_VICTORY;
            }
            else
            {
                return BOARDSTATUS_PLAYER_TWO_VICTORY;
            }
        }
    }

    // check diagonal 1
    if ((_b.squares[0] == _b.squares[4]) && (_b.squares[4] == _b.squares[8])
        && (_b.squares[0] != SQUARETYPE_EMPTY))
    {
        if (_b.squares[0] == SQUARETYPE_PLAYER_ONE)
        {
            return BOARDSTATUS_PLAYER_ONE_VICTORY;
        }
        else
        {
            return BOARDSTATUS_PLAYER_TWO_VICTORY;
        }
    }

    // check diagonal 2
    if ((_b.squares[2] == _b.squares[4]) && (_b.squares[4] == _b.squares[6])
        && (_b.squares[2] != SQUARETYPE_EMPTY))
    {
        if (_b.squares[2] == SQUARETYPE_PLAYER_ONE)
        {
            return BOARDSTATUS_PLAYER_ONE_VICTORY;
        }
        else
        {
            return BOARDSTATUS_PLAYER_TWO_VICTORY;
        }
    }

    // check for draw
    bool draw = true;
    for (int i = 0; i < 9; i++)
    {
        if (_b.squares[i] == SQUARETYPE_EMPTY)
        {
            draw = false;
            break;
        }
    }
    if (draw)
    {
        return BOARDSTATUS_DRAW;
    }

    // if all checks fail the game is still ongoing
    return BOARDSTATUS_ONGOING;
}

// returns false if successful, and true if an error ocurred
bool Make_Play(Board* _b, const int _pos, const SquareType _square_type)
{
    if (Is_Play_Out_Of_Bounds(_pos))
    {
        Log_Error("Play position is out of bounds!");
        return true;
    }
    if (_b->squares[_pos] != SQUARETYPE_EMPTY)
    {
        Log_Error("Attempted to play at an occupied position!");
        return true;
    }
    _b->squares[_pos] = _square_type;
    return false;
}

bool Is_Play_Out_Of_Bounds(const int _pos)
{
    return ((_pos < 0) || (_pos > 8));
}
