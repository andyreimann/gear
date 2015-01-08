#pragma once

#include <G2Core/Entity.h>

#include <QtGui/qimage.h>

#include <map>

/** This class defines an Entity in the domain of the GEAR Studio.
* @created	2014/12/17
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class ManagedEntity : public G2::Entity
{
	public:
		ManagedEntity() {}
		ManagedEntity(std::string const& name, Json::Value const& description = Json::Value())
		{
			mEntityDesc["name"] = name;
		}
		ManagedEntity(Json::Value const& description)
			: mEntityDesc(description) {}

		ManagedEntity(ManagedEntity && rhs)
		{
			*this = std::move(rhs); // rvalue property is kept with std::move!
		}

		ManagedEntity& operator=(ManagedEntity && rhs)
		{
			// do assignment here
			mEntityDesc = std::move(rhs.mEntityDesc);
			rhs.mEntityDesc = Json::Value();
			return static_cast<ManagedEntity&>(G2::Entity::operator=(std::move(rhs)));
		}
		ManagedEntity& operator=(G2::Entity && rhs)
		{
			mEntityDesc = Json::Value();
			return static_cast<ManagedEntity&>(G2::Entity::operator=(std::move(rhs)));
		}

		/** This function will return the Name.
		* @return The Name.
		*/
		std::string getName() const { return mEntityDesc["name"].asString(); }
		/** This function will return a reference to the properties object registered on the given technicalName.
		 * @param technicalName The technical name to get the properties for.
		 * @return A reference to the properties object registered on the given technicalName.
		 * @note If the given properties entry does not exist, a new empty one will be created!
		 */
		Json::Value& getProperties(std::string technicalName) { return mEntityDesc["properties"][technicalName]; }
		/** This function will check if there is a properties entry set on the given technicalName.
		* @param technicalName The technical name to get the properties for.
		* @return True if the given properties entry is set, false if not.
		*/
		bool hasProperties(std::string technicalName) { return mEntityDesc["properties"].isMember(technicalName); }
		/** This function will remove the 
		* @param technicalName The technical name to get the properties for.
		* @return A reference to the properties object registered on the given technicalName.
		* @note If the given properties entry does not exist, a new empty one will be created!
		*/
		void removeProperties(std::string technicalName) 
		{ 
			Json::Value removed = mEntityDesc["properties"].removeMember(technicalName);
			if (!removed.isNull())
			{
				// the property was present

			}
		}

		/** Returns the entire entity description.
		 */
		Json::Value const& getEntityDescription() const { return mEntityDesc; }


		std::map<std::string, QImage>	imageCache;

	private:

		Json::Value mEntityDesc;										// The name of the ManagedEntity
};
