#pragma once
#include "Light.h"



class PointLight :
    public Light
{
public:
    PointLight();

    PointLight(GLfloat red, GLfloat green, GLfloat blue, 
               GLfloat a_intensity, GLfloat d_intensity,
               GLfloat xPos, GLfloat yPos, GLfloat zPos,
               GLfloat con, GLfloat lin, GLfloat exp);

    void use_light(GLfloat ambient_intensity_location, GLfloat ambient_colour_location,
                   GLfloat diffuse_intensity_location, GLfloat position_location,
                   GLfloat constant_location, GLfloat linear_location, GLfloat exponent_location);

    ~PointLight();

private:
    glm::vec3 position;
    GLfloat constant, linear, exponent;
};

