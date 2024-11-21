#pragma once

#include "tictactoe.h"

typedef struct PossibilityNode
{
    struct PossibilityNode* parent;       // non-owning pointer
    struct PossibilityNode* children[9];  // all owning pointers
    Board                   board;        // current imaginary board of this possibility node
} PossibilityNode;

typedef struct PossibilityMatrix
{
    int victories[9];
    int defeats[9];
    int draws[9];
    int ongoings[9];
} PossibilityMatrix;

extern int Get_Best_Play(const Board* _b, bool _multithread);
