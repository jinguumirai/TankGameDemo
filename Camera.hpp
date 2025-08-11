#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>

class Camera
{
public:
    virtual glm::mat4 get_view() = 0;
};
#endif