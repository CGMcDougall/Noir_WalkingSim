#include "branch.h"

namespace game {

    Branch::Branch(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture) : SceneNode(name, geometry, material, texture) {
        depth_ = 0;
        wind_ = glm::vec3(0, 0, 1);
        timer_ = 1;
    }

    Branch::~Branch() {}

    void Branch::Update(void) {
        if (depth_ > 0) {
            // Each branch pulls its parent's up vector and applies the rotation and orbit transformations to find its own up vector
            // Because a parent will always update before a child, this works.
            glm::quat up = glm::normalize((glm::quat(0, GetParent()->GetUp()) * GetOrbitRotation())); 
            glm::vec3 rotation_angle = glm::normalize(glm::cross(GetWind(), glm::vec3(up.x, up.y, up.z)));
            
            // Each section is the same as the last, except I made the swaying less rigid by adding more steps to the animation.
            // Also, each "depth" of the tree (height, really) sways a bit more than the previous (lighter branches so they should move more). I don't get bonus for that, do I?
            if (0 <= timer_ && timer_ <= 4 || 15 <= timer_ && timer_ <= 19) {
                glm::quat ang_m = glm::normalize(glm::angleAxis(2 * glm::pi<float>() / (10000/depth_), rotation_angle));
                Orbit(glm::vec3(GetScale().x * -BRANCH_LENGTH / 2.5, 0, 0), ang_m);
                up = glm::normalize(GetOrbitRotation() * up); // The up vector gets calculated, but we add another orbit so it needs to be updated again
                timer_ += 1;

            } 
            else if (5 <= timer_ && timer_ <= 14) {
                glm::quat ang_m = glm::normalize(glm::angleAxis(2 * glm::pi<float>() / (4000 / depth_), rotation_angle));
                Orbit(glm::vec3(GetScale().x * -BRANCH_LENGTH / 2.5, 0, 0), ang_m);
                up = glm::normalize(GetOrbitRotation() * up);
                timer_ += 1;

            }
            else if (20 <= timer_ && timer_ <= 24 || 35 <= timer_ && timer_<= 40) {
                glm::quat ang_m = glm::normalize(glm::angleAxis(-2 * glm::pi<float>() / (10000 / depth_), rotation_angle)); // The angle is negative here to add opposite sway
                Orbit(glm::vec3(GetScale().x * -BRANCH_LENGTH / 2.5, 0, 0), ang_m);
                up = glm::normalize(GetOrbitRotation() * up);
                timer_ += 1;
            } 
            else if (25 <= timer_ && timer_ <= 34) {
                glm::quat ang_m = glm::normalize(glm::angleAxis(-2 * glm::pi<float>() / (4000 / depth_), rotation_angle)); // Here as well ^
                Orbit(glm::vec3(GetScale().x * -BRANCH_LENGTH / 2.5, 0, 0), ang_m);
                up = glm::normalize(GetOrbitRotation() * up);
                timer_ += 1;
            }
        

            if (timer_ >= 41) {
                timer_ = 0;
            }
            
            SetUp(glm::vec3(up.x, up.y, up.z)); // We set this node's up vector so its children can use it
            
        }
    }

} // namespace game
