#include"play.hpp"
#include <stb_image.h>
#include "Complete.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Play::Play(GameLib3D::Framework* fra_instance):
PlayStatus(fra_instance), EPSILON(1e-5),
tank(this, "resources/model/tank/export3dcoat.obj", 0, {0, -5, -2}, {0.01, 0.01, 0.01}, 1.0, 60.0f),
simu_box(this, "resources/wood.png", 1, 0.0f, 0.0f, glm::vec3(0.0f), 
glm::vec3(10.0f, 5.0f, 10.0f)), follow_tank_camera(&tank),
shader("point_shadows.vs", "point_shadows.fs"), lightPos(0.0f, 0.0f, 0.0f),
simpleDepthShader("point_shadows_depth.vs", "point_shadows_depth.fs", "point_shadows_depth.gs"),
ui_object1(frame_instance, "resources/ui_star.png", 0),
ui_object2(frame_instance, "resources/ui_star.png", 1),
ui_object3(frame_instance, "resources/ui_star.png", 2)
{
    
    boxes.push_back(ParamAsset(this, "resources/wood.png", 0, 0.0f, 0.0f, 
    {-2.0f, -4.5f, -2.0f}, glm::vec3(0.5f)));
    boxes.push_back(ParamAsset(this, "resources/wood.png", 0, 0.0f, 0.0f, 
    {2.0f, -4.5f, 2.0f}, glm::vec3(0.5f)));
    
    ui_objects[0] = &ui_object1;
    ui_objects[1] = &ui_object2;
    ui_objects[2] = &ui_object3;
    int width, height, nrComponents;
    string bullet_texture_name = "resources/bullet.jpg";
    unsigned char * bullet_data = stbi_load(bullet_texture_name.c_str(), 
        &width, &height, &nrComponents, 0);
    image_data_map[bullet_texture_name] = bullet_data;
    image_height_map[bullet_texture_name] = height;
    image_width_map[bullet_texture_name] = width;
    image_nrcomp_map[bullet_texture_name] = nrComponents;
    bullet_texture_name = "resources/enemy_bullet.jpg";
    bullet_data = stbi_load(bullet_texture_name.c_str(), 
        &width, &height, &nrComponents, 0);
    image_data_map[bullet_texture_name] = bullet_data;
    image_height_map[bullet_texture_name] = height;
    image_width_map[bullet_texture_name] = width;
    image_nrcomp_map[bullet_texture_name] = nrComponents;
    glm::vec3 pos_array[] = {{3.0, -5.0, 3.0}, {-3.0, -5.0, -3.0}, {-5.0, -5.0, -5.0},
        {5.0, -5.0, 5.0}};
    for (int i = 0; i < 4; i++)
    {
        enemy_tanks.push_back(ObjAsset(this, "resources/model/Tiger/Tiger_I.obj", 0, 
            pos_array[i], {0.2, 0.2, 0.2}, 1.0, 60.0f, 0.0f));
    }
    
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthCubeMap);
}

void Play::play_init()
{
    glEnable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
        frame_instance->width(), frame_instance->height(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shader.use();
    shader.setInt("diffuseTexture", 0);
    shader.setInt("depthMap", 10);
}

void Play::draw()
{
    play_init();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float near_plane = 1.0f;
    float far_plane = 25.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)frame_instance->width() / (float)frame_instance->height(), near_plane, far_plane);
    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    glViewport(0, 0, frame_instance->width(), frame_instance->height());
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    simpleDepthShader.use();
    for (unsigned int i = 0; i < 6; ++i)
        simpleDepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    simpleDepthShader.setVec3("lightPos", lightPos);
    simpleDepthShader.setFloat("far_plane", far_plane);
    simu_box.draw(simpleDepthShader);
    for (auto& box : boxes)
    {
        box.draw(simpleDepthShader);
    }
    for (auto& bullet : bullets)
    {
        bullet.draw(simpleDepthShader);
    }

    for (auto& enemy_tank: enemy_tanks)
    {
        enemy_tank.draw(simpleDepthShader);
    }
    tank.draw(simpleDepthShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, frame_instance->width(), frame_instance->height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.use();
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 
    (float)frame_instance->width() / (float)frame_instance->height(), 0.1f, 100.0f);
    auto view = follow_tank_camera.get_view();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setVec3("lightPos", lightPos);
    shader.setVec3("viewPos", {0.0f, 0.0f, 0.0f});
    shader.setInt("shadows", true); // enable/disable shadows by pressing 'SPACE'
    shader.setFloat("far_plane", far_plane);
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
    simu_box.draw(shader);
    for (auto& box: boxes)
    {
        box.draw(shader);
    }
    for (auto& bullet : bullets)
    {
        bullet.draw(shader);
    }
    for (auto& enemy_tank: enemy_tanks)
    {
        enemy_tank.draw(shader);
    }
    for (auto& enemy_bullet: enemy_bullets)
    {
        enemy_bullet.draw(shader);
    }
    tank.draw(shader);
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    for (int i = 0; i < tank.life; i++)
    {
        ui_objects[i]->draw();
    }
}

Play::~Play()
{
    for (auto& map_pair: image_data_map)
    {
        stbi_image_free(map_pair.second);
    }
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &depthMapFBO);
    glDeleteTextures(1, &depthCubeMap);
}

Base* Play::update(Base* another_status)
{
    GameLib3D::InputKey input_key = frame_instance->read_once_input();
    if (enemy_tanks.size() <= 0)
    {
        return new Complete("resources/congratulations.png", frame_instance);
    }
    if (input_key == GameLib3D::P)
    {
        return another_status;
    }
    tank.control_update(frame_instance, input_key);
    double delta_time = frame_instance->average_delta_time() / 1000;
    auto bullet_iter = bullets.begin();
    for (int i = 0; i < enemy_tanks.size(); i++)
    {
        enemy_tanks[i].auto_update(frame_instance);
    }
    while (bullet_iter != bullets.end())
    {
        (*bullet_iter).pos_vec += ((*bullet_iter).front_vec() * glm::vec3(bullet_iter->speed) * 
        glm::vec3(delta_time));
        if (!in_box(&(*bullet_iter)))
        {
            bullet_iter = bullets.erase(bullet_iter);
        }
        else
        {
            bullet_iter++;
        }
    }
    auto enemy_bullet_iter = enemy_bullets.begin();
    while (enemy_bullet_iter != enemy_bullets.end())
    {
        (*enemy_bullet_iter).pos_vec += ((*enemy_bullet_iter).front_vec() * 
            glm::vec3(enemy_bullet_iter->speed * delta_time));
        if (!in_box(&(*enemy_bullet_iter)))
        {
            enemy_bullets.erase(enemy_bullet_iter);
        }
        else
        {
            enemy_bullet_iter++;
        }
    }
    bullet_iter = bullets.begin();
    while (bullet_iter != bullets.end())
    {
        auto box_iter = boxes.begin();
        bool crash_flag = false;
        while(box_iter != boxes.end())
        {
            if (is_crash(&(*box_iter), &(*bullet_iter)))
            {
                bullet_iter = bullets.erase(bullet_iter);
                crash_flag = true;
                break;
            }
            box_iter++;
        }
        if (!crash_flag)
        {
            bullet_iter++;
        }
    }
    
    bullet_iter = bullets.begin();
    while (bullet_iter != bullets.end())
    {
        auto enemy_iter = enemy_tanks.begin();
        bool crash_flag = false;
        while(enemy_iter != enemy_tanks.end())
        {
            if (is_crash(&(*enemy_iter), &(*bullet_iter)))
            {
                enemy_tanks.erase(enemy_iter);
                bullet_iter = bullets.erase(bullet_iter);
                crash_flag = true;
                break;
            }
            else
            {
                enemy_iter++;
            }
        }
        if (!crash_flag)
        {
            bullet_iter++;
        }
    }
    enemy_bullet_iter = enemy_bullets.begin();
    while (enemy_bullet_iter != enemy_bullets.end())
    {
        auto box_iter = boxes.begin();
        bool crash_flag = false;
        while (box_iter != boxes.end())
        {
            if (is_crash(&(*box_iter), &(*enemy_bullet_iter)))
            {
                enemy_bullets.erase(enemy_bullet_iter);
                crash_flag = true;
                break;
            }
            box_iter++;
        }
        if (!crash_flag && is_crash(&(*enemy_bullet_iter), &tank))
        {
            crash_flag = true;
            enemy_bullets.erase(enemy_bullet_iter);
            tank.life--;
            if (tank.life <= 0)
            {
                return new Complete("resources/gameover.jpg", frame_instance);
            }
        }
        if (!crash_flag)
        {
            enemy_bullet_iter++;
        }
    }
    this->draw();
    return this;
}

bool Play::in_box(BasicAsset* asset)
{
    auto asset_edges = asset->edge_vector();
    auto box_edges = simu_box.edge_vector();

    if (asset_edges[0] <= box_edges[0] + EPSILON && asset_edges[1] >= box_edges[1] - EPSILON && 
    asset_edges[2] <= box_edges[2] + EPSILON && asset_edges[3] >= box_edges[3] - EPSILON && asset_edges[4] <= box_edges[4] + EPSILON &&
    asset_edges[5] >= box_edges[5] - EPSILON)
    {
        return true;
    }
    else 
    {
        return false;
    }
}

bool Play::is_crash(const BasicAsset * asset1, const BasicAsset * asset2)
{
    auto edge1 = asset1->edge_vector();
    auto edge2 = asset2->edge_vector();
    if (edge1[1] <= edge2[0] + EPSILON && edge2[1] <= edge1[0] + EPSILON && edge1[3] <= edge2[2] + EPSILON && 
    edge2[3] <= edge1[2] + EPSILON && edge1[5] <= edge2[4] + EPSILON && edge2[5] <= edge1[4] + EPSILON)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Play::is_crash(const BasicAsset * asset1, const std::vector<ParamAsset>& assets)
{
    for (int i = 0; i < assets.size(); i++)
    {
        
        if (is_crash(asset1, &assets[i]))
        {
            return true;
        }
    }
    return false;
}

bool Play::crash_boxes(const BasicAsset* asset)
{
    return is_crash(asset, boxes);
}

bool Play::crash_objects(const BasicAsset* asset)
{
    if (asset != &tank && is_crash(&tank, asset))
    {
        return true;
    }

    for (int i = 0; i < boxes.size(); i++)
    {
        if (asset != &boxes[i] && is_crash(asset, &boxes[i]))
        {
            return true;
        }
    }

    for (int i = 0; i < enemy_tanks.size(); i++)
    {
        if (asset != &enemy_tanks[i] && is_crash(asset, &enemy_tanks[i]))
        {
            return true;
        }
    }
    return false;
}