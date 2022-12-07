#include "streetlamp.h"

namespace game {

	Streetlamp::Streetlamp(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture) : SceneNode(name, geometry, material, texture) {
		lightDirection = glm::vec3(0, -1, 0);
		cutOffRadius = glm::cos(glm::radians(12.5f));
	}

	Streetlamp::~Streetlamp() {}

}