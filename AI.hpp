#ifndef AI_H_
#define AI_H_
#include "GameLib3D.hpp"

enum EnemyStatus
{
    Random,
    Search,
    NotChange,
};

enum RandomMovement
{
    MoveForward,
    Stepback,
    LeftRotate,
    RightRotate,
    NoAction
};

class AI
{
public:
    EnemyStatus now_status;
    RandomMovement rand_status;
    double pre_time;
    GameLib3D::Framework* frame_instance;
    AI(GameLib3D::Framework* frame_work);

    /// @brief Change the next status per second. 
    /// @return Next random status. If delta time less than 1s, this result would not be changed.
    RandomMovement next_random_status();
};

#endif