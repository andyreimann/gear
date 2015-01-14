#include "Scene.h"
#include "GEARStudioEvents.h"

#include <G2/RenderComponent.h>
#include <G2/CameraComponent.h>
#include <G2/TransformComponent.h>
#include <G2/LightComponent.h>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <algorithm>
#include <QProgressDialog>

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
		mLoadedEntitiesIdToNameMapping[entity->getId()] = name;
		G2S::onManagedEntityCreated(this, entity);
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

ManagedEntity*
Scene::get(unsigned int entityId)
{
	if (mLoadedEntitiesIdToNameMapping.count(entityId) == 1)
	{
		return get(mLoadedEntitiesIdToNameMapping[entityId]);
	}
	return nullptr;
}


void
Scene::_init3D(QProgressDialog* progress)
{
	// load default effect
	auto* renderSystem = G2::ECSManager::getShared()
		.getSystem<G2::RenderSystem, G2::RenderComponent>();

	std::shared_ptr<G2::Effect> effect = mEffectImporter.import("shader/Entity.g2fx");
	if (effect.get() != nullptr)
	{
		renderSystem->setDefaultEffect(effect);
	}
	/* we dont adjust the background of the editor since this color is ment to be used when the game runs and not the editor!
	renderSystem->setClearColor(
		glm::vec4(
			mResource["rendersystem"]["default_color"]["x"].asFloat(),
			mResource["rendersystem"]["default_color"]["y"].asFloat(),
			mResource["rendersystem"]["default_color"]["z"].asFloat(),
			mResource["rendersystem"]["default_color"]["a"].asFloat()
		)
	);
	*/
	renderSystem->reserveSize(mResource["rendersystem"]["size_hint"].asInt());

	Json::Value const& entities = mResource["entities"];

	progress->setMaximum(progress->maximum() + entities.size());
	for (unsigned int i = 0; i < entities.size(); ++i)  // Iterates over the sequence elements.
	{
		progress->setValue(progress->value() + i);
		_initEntityFromJson(entities[i]);
	}
}

void
Scene::_initEntityFromJson(Json::Value const& entityDesc)
{
	if (entityDesc.isMember("name"))
	{
		std::string name = entityDesc["name"].asString();
		mLoadedEntities[name] = std::move(ManagedEntity(entityDesc));
		auto* entity = &mLoadedEntities[name];
		mLoadedEntitiesIdToNameMapping[entity->getId()] = name;
		G2S::onManagedEntityCreated(this, entity);
		// whoever wants to add something to the ManagedEntity will have to register to this event.
		G2S::onDeserializeManagedEntity(entity, entity->getEntityDescription());
	}
	else
	{
		// TODO log warning!
	}
}

void
Scene::load(QProgressDialog* progress)
{
	if (!error())
	{
		// init every object
		_init3D(progress);
		std::stringstream log;
		log << "Scene " << mResource["name"].asString() << " loaded";
		G2S::onLog(INFO, log.str());
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

		std::stringstream log;
		log << "Scene " << mResource["name"].asString() << " saved";
		G2S::onLog(INFO, log.str());
	}
}

std::string Scene::getName() const
{
	return mResource["name"].asString();
}

void Scene::generateEntityInitializationCode(std::ostream& out, QProgressDialog* progress) const
{
	progress->setMaximum(progress->maximum() + mLoadedEntities.size());
	std::string indention = "		";
	std::stringstream name;
	for (auto it = mLoadedEntities.begin(); it != mLoadedEntities.end(); ++it)
	{
		progress->setValue(progress->value() + 1);
		std::string entityVar = "e" + std::to_string(it->second.getId());

		std::string name = it->first;
		boost::replace_all(name, "\\", "\\\\");
		boost::replace_all(name, "\"", "\\\"");
		
		out << indention << "// START '" << it->first << "'" << std::endl;
		out << indention << "mManagedEntities[\"" << name << "\"] = std::move(G2::Entity());" << std::endl; // create entity instance
		out << indention << "auto& " << entityVar << " = mManagedEntities[\"" << name << "\"];" << std::endl; // create accessor
		G2S::onGenerateCppCodeForManagedEntity(&it->second, entityVar, out);
		out << indention << "// END '" << it->first << "'" << std::endl;
	}
}