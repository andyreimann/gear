#pragma once

#include <G2Core/Entity.h>


/** This class defines an Entity in the domain of the GEAR Studio.
* @created	2014/12/17
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class ManagedEntity : public G2::Entity
{
	public:
		ManagedEntity() {}
		ManagedEntity(std::string const& name) : mName(name) {}

		ManagedEntity(ManagedEntity && rhs)
		{
			*this = std::move(rhs); // rvalue property is kept with std::move!
		}

		ManagedEntity& operator=(ManagedEntity && rhs)
		{
			// do assignment here
			mName = rhs.mName;
			rhs.mName = "";

			return static_cast<ManagedEntity&>(G2::Entity::operator=(std::move(rhs)));
		}
		ManagedEntity& operator=(G2::Entity && rhs)
		{
			mName = "";
			return static_cast<ManagedEntity&>(G2::Entity::operator=(std::move(rhs)));
		}

		/** This function will return the Name.
		* @return The Name.
		*/
		std::string const& getName() const { return mName; }

	private:
		std::string mName;
};
