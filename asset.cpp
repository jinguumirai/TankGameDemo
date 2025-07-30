#include"asset.hpp"
#include "play.hpp"

float BasicAsset::radians_angle()
{
    return glm::radians(rotated_angle);
}

float BasicAsset::cos_angle()
{
    return cos(double(radians_angle()));
}

float BasicAsset::sin_angle()
{
    return sin(radians_angle());
}

glm::mat4 BasicAsset::model_mat()
{
    auto model = glm::mat4(1.0);
    model = glm::translate(model, pos_vec);
    model = glm::scale(model, scale_vec);
    model = glm::rotate(model, glm::radians(rotated_angle), glm::vec3(0.0f, 1.0f, 0.0f));
    return model;
}

BasicAsset::BasicAsset(glm::vec3 pos, glm::vec3 scale, float rotated_angle,
glm::vec3 ori_front, int reverse_normal, float speed, float rotate_speed):
pos_vec(pos), scale_vec(scale), rotated_angle(rotated_angle), ori_front(ori_front), 
reverse_normals(reverse_normal), speed(speed), rotate_speed(rotate_speed)
{
    ;
}

BasicAsset::~BasicAsset()
{
    ;
}

/// @brief get the asset's front vector
/// @return the normalized front vector
glm::vec3 BasicAsset::front_vec()
{
    ori_front = glm::normalize(ori_front);
    float front_x = ori_front.x;
    float front_z = ori_front.z;
    glm::vec3 return_vec = {front_x * cos_angle() + front_z * sin_angle(),
    0, front_z * cos(glm::radians(rotated_angle)) - front_x * sin(glm::radians(rotated_angle))};
    return glm::normalize(return_vec);
}

ParamAsset& ParamAsset::operator=(const ParamAsset& another)
{
    BasicAsset::pos_vec = another.pos_vec;
    BasicAsset::speed = another.speed;
    BasicAsset::rotate_speed = another.rotate_speed;
    BasicAsset::x_min = another.x_min;
    BasicAsset::x_max = another.x_max;
    BasicAsset::y_max = another.y_max;
    BasicAsset::y_min = another.y_min;
    BasicAsset::z_max = another.z_max;
    BasicAsset::z_min = another.z_min;
    BasicAsset::ori_front = another.ori_front;
    BasicAsset::rotated_angle = another.rotated_angle;
    BasicAsset::reverse_normals = another.reverse_normals;
    BasicAsset::scale_vec = another.scale_vec;
    texture_path = another.texture_path;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, nrComponents;
    unsigned char *data = stbi_load(texture_path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
            
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << texture_path << std::endl;
        stbi_image_free(data);
    }

    GLfloat vertices[] = {
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
        // bottom face
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left      
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
        
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

    glBindVertexArray(0);
    return *this;
}

ParamAsset::ParamAsset(const ParamAsset& another):
BasicAsset(another.pos_vec, another.scale_vec, another.rotated_angle, another.ori_front, 
another.reverse_normals, another.speed, another.rotate_speed)
{
    x_max = another.x_max;
    x_min = another.x_min;
    y_max = another.y_max;
    y_min = another.y_min;
    z_max = another.z_max;
    z_min = another.z_min;
    texture_path = another.texture_path;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, nrComponents;
    unsigned char *data = stbi_load(texture_path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
            
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << texture_path << std::endl;
        stbi_image_free(data);
    }

    GLfloat vertices[] = {
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
        // bottom face
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left      
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
        
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

    glBindVertexArray(0);
}

ParamAsset::ParamAsset(Play* play_instance, const char* texture_path, int reverse_normal, float rotate_speed, 
float speed, glm::vec3 pos_vec, glm::vec3 scale,
float rotated_angle, glm::vec3 ori_front):
BasicAsset(pos_vec, scale, rotated_angle, ori_front, reverse_normal, speed, rotate_speed),
texture_path(texture_path)
{
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, nrComponents;
    unsigned char* data;
    
    if (play_instance->image_data_map.find(texture_path) != play_instance->image_data_map.end())
    {
        data = play_instance->image_data_map[texture_path];
        width = play_instance->image_width_map[texture_path];
        height = play_instance->image_height_map[texture_path];
        nrComponents = play_instance->image_nrcomp_map[texture_path];
    }
    else
    {
        data = stbi_load(texture_path, &width, &height, &nrComponents, 0);
    }
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
            
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << texture_path << std::endl;
        // stbi_image_free(data);
    }

    GLfloat vertices[] = {
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
        // bottom face
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left      
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
        
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

    glBindVertexArray(0);

    vector<vector<vector<float>>> vecs;
    vecs.resize(3);
    vecs[0].push_back({-1.f});
    vecs[0].push_back({1.0f});

    x_min = -1 * scale_vec.x;
    x_max = 1 * scale_vec.x; 
    y_max = 1 * scale_vec.y;
    y_min = -1 * scale_vec.y;
    z_max = 1 * scale_vec.z;
    z_min = -1 * scale_vec.z;
}

void ParamAsset::draw(Shader& shader)
{
    shader.use();
    shader.setInt("reverse_normals", reverse_normals);
    shader.setMat4("model", model_mat());
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ParamAsset::~ParamAsset()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture);
}

ObjAsset::ObjAsset(Play* play_instance, const char * object_path, int reverse_normal, glm::vec3 pos_vec, 
glm::vec3 scale_vec, float speed, float rotate_speed, float rotated_angle, glm::vec3 ori_front):
BasicAsset(pos_vec, scale_vec, rotated_angle, ori_front, reverse_normal, speed, rotate_speed), 
model(object_path), ai(play_instance->frame_instance)
{
    life = 3;
    view_distance = 3.0;
    this->play_instance = play_instance;
    cool_down = 0.0f;
    x_max = -std::numeric_limits<float>::max();
    x_min = std::numeric_limits<float>::max();
    y_max = -std::numeric_limits<float>::max();
    y_min = std::numeric_limits<float>::max();
    z_max = -std::numeric_limits<float>::max();
    z_min = std::numeric_limits<float>::max();

    for (const auto& mesh : model.meshes)
    {
        for (const auto& verti: mesh.vertices)
        {
            auto pos_vec = verti.Position;
            x_max = pos_vec.x > x_max ? pos_vec.x : x_max;
            x_min = pos_vec.x < x_min ? pos_vec.x : x_min;
            y_max = pos_vec.y > y_max ? pos_vec.y : y_max;
            y_min = pos_vec.y < y_min ? pos_vec.y : y_min;
            z_max = pos_vec.z > z_max ? pos_vec.z : z_max;
            z_min = pos_vec.z < z_min ? pos_vec.z : z_min;
        }
    }
    x_max *= scale_vec.x;
    x_min *= scale_vec.x;
    y_max *= scale_vec.y;
    y_min *= scale_vec.y;
    z_max *= scale_vec.z;
    z_min *= scale_vec.z;
}

void ObjAsset::draw(Shader& shader)
{
    shader.use();
    shader.setInt("reverse_normal", reverse_normals);
    shader.setMat4("model", model_mat());
    model.Draw(shader);
}

std::vector<float> BasicAsset::edge_vector() const
{
    return {x_max + pos_vec.x, x_min + pos_vec.x, y_max + pos_vec.y, y_min + pos_vec.y,
    z_max + pos_vec.z, z_min + pos_vec.z};
}

void ObjAsset::control_update(GameLib3D::Framework* frame_instance, 
    GameLib3D::InputKey once_key)
{
    double delta_time = frame_instance->average_delta_time() / 1000;

    cool_down -= delta_time;
    if (cool_down < 0)
    {
        cool_down = 0;
    }
    if (cool_down <= 0.0f && once_key == GameLib3D::SPACE)
    {
        cool_down = 1.5f;
        play_instance->bullets.push_back(ParamAsset(play_instance, "resources/bullet.jpg", 0, 0.0f, 
        3.0f, pos_vec + front_vec() + glm::vec3({0.0f, 0.5f, 0.0f}), {0.05f, 0.05f, 0.05f}, 
        0.0f, front_vec()));
    }
    
    GameLib3D::InputKey continous_key = frame_instance->read_input();
    
    if (continous_key == GameLib3D::UP)
    {
        auto delta_vec = play_instance->tank.front_vec() * glm::vec3(play_instance->tank.speed * delta_time);
        play_instance->tank.pos_vec += delta_vec;
        if (!(play_instance->in_box(&(play_instance->tank))) || play_instance->crash_objects(&(play_instance->tank)))
        {
            play_instance->tank.pos_vec -= delta_vec;
        }
    }
    if (continous_key == GameLib3D::DOWN)
    {
        auto delta_vec = play_instance->tank.front_vec() * glm::vec3(play_instance->tank.speed * delta_time);
        play_instance->tank.pos_vec -= delta_vec;
        if (!(play_instance->in_box(&(play_instance->tank))) || play_instance->crash_objects(&(play_instance->tank)))
        {
            play_instance->tank.pos_vec += delta_vec;
        }
    }

    if (continous_key == GameLib3D::LEFT)
    {
        play_instance->tank.rotated_angle += (play_instance->tank.rotate_speed * delta_time);
        while (play_instance->tank.rotated_angle >= 360.0f)
        {
            play_instance->tank.rotated_angle -= 360.0f;
        }
    }

    if (continous_key == GameLib3D::RIGHT)
    {
        play_instance->tank.rotated_angle -= (play_instance->tank.rotate_speed * delta_time);
        while (play_instance->tank.rotated_angle < 0.0f)
        {
            play_instance->tank.rotated_angle += 360.0f;
        }
    }
}

void ObjAsset::auto_update(GameLib3D::Framework* frame_work)
{
    double delta_time = frame_work->average_delta_time() / 1000;
    cool_down -= delta_time;
    if (could_see(play_instance->tank))
    {
        ai.now_status = EnemyStatus::Search;
        auto diff_vec = play_instance->tank.pos_vec - pos_vec;
        diff_vec = glm::normalize(diff_vec);
        auto front_vec3 = front_vec();
        double front_theta = std::atan2(front_vec3.x, front_vec3.z);
        double diff_theta = std::atan2(diff_vec.x, diff_vec.z);
        const double PI = M_PI;
        if (abs(front_theta - diff_theta) > PI / 15)
        {
            if (front_theta > diff_theta)
            {
                if (front_theta - diff_theta < PI)
                {
                    rotated_angle -= rotate_speed * delta_time;
                }
                else
                {
                    rotated_angle += rotate_speed * delta_time;
                }
            }
            else
            {
                if (diff_theta - front_theta < PI)
                {
                    rotated_angle += rotate_speed * delta_time;
                }
                else
                {
                    rotated_angle -= rotate_speed * delta_time;
                }
            }
        }
        else
        {
            if (cool_down <= 0.0f)
            {
                play_instance->enemy_bullets.emplace_back(ParamAsset(play_instance, 
                    "resources/enemy_bullet.jpg", 0, 0.0f, 3.0f, 
                    pos_vec + front_vec() + glm::vec3({0.0f, 0.5f, 0.0f}), 
                    {0.02f, 0.02f, 0.02f}, 0.0f, front_vec()));
                cool_down = 1.5f;
            }
            auto delta_vec = front_vec() * glm::vec3(delta_time * speed);
            pos_vec += delta_vec;
            if (!play_instance->in_box(this) || play_instance->crash_objects(this))
            {
                pos_vec -= delta_vec;
            }
            
        }
    }
    else
    {
        ai.now_status = EnemyStatus::Random;
    }
    if (ai.now_status == EnemyStatus::Random)
    {
        RandomMovement temp_random_move = ai.next_random_status();
        if (temp_random_move == RandomMovement::MoveForward)
        {
            auto delta_vec = front_vec() * glm::vec3(delta_time * speed);
            pos_vec += delta_vec;
            if (!(play_instance->in_box(this)) || play_instance->crash_objects(this))
            {
                pos_vec -= delta_vec;
            }
        }
        else if (temp_random_move == RandomMovement::Stepback)
        {
            auto delta_vec = front_vec() * glm::vec3(delta_time * speed);
            pos_vec -= delta_vec;
            if (!(play_instance->in_box(this)) || play_instance->crash_objects(this))
            {
                pos_vec += delta_vec;
            }
        }
        else if (temp_random_move == RandomMovement::LeftRotate)
        {
            rotated_angle += (rotate_speed * delta_time);
            while (rotated_angle >= 360.0f)
            {
                rotated_angle -= 360.0f;
            }
        }
        else if (temp_random_move == RandomMovement::RightRotate)
        {
            rotated_angle -= (rotate_speed * delta_time);
            while (rotated_angle >= 360.0f)
            {
                rotated_angle += 360.0f;
            }
        }
    }
}

bool ObjAsset::could_see(BasicAsset& asset)
{
    double asset_distance = glm::distance(asset.pos_vec, pos_vec);
    if (asset_distance < view_distance && glm::dot(pos_vec, asset.pos_vec) >= 0.0)
    {
        return true;
    }
    else
    {
        return false;
    }
}