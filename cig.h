#ifndef CIG_H_
#define CIG_H_

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


	class Cigarette : public SceneNode {

	public:
		
		Cigarette(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture = NULL);
		~Cigarette();

		void giveSmokeParticle(SceneNode* s);
		
		void SetPosition(glm::vec3 p);
		void SetJoint(glm::vec3 j);
		
		void Draw(Camera *cam);
		void Update();

		

	private:

		SceneNode *smoke;
		glm::vec3 smokePos;
		glm::mat4 orbit;


	};

}

#endif