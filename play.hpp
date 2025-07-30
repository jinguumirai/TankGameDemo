#ifndef PLAY_H_
#define PLAY_H_
#include"playstatus.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "model.hpp"
#include "gamecamera.hpp"
#include "asset.hpp"
#include <unordered_map>
#include <string>
#include "UIObject.hpp"

class Play: public PlayStatus
{
    public:
    UIObject ui_object1;
    UIObject ui_object2;
    UIObject ui_object3;
    UIObject* ui_objects[3];
    const float EPSILON;
    bool is_crash(const BasicAsset * asset1, const BasicAsset * asset2);
    bool is_crash(const BasicAsset * asset1, const std::vector<ParamAsset>& assets);
    bool crash_boxes(const BasicAsset* asset);
    bool crash_objects(const BasicAsset* asset);
    std::vector<ParamAsset> bullets;
    std::vector<ParamAsset> enemy_bullets;
    std::vector<ParamAsset> boxes;
    std::vector<ObjAsset> enemy_tanks;
    std::unordered_map<std::string, unsigned char *> image_data_map;
    std::unordered_map<std::string, int> image_height_map;
    std::unordered_map<std::string, int> image_width_map;
    std::unordered_map<std::string, int> image_nrcomp_map;
    Shader shader;
    Shader simpleDepthShader;
    ObjAsset tank;
    ParamAsset simu_box;
    FollowCamera follow_tank_camera;
    GLuint depthMapFBO;
    GLuint depthCubeMap;
    glm::vec3 lightPos;
    Play(GameLib3D::Framework * fra_instance);
    virtual ~Play();
    void play_init();
    bool in_box(BasicAsset*);
    virtual void draw();
    virtual Base* update(Base* another_status);
};
#endif