#ifndef SCENE_NODE_H_
#define SCENE_NODE_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "resource.h"
#include "camera.h"
#include <vector>
#include <sstream>


namespace game {

    // Class that manages one object in a scene 
    class SceneNode {

        public:
            // Create scene node from given resources
            SceneNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL);

            // Destructor
            ~SceneNode();
            
            // Get name of node
            const std::string GetName(void) const;

            // Get node attributes
            glm::vec3 GetPosition(void) const;
            glm::quat GetOrientation(void) const;
            glm::vec3 GetScale(void) const;
            glm::mat4 GetMatrix(void) const;
            glm::vec3 GetUp(void) const;
            glm::quat GetOrbitRotation(void) const;

            // Set node attributes
            void SetPosition(glm::vec3 position);
            void SetOrientation(glm::quat orientation);
            void SetScale(glm::vec3 scale);
            void SetUp(glm::vec3 up);
            
            // Children/parent actions
            void AddChild(SceneNode*);
            void SetParent(SceneNode*);
            SceneNode* GetParent();

            void SetJoint(glm::vec3 j) { joint_ = j; }
            void SetOrbit(glm::quat o) { orbit_ = o; }
            
            // Perform transformations on node
            void Translate(glm::vec3 trans);
            void Rotate(glm::quat rot);
            void Scale(glm::vec3 scale);
            void Orbit(glm::vec3 pos, glm::quat orb);

            // Draw the node according to scene parameters in 'camera'
            // variable
            virtual void Draw(Camera *camera);

            // Update the node
            virtual void Update(void);

            // OpenGL variables
            GLenum GetMode(void) const;
            GLuint GetArrayBuffer(void) const;
            GLuint GetElementArrayBuffer(void) const;
            GLsizei GetSize(void) const;
            GLuint GetMaterial(void) const;

            void setLightSources(std::vector<glm::vec3> li);
            void setDirectionalLightSources(std::vector<glm::vec3> li);


        protected:
            std::string name_; // Name of the scene node
            GLuint array_buffer_; // References to geometry: vertex and array buffers
            GLuint element_array_buffer_;
            GLenum mode_; // Type of geometry
            GLsizei size_; // Number of primitives in geometry
            GLuint material_; // Reference to shader program
            GLuint texture_; // Reference to texture resource
            glm::vec3 position_; // Position of node
            glm::quat orientation_; // Orientation of node
            glm::vec3 scale_; // Scale of node
            glm::vec3 orbit_position_; // Position of the node's orbit anchor
            glm::quat orbit_rotation_; // Rotation quaternion of the node's orbit
            glm::mat4 transf_; // Holds the final rotation information for this node before it updates that of its children.
            glm::vec3 up_; // The up vector of the node

            SceneNode* parent_; // Parent node
            std::vector<SceneNode*> children_;  // Child node(s)

            glm::quat orbit_; //orbit
            glm::vec3 joint_ = glm::vec3(0, 0, 0); //joint, assumed to be 0,0,0

            // Set matrices that transform the node in a shader program
            void SetupShader(GLuint program);

            //list of lightSources
            std::vector<glm::vec3> lightSource;

            // List of focal light source positions
            std::vector<glm::vec3> lampLightPos;

    }; // class SceneNode

} // namespace game

#endif // SCENE_NODE_H_
