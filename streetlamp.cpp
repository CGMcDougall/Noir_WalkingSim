#include "streetlamp.h"

namespace game {

	Streetlamp::Streetlamp(const std::string name, const Resource* geometry, const Resource* material) : SceneNode(name, geometry, material) {
		lightDirection = glm::vec3(0, -1, 0);
		cutOffRadius = glm::cos(glm::radians(12.5f));
	}

	Streetlamp::~Streetlamp() {}

}