#include "Cig.h"


namespace game {

	Cigarette::Cigarette(const std::string name, const Resource* geometry, const Resource* material, const Resource *texture) : SceneNode(name, geometry, material,texture) {
		smoke = NULL;
	}

	Cigarette::~Cigarette() {

	}
		
	void Cigarette::giveSmokeParticle(SceneNode *s) {
		smoke = s;
		glm::vec3 pos = glm::vec3(GetPosition().x, GetPosition().y, GetPosition().z-0.03);
		smoke->SetPosition(pos);
		smoke->SetJoint(pos);
		smoke->SetScale(glm::vec3(1,1,1)*0.1f);
		//smoke->Orbit(pos, GetOrbitRotation());
	}

	void Cigarette::Update() {
	}

	void Cigarette::Draw(Camera* cam) {
		SceneNode::Draw(cam);
		if(smoke != NULL)smoke->Draw(cam);
	}

	void Cigarette::SetPosition(glm::vec3 p) {
		position_ = p;
	}


	void Cigarette::SetJoint(glm::vec3 j) {
		joint_ = j;
	}





}