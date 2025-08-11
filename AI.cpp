#include "AI.hpp"
#include <random>

AI::AI(GameLib3D::Framework* frame_work)
{
    now_status = EnemyStatus::Random;
    rand_status = RandomMovement::NoAction;
    pre_time = frame_work->get_time();
    frame_instance = frame_work;
}

RandomMovement AI::next_random_status()
{
    double current_time = frame_instance->get_time();
    if (current_time - pre_time >= 0.5)
    {
        pre_time = current_time;
        /*
        std::mt19937 move(static_cast<unsigned int>(std::time(nullptr)));
        std::uniform_int_distribution<int> dist(0, RandomMovement::NoAction - 1);
        RandomMovement next_movement = static_cast<RandomMovement>(dist(move));
        */
        RandomMovement next_movement = rand_status;
        int rand_index = rand() % 5;
        switch (rand_index)
        {
        case 0:
            next_movement = RandomMovement::MoveForward;
            break;

        case 1:
            next_movement = RandomMovement::Stepback;
            break;
        
        case 2:
            next_movement = RandomMovement::LeftRotate;
            break;

        case 3:
            next_movement = RandomMovement::RightRotate;
            break;

        default:
            next_movement = RandomMovement::NoAction;
            break;
        }
        rand_status = next_movement;
        return next_movement;
    }
    else
    {
        return rand_status;
    }
}