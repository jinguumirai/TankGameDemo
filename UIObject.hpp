#include "shader.hpp"
#include "GameLib3D.hpp"
#ifndef UIOBJECT_H_
#define UIOBJECT_H_
class UIObject
{
public:
    unsigned int UIVAO, UIVBO, UIEBO;
    unsigned int texture3;
    int height, width, nrChannels;
    unsigned char * data;
    Shader ui_shader;
    UIObject(GameLib3D::Framework*, const char* ui_image_path, const int index);
    void draw();
};
#endif