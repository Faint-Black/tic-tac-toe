#include "bot_algorithm.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "tictactoe.h"
#include "utils/utils.h"

typedef struct ThreadData
{
    const Board*       board;
    int                index;
    PossibilityMatrix* output;
} ThreadData;

// algorithm functions
static PossibilityMatrix Synchronous_Iteration(const Board* _b, int _index);
static void*             Threaded_Iteration(void* _arg);
static PossibilityMatrix Init_Possibilitymatrix(void);
static PossibilityMatrix Sum_Matrices(PossibilityMatrix _a, PossibilityMatrix _b);
static PossibilityNode   Init_Possibilitynode(void);
static void              Recursive_Loop(int _depth, int _origin_pos, PossibilityNode* _parent,
                                        PossibilityMatrix* _matrix);
static void              Make_Child(PossibilityNode* _parent, int _origin_pos, int _index,
                                    PossibilityMatrix* _matrix);
static void              Recursive_Infanticide(PossibilityNode* _parent);
// helper functions
static SquareType Get_Current_Turn(const Board* _b);
static int        Get_Indexed_Empty_Square(const Board* _b, int _index);

// Analyzes the board and returns the best possible play position
int Get_Best_Play(const Board* _b, bool _multithread)
{
    PossibilityMatrix all_positions_results = Init_Possibilitymatrix();

    if (_multithread)
    {
        printf("[multithreading enabled!]\n");

        // thread creation
        int               thread_status;
        pthread_t         threads[9];
        ThreadData        thread_data[9];
        PossibilityMatrix results_array[9];
        for (int i = 0; i < 9; i++)
        {
            results_array[i]      = Init_Possibilitymatrix();
            thread_data[i].board  = _b;
            thread_data[i].index  = i;
            thread_data[i].output = &results_array[i];
            thread_status = pthread_create(&threads[i], NULL, Threaded_Iteration, &thread_data[i]);
            if (thread_status != 0)
            {
                Fatal_Error("Failed to create thread!");
            }
        }

        // wait for the threads to finish
        for (int i = 0; i < 9; i++)
        {
            thread_status = pthread_join(threads[i], NULL);
            if (thread_status != 0)
            {
                Fatal_Error("Failed to join threads!");
            }
        }

        // sum the matrices and get the result
        for (int i = 0; i < 9; i++)
        {
            all_positions_results = Sum_Matrices(all_positions_results, results_array[i]);
        }
    }
    else
    {
        for (int i = 0; i < 9; i++)
        {
            all_positions_results =
                Sum_Matrices(Synchronous_Iteration(_b, i), all_positions_results);
        }
    }

    // see all victory positions
    printf("Victory positions: ");
    for (int i = 0; i < 9; i++)
    {
        if (i % 3 == 0)
        {
            printf("\n");
        }
        printf("%d ", all_positions_results.victories[i]);
    }
    printf("\n\n");

    // see all defeat positions
    printf("Defeat positions: ");
    for (int i = 0; i < 9; i++)
    {
        if (i % 3 == 0)
        {
            printf("\n");
        }
        printf("%d ", all_positions_results.defeats[i]);
    }
    printf("\n\n");

    // see all draw positions
    printf("Draw positions: ");
    for (int i = 0; i < 9; i++)
    {
        if (i % 3 == 0)
        {
            printf("\n");
        }
        printf("%d ", all_positions_results.draws[i]);
    }
    printf("\n\n");

    // check for lowest defeat num and pos, that isn't occupied
    int defeat_min_pos = 0;
    int defeat_min     = 99999999;
    for (int i = 0; i < 9; i++)
    {
        if ((all_positions_results.defeats[i] < defeat_min) && (_b->squares[i] == SQUARETYPE_EMPTY))
        {
            defeat_min     = all_positions_results.defeats[i];
            defeat_min_pos = i;
        }
    }

    int victory_max_pos = Biggest_Num_Position_In_Array(all_positions_results.victories, 9);
    int victory_max     = all_positions_results.victories[victory_max_pos];

    if ((victory_max == 1) || (victory_max == 15))
    {
        printf("the algorithm chose %d because it has an immediate victory spot\n",
               victory_max_pos);
        return victory_max_pos;
    }
    else
    {
        printf("the algorithm chose %d because it has the least ammount of defeats\n",
               defeat_min_pos);
        return defeat_min_pos;
    }
}

PossibilityMatrix Synchronous_Iteration(const Board* _b, int _index)
{
    int origin_pos = Get_Indexed_Empty_Square(_b, _index);
    if (origin_pos == -1)
    {
        return Init_Possibilitymatrix();
    }

    PossibilityMatrix endpoint_results = Init_Possibilitymatrix();

    PossibilityNode* root = (PossibilityNode*)malloc(sizeof(PossibilityNode));
    *root                 = Init_Possibilitynode();
    root->board           = *_b;

    Make_Play(&root->board, origin_pos, Get_Current_Turn(&root->board));

    Recursive_Loop(9, origin_pos, root, &endpoint_results);

    // free allocated memory
    Recursive_Infanticide(root);

    return endpoint_results;
}

void* Threaded_Iteration(void* _arg)
{
    ThreadData data = *(ThreadData*)_arg;

    *data.output = Synchronous_Iteration(data.board, data.index);

    return NULL;
}

PossibilityMatrix Init_Possibilitymatrix(void)
{
    PossibilityMatrix result;
    for (int i = 0; i < 9; i++)
    {
        result.victories[i] = 0;
        result.defeats[i]   = 0;
        result.draws[i]     = 0;
        result.ongoings[i]  = 0;
    }

    return result;
}

PossibilityMatrix Sum_Matrices(PossibilityMatrix _a, PossibilityMatrix _b)
{
    PossibilityMatrix result;
    for (int i = 0; i < 9; i++)
    {
        result.victories[i] = _a.victories[i] + _b.victories[i];
        result.defeats[i]   = _a.defeats[i] + _b.defeats[i];
        result.draws[i]     = _a.draws[i] + _b.draws[i];
        result.ongoings[i]  = _a.ongoings[i] + _b.ongoings[i];
    }

    return result;
}

PossibilityNode Init_Possibilitynode(void)
{
    PossibilityNode result;
    result.parent = NULL;
    for (int i = 0; i < 9; i++)
    {
        result.children[i] = NULL;
    }
    result.board = Init_Board();

    return result;
}

void Recursive_Loop(int _depth, int _origin_pos, PossibilityNode* _parent,
                    PossibilityMatrix* _matrix)
{
    if (_depth <= 0)
    {
        return;
    }

    // make and execute the derived possibility nodes
    for (int i = 0; i < 9; i++)
    {
        Make_Child(_parent, _origin_pos, i, _matrix);
    }

    // for every allocated child, perform the recursive loop
    for (int i = 0; i < 9; i++)
    {
        if (_parent->children[i] != NULL)
        {
            Recursive_Loop(_depth - 1, _origin_pos, _parent->children[i], _matrix);
        }
    }
}

void Make_Child(PossibilityNode* _parent, int _origin_pos, int _index, PossibilityMatrix* _matrix)
{
    if (Get_Indexed_Empty_Square(&_parent->board, _index) == -1)
    {
        return;
    }

    PossibilityNode* child    = (PossibilityNode*)malloc(sizeof(PossibilityNode));
    *child                    = Init_Possibilitynode();
    child->board              = _parent->board;
    child->parent             = _parent;
    _parent->children[_index] = child;

    Make_Play(&child->board, Get_Indexed_Empty_Square(&child->board, _index),
              Get_Current_Turn(&child->board));

    // change harcoded turn for algorithm
    int status = Check_Board_Status(child->board);
    if (status == BOARDSTATUS_PLAYER_ONE_VICTORY)
    {
        _matrix->defeats[_origin_pos] += 1;
    }
    if (status == BOARDSTATUS_PLAYER_TWO_VICTORY)
    {
        _matrix->victories[_origin_pos] += 1;
    }
    if (status == BOARDSTATUS_DRAW)
    {
        _matrix->draws[_origin_pos] += 1;
    }
    if (status == BOARDSTATUS_ONGOING)
    {
        _matrix->ongoings[_origin_pos] += 1;
    }
}

// free all memory allocated in a possibilityNode tree
// TODO: rename function
void Recursive_Infanticide(PossibilityNode* _parent)
{
    if (_parent == NULL)
    {
        return;
    }

    bool child_has_child = false;
    for (int i = 0; i < 9; i++)
    {
        PossibilityNode* child = _parent->children[i];
        if (child == NULL)
        {
            continue;
        }

        // check if the child is a parent
        for (int i = 0; i < 9; i++)
        {
            if (child->children[i] != NULL)
            {
                child_has_child = true;
                break;
            }
        }
        // if not, delete it safely
        if (child_has_child == false)
        {
            free(child);
        }
        else
        {
            Recursive_Infanticide(child);
        }
    }

    free(_parent);
}

//----------------------------------------------------------------------------

SquareType Get_Current_Turn(const Board* _b)
{
    // reminder that player_one('X') will always play first

    int one = 0;
    int two = 0;
    for (int i = 0; i < 9; i++)
    {
        if (_b->squares[i] == SQUARETYPE_PLAYER_ONE)
        {
            one += 1;
        }
        if (_b->squares[i] == SQUARETYPE_PLAYER_TWO)
        {
            two += 1;
        }
    }

    if (one == two)
    {
        return SQUARETYPE_PLAYER_ONE;
    }
    else
    {
        return SQUARETYPE_PLAYER_TWO;
    }
}

// returns the square position of the indexed empty square of a board
// returns -1 if it doesnt find a single empty square
//
//  example: index = 1
//  O |   | X
//  --+---+--
//    | O | O
//  --+---+--
//  X | O | X
//
//  returns 3, which is the position of the second empty square (zero based index)
int Get_Indexed_Empty_Square(const Board* _b, int _index)
{
    int result = -1;
    for (int i = 0; i < 9; i++)
    {
        if (_b->squares[i] == SQUARETYPE_EMPTY)
        {
            if (_index == 0)
            {
                result = i;
                return result;
            }
            else
            {
                _index -= 1;
            }
        }
    }
    return result;
}
