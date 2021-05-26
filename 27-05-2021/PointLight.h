#pragma once
#include "Light.h"

#include "OmniShadowMap.h"

#include <vector>

class PointLight :
    public Light
{
public:
    PointLight();

    PointLight(GLuint shadowWidth, GLuint shadowHeight,
                GLfloat near, GLfloat far,
                GLfloat red, GLfloat green, GLfloat blue, 
               GLfloat a_intensity, GLfloat d_intensity,
               GLfloat xPos, GLfloat yPos, GLfloat zPos,
               GLfloat con, GLfloat lin, GLfloat exp);

    void use_light(GLuint ambient_intensity_location, GLuint ambient_colour_location,
        GLuint diffuse_intensity_location, GLuint position_location,
        GLuint constant_location, GLuint linear_location, GLuint exponent_location);

    std::vector<glm::mat4> calculateLightTransform();

    glm::vec3 getPosition();

    GLfloat getFarPlane();

    ~PointLight();

protected:
    glm::vec3 position;
    GLfloat constant, linear, exponent;

    GLfloat farPlane;
};

