#ifndef NAVIMAP_H_
#define NAVIMAP_H_

#include "gamecamera.hpp"
#include <GLFW/glfw3.h>
#include "shader.hpp"

class NaviMap
{
public:
    UpCamera up_camera;
    GLuint quadVAO, quadVBO, FBO, RBO, quad_texture;
    Shader map_shader;
    Shader normal_shader;
    int map_height, map_width, window_height, window_width;
    NaviMap(const BasicAsset* target, int map_height, int map_width,
    int window_height, int window_width);
    void draw();
    void create_quad(GLuint &VAO, GLuint &VBO);
    void createFBO(int width, int height, GLuint &texColor);
};
#endif