#include "Scene.h"
#include "GEARStudioEvents.h"

#include <G2/RenderComponent.h>
#include <G2/CameraComponent.h>
#include <G2/TransformComponent.h>
#include <G2/LightComponent.h>

#include <boost/algorithm/string/predicate.hpp>
#include <algorithm>

Scene::Scene(std::string projectDirectory, std::string const& sceneFile) :
	JsonDeserializer(sceneFile),
	JsonSerializer(sceneFile),
	mProjectDirectory(projectDirectory)
{
}

ManagedEntity*
Scene::createNewEntity(std::string const& name)
{
	if(get(name) == nullptr)
	{
		mLoadedEntities[name] = std::move(ManagedEntity(name));
		auto* entity = &mLoadedEntities[name];
		GEARStudioEvents::onManagedEntityCreated(this, entity);
		return entity;
	}
	return nullptr;
}

ManagedEntity*
Scene::get(std::string const& name)
{
	if (mLoadedEntities.count(name) == 1)
	{
		return &mLoadedEntities[name];
	}
	return nullptr;
}


void
Scene::_init3D()
{
	// load default effect
	auto* renderSystem = G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem, G2::RenderComponent>();

	std::shared_ptr<G2::Effect> effect = mEffectImporter.import(mProjectDirectory + "/" + mResource["rendersystem"]["default_effect"].asString());
	if (effect.get() != nullptr)
	{
		renderSystem->setDefaultEffect(effect);
	}
	renderSystem->setClearColor(
		glm::vec4(
			mResource["rendersystem"]["default_color"]["x"].asFloat(),
			mResource["rendersystem"]["default_color"]["y"].asFloat(),
			mResource["rendersystem"]["default_color"]["z"].asFloat(),
			mResource["rendersystem"]["default_color"]["a"].asFloat()
		)
	);
	renderSystem->reserveSize(mResource["rendersystem"]["size_hint"].asInt());

	Json::Value const& entities = mResource["entities"];

	for (int i = 0; i < entities.size(); ++i)  // Iterates over the sequence elements.
	{
		_initEntityFromJson(entities[i]);
	}
}

void
Scene::_initEntityFromJson(Json::Value const& entityDesc)
{
	if (entityDesc.isMember("name"))
	{
		mLoadedEntities[entityDesc["name"].asString()] = std::move(ManagedEntity(entityDesc));
		auto* entity = &mLoadedEntities[entityDesc["name"].asString()];
		GEARStudioEvents::onManagedEntityCreated(this, entity);
		// whoever wants to add something to the ManagedEntity will have to register to this event.
		GEARStudioEvents::onDeserializeManagedEntity(entity, entity->getEntityDescription());
	}
	else
	{
		// TODO log warning!
	}
}

//void
//Scene::_initComponentFromJson(Json::Value const& componentDesc, ManagedEntity* entity)
//{
//	if (componentDesc.isMember("type"))
//	{
//		std::string type = componentDesc["type"].asString();
//		if ("G2::RenderComponent" == type)
//		{
//			auto* comp = entity->addComponent<G2::RenderComponent>();
//
//			std::string meshPath = componentDesc["mesh"].asString();
//			std::transform(meshPath.begin(), meshPath.end(), meshPath.begin(), ::tolower);
//			if (boost::algorithm::ends_with(meshPath, "fbx"))
//			{
//				// load with FBX importer and let him attach the imported mesh to the entity pointer
//				mFbxImporter.import(mProjectDirectory + "/" + componentDesc["mesh"].asString(), true, true, true, false, false, nullptr, entity);
//			}
//		}
//		else if ("G2::LightComponent" == type)
//		{
//			if (componentDesc["light"]["type"].asString() == "DIRECTIONAL")
//			{
//				auto* comp = entity->addComponent<G2::LightComponent>(G2::LightType::DIRECTIONAL);
//
//				comp->diffuse = glm::vec4(
//					componentDesc["light"]["diffuse"]["x"].asFloat(),
//					componentDesc["light"]["diffuse"]["y"].asFloat(),
//					componentDesc["light"]["diffuse"]["z"].asFloat(),
//					componentDesc["light"]["diffuse"]["a"].asFloat()
//					);
//				comp->attenuation = componentDesc["light"]["attenuation"]["constant"].asFloat();
//				comp->linearAttenuation = componentDesc["light"]["attenuation"]["linear"].asFloat();
//				comp->exponentialAttenuation = componentDesc["light"]["attenuation"]["exponential"].asFloat();
//			}
//
//		}
//		else if ("G2::TransformComponent" == type)
//		{
//			auto* comp = entity->addComponent<G2::TransformComponent>();
//
//			if (componentDesc.isMember("rotation"))
//			{
//				comp->rotateAxis(componentDesc["rotation"]["angle"].asFloat(), glm::vec3(
//					componentDesc["rotation"]["axis"]["x"].asFloat(),
//					componentDesc["rotation"]["axis"]["y"].asFloat(),
//					componentDesc["rotation"]["axis"]["z"].asFloat()
//				));
//			}
//		}
//	}
//}

void
Scene::load()
{
	if (!error())
	{
		// init every object
		_init3D();
	}
}

void
Scene::save()
{
	if (!error())
	{
		// save all ManagedEntities of the scene into a new Json
		Json::Value entities(Json::arrayValue);

		for (auto it = mLoadedEntities.begin(); it != mLoadedEntities.end(); ++it)
		{
			// every ManagedEntity has it's entire description as Json for fast access.
			// we always assume that this description is up to date!
			entities.append(it->second.getEntityDescription());
		}
		mResource["entities"] = entities;

		serialize(mResource);
	}
}

std::string
Scene::getName() const
{
	return mResource["name"].asString();
}