#pragma once
#include "PointLight.h"


class SpotLight :
    public PointLight
{
public:
    SpotLight();

    SpotLight(GLfloat red, GLfloat green, GLfloat blue,
        GLfloat a_intensity, GLfloat d_intensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos,
        GLfloat xDir, GLfloat yDir, GLfloat zDir,
        GLfloat con, GLfloat lin, GLfloat exp,
        GLfloat edgyy);

    void use_light(GLuint ambient_intensity_location, GLuint ambient_colour_location,
        GLuint diffuse_intensity_location, GLuint position_location, GLuint direction_location,
        GLuint constant_location, GLuint linear_location, GLuint exponent_location,
        GLuint edge_location);

    void set_flash(glm::vec3 pos, glm::vec3 dir);

    ~SpotLight();

private:
    glm::vec3 direction;

    GLfloat edge, processedEdge;
};

