#pragma once
#include <G2/GEAR.h>

static std::string ASSET_PATH = "../../Assets/";

class Planet
{
	public:

		Planet(
			std::string const& name,
			std::shared_ptr<G2::FBXMesh> planetMesh,
			glm::vec3 const& position,
			glm::vec3 const& scale,
			glm::vec3 const& rotationAxis,
			double period,
			double axisRotationSpeed,
			std::shared_ptr<G2::Texture> diffuseTex,
			std::shared_ptr<G2::Effect> effect
		);

		std::shared_ptr<G2::FBXMesh>& getPlanetMesh() { return mPlanetMesh; }

		~Planet();
	private:

		void _onRenderFrame(G2::FrameInfo const&);

		void _onKeyDown(G2::KeyCode keyCode);

		std::string						mName;					// The name of the planet
		G2::Entity						mAnchor;				// The anchor of the planet in the solar system
		std::shared_ptr<G2::FBXMesh>	mPlanetMesh;			// The mesh entity of the Planet
		glm::vec3						mRotationAxis;			// The rotation axis of the planet
		double							mAnchorRotationSpeed;	// The rotation speed around the anchor
		double							mAxisRotationSpeed;		// The rotation speed around the axis
		double							mSpeed;
};

