#ifndef STREETLAMP_H_
#define STREETLAMP_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "resource.h"
#include "scene_node.h"

namespace game {

    // Abstraction of a streetlamp
    class Streetlamp : public SceneNode {

    public:
        // Create streetlamp from given resources
        Streetlamp(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture);

        // Destructor
        ~Streetlamp();

        // Get/set attributes specific to streetlamps
        glm::vec3 getLightPos() { return lightPos; };
        void setLightPos(glm::vec3 lightP) { lightPos = lightP; };
        glm::vec3 getlightDirection() { return lightDirection; };
        void setLightDirection(glm::vec3 dir) { lightDirection = dir; };
        float getCutOff() { return cutOffRadius; };
        void setCutoff(float cutOff) { cutOffRadius = cutOff; };

    private:
        glm::vec3 lightPos;
        glm::vec3 lightDirection;
        float cutOffRadius;
        
    }; // class Streetlamp

} // namespace game

#endif
