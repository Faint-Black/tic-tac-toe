#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef enum SquareType
{
    SQUARETYPE_EMPTY,       // ' '
    SQUARETYPE_PLAYER_ONE,  // 'X'
    SQUARETYPE_PLAYER_TWO,  // 'O'
} SquareType;

typedef enum BoardStatus
{
    BOARDSTATUS_ONGOING,
    BOARDSTATUS_PLAYER_ONE_VICTORY,
    BOARDSTATUS_PLAYER_TWO_VICTORY,
    BOARDSTATUS_DRAW,
} BoardStatus;

typedef struct Board
{
    SquareType squares[9];
} Board;

extern void        Print_Tutorial(void);
extern char        Get_Squaretype_Char(SquareType _square_type);
extern void        Print_Board(Board _b);
extern void        Print_Board_Status(BoardStatus _status);
extern Board       Init_Board(void);
extern BoardStatus Check_Board_Status(Board _b);
extern bool        Make_Play(Board* _b, int _pos, SquareType _square_type);
extern bool        Is_Play_Out_Of_Bounds(int _pos);
