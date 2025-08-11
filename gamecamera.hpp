#ifndef GAME_CAMERA_H_
#define GAME_CAMERA_H_
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.hpp"

class BasicAsset;

class FollowCamera: public Camera
{
    public:
    BasicAsset * const follow_object;
    FollowCamera(BasicAsset * const origin_object);

    virtual glm::mat4 get_view() override;
};

class UpCamera: public Camera
{
public:
    const BasicAsset * follow_object;
    UpCamera(const BasicAsset* origin_object);
    virtual glm::mat4 get_view() override;
};
#endif