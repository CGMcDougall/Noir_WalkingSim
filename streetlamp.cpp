#include "streetlamp.h"

namespace game {

	Streetlamp::Streetlamp(const std::string name, const Resource* geometry, const Resource* material) : SceneNode(name, geometry, material) {
	}

	Streetlamp::~Streetlamp() {}

}