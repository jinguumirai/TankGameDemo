#ifndef ASSET_H_
#define ASSET_H_
#include"playstatus.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "model.hpp"
#include "GameLib3D.hpp"
#include <stb_image.h>
#include "AI.hpp"

class Play;

class BasicAsset
{
    public:
    float speed;
    float rotate_speed;
    float radians_angle();
    float cos_angle();
    float sin_angle();
    float x_max, x_min, y_max, y_min, z_max, z_min;
    std::vector<float> edge_vector() const;
    glm::vec3 ori_front;
    glm::vec3 pos_vec;
    float rotated_angle;
    glm::vec3 scale_vec;
    int reverse_normals;
    /// @brief return model matrix which used in rastrization.
    /// @return model matrix which used in rastrization.
    glm::mat4 model_mat();
    BasicAsset(glm::vec3 pos, glm::vec3 scale, float rotated_angle, glm::vec3 ori_front, int reverse_normal, float speed, float rotate_speed);
    virtual ~BasicAsset();
    virtual void draw(Shader& shader) = 0;
    glm::vec3 front_vec();
};

class ObjAsset: public BasicAsset
{
    public:
    int life;
    AI ai;
    float cool_down;
    Play* play_instance;
    Model model;
    double view_distance;

    /// @brief Initialize ObjAsset.
    /// @param play_instance The pointer of Play, must not be null.
    /// @param object_path The Object path string, must not be null.
    /// @param reverse_normal If the object is skybox, set it to 1 to reverse normal vector to avoid rendering error of the box which used as skybox.  
    /// @param pos_vec The position vector of object.
    /// @param scale_vec The scale vector of object.
    /// @param speed The speed of object whose unit is m/s.
    /// @param rotate_speed The speed of rotate whose unit is angle system/s.
    /// @param rotated_angle The angle of rotate whose unit is angle system.
    /// @param ori_front The front vector of object.
    ObjAsset(Play* play_instance, const char * object_path, int reverse_normal, glm::vec3 pos_vec, 
    glm::vec3 scale_vec, float speed, float rotate_speed=0.0f, float rotated_angle=0.f, 
    glm::vec3 ori_front={0.0f, 0.0f, 1.0f});
    virtual void draw(Shader& shader) override;

    /// @brief The object which controlled by plyer update function.
    /// @param frame_work The pointer of frame work and it must not be null.
    /// @param once_key The once key readed.
    virtual void control_update(GameLib3D::Framework* frame_work, GameLib3D::InputKey once_key);
    virtual void auto_update(GameLib3D::Framework* frame_work);

    /// @brief Get the result that this asset could see other_asset
    /// @param asset Other asset
    /// @return If this asset could see other_asset, return true, else return false.
    virtual bool could_see(BasicAsset& other_asset);
};

class ParamAsset: public BasicAsset
{
    public:
    GLuint VAO, VBO;
    GLuint texture;
    Play* play_instance;
    float rotate_speed;
    std::string texture_path;
    ParamAsset(const ParamAsset&);

    /// @brief Initialize ParamAsset.
    /// @param play_instance The pointer of Play, must not be null.
    /// @param texture_path The path of texture, must bot be null.
    /// @param reverse_normal If the object is skybox, set it to 1 to reverse normal vector to avoid rendering error of the box which used as skybox.  
    /// @param rotate_speed The speed of rotate whose unit is angle system/s.
    /// @param speed The speed of ParamAsset whose unit is m/s.
    /// @param pos_vec The position vector of ParamAsset.
    /// @param scale The scale vector of ParamAsset.
    /// @param rotated_angle The angle of rotate whose unit is angle system.
    /// @param ori_front The front vector of object.
    ParamAsset(Play* play_instance, const char* texture_path, int reverse_normal, float rotate_speed=0.0f, 
    float speed=0.0f, glm::vec3 pos_vec={0.0f, 0.0f, 0.0f}, glm::vec3 scale=glm::vec3(1.0f),
    float rotated_angle=0.0f, glm::vec3 ori_front={0.0f, 0.0f, -1.0f});
    
    ParamAsset& operator=(const ParamAsset& another);
    virtual void draw(Shader& shader);
    virtual ~ParamAsset();
};

#endif