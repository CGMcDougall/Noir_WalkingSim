#ifndef BRANCH_H_
#define BRANCH_H_

#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "resource.h"
#include "scene_node.h"

#define BRANCH_LENGTH 8

namespace game {

    // Abstraction of a branch
    class Branch : public SceneNode {

    public:
        // Create branch from given resources
        Branch(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture);

        // Destructor
        ~Branch();

        void SetDepth(int depth) { depth_ = depth; };
        int GetDepth() { return depth_; }
        glm::vec3 GetWind() { return wind_; }

        // Update geometry configuration
        void Update(void);

    private:
        int depth_; // Holds the recursion depth of the branches
        glm::vec3 wind_; // Holds the direction of the wind
        int timer_; // Holds the number of updates done. Used to regulate swaying animation
        
    }; // class Branch

} // namespace game



#endif
