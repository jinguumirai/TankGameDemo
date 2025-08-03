#include "UIObject.hpp"
#include <stb_image.h>

UIObject::UIObject(GameLib3D::Framework* framework, const char* ui_image_path, const int index):
ui_shader("ui.vs", "ui.fs")
{
    data = stbi_load(ui_image_path, &width, &height, &nrChannels, 0);
    if (data)
    {
        float width_scale = (float)width / framework->width() * 2;
        float height_scale = (float)height / framework->height() * 2;
        float height_start = 1.0 - height_scale;
        
        
        float ui_vertices[] = {
            (index + 1) * width_scale - 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
            (index + 1) * width_scale - 1.0f, height_start, 0.0f, 0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
            index * width_scale - 1.0f, height_start, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
            index * width_scale - 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,   0.0f, 1.0f
        };
        unsigned int ui_indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };
        glGenVertexArrays(1, &UIVAO);
        glGenBuffers(1, &UIVBO);
        glGenBuffers(1, &UIEBO);
        glBindVertexArray(UIVAO);

        glBindBuffer(GL_ARRAY_BUFFER, UIVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ui_vertices), ui_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, UIEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ui_indices), ui_indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glGenTextures(1, &texture3);
        glBindTexture(GL_TEXTURE_2D, texture3); 
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if (nrChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(data);
        ui_shader.use();
        glUniform1i(glGetUniformLocation(ui_shader.ID, "texture1"), 2);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}

void UIObject::draw()
{
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture3);
    ui_shader.use();
    glBindVertexArray(UIVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glEnable(GL_DEPTH_TEST);
}