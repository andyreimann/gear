// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "BaseComponent.h"
#include "TransformSystem.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace G2 
{
	/** A TransformComponent describes the transformation in 3D-Space.
	 * This transformation is split up in a rotation, translation and a scaling.
	 * It combines the transformation into a final local and world space matrix
	 * calculated with LS = WS = T * R * S.
	 * A TransformComponent can also have a parent TransformComponent, in which
	 * case the calculation of the world space matrix is changed to
	 * WS = PWS * LS.
	 * @created:	2014/01/30
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class TransformComponent : public BaseComponent<TransformSystem> 
	{
		
		public:
			/** Constructs a new TransformComponent.
			 */
			TransformComponent();
			/** Move constructor to move a TransformComponent.
			 */
			TransformComponent(TransformComponent && rhs);
			/** Move assignment operator to move a TransformComponent.
			 */
			TransformComponent& operator=(TransformComponent && rhs);
			/**
			 * This function will translate the position of the TransformComponent.
			 * @param translation The translation to add.
			 */
			TransformComponent* translate(glm::vec3 const& translation);
			/**
			 * This function will set the position of the TransformComponent.
			 * @param position The new position.
			 */
			TransformComponent* setPosition(glm::vec3 const& position);
			/**
			 * This function will return the position of the TransformComponent.
			 * @return The position of the TransformComponent.
			 */
			glm::vec3 const& getPosition() const;
			/**
			 * This function will rotate the TransformComponent.
			 * @param rotation The rotation to add.
			 */
			TransformComponent* rotate(glm::quat const& rotation);
			/**
			 * This function will rotate the TransformComponent.
			 * @param rotation The rotation to add.
			 */
			TransformComponent* rotate(glm::mat3 const& rotation);
			/**
			 * This function will rotate the TransformComponent.
			 * @param rotation The rotation to add.
			 */
			TransformComponent* rotate(glm::mat4 const& rotation);
			/**
			 * This function will rotate the TransformComponent around the X-Axis.
			 * @param degrees The degrees to rotate the TransformComponent.
			 */
			TransformComponent* rotateX(float degrees);
			/**
			 * This function will rotate the TransformComponent around the Y-Axis.
			 * @param degrees The degrees to rotate the TransformComponent.
			 */
			TransformComponent* rotateY(float degrees);
			/**
			 * This function will rotate the TransformComponent around the Z-Axis.
			 * @param degrees The degrees to rotate the TransformComponent.
			 */
			TransformComponent* rotateZ(float degrees);
			/**
			 * This function will rotate the TransformComponent around the given axis.
			 * @param degrees The degrees to rotate the TransformComponent.
			 * @param axis The axis to rotate around.
			 */
			TransformComponent* rotateAxis(float degrees, glm::vec3 const& axis);
			/**
			 * This function will rotate the TransformComponent with the euler angles (yaw * pitch * roll). 
			 * @param yaw The yaw degrees to apply.
			 * @param pitch The pitch degrees to apply.
			 * @param roll The roll degrees to apply.
			 */
			TransformComponent* rotateEulerAnglesYXZ(float yaw, float pitch, float roll);
			/**
			 * This function will set the rotation of the TransformComponent.
			 * @param rotation The rotation to apply.
			 */
			TransformComponent* setRotation(glm::quat const& rotation);
			/**
			 * This function will set the rotation of the TransformComponent.
			 * @param rotation The rotation to apply.
			 */
			TransformComponent* setRotation(glm::mat3 const& rotation);
			/**
			 * This function will set the rotation of the TransformComponent.
			 * @param rotation The rotation to apply.
			 */
			TransformComponent* setRotation(glm::mat4 const& rotation);
			/**
			 * This function will return the rotation of the TransformComponent.
			 * @return The rotation of the TransformComponent.
			 */
			glm::quat const& getRotation() const;
			/**
			 * This function will set the scale of the TransformComponent.
			 * @param scale The scale to apply.
			 */
			TransformComponent* setScale(glm::vec3 const& scale);
			/**
			 * This function will return the scale of the TransformComponent.
			 * @return The scale of the TransformComponent.
			 */
			glm::vec3 const& getScale() const;
			/** This function will return the composed local space matrix.
			 * @note The returned matrix is a cached version. To get an updated
			 * version, call updateLocalSpaceMatrix() before calling this function!
			 * @return The local space matrix of this TransformComponent.
			 */
			glm::mat4 const& getLocalSpaceMatrix() const;
			/** This function will return the composed worls space matrix.
			 * @note The returned matrix is a cached version. To get an updated
			 * version, call updateLocalSpaceMatrix() before calling this function!
			 * @return The world space matrix of this TransformComponent (parent world space matrices are composed).
			 */
			glm::mat4 const& getWorldSpaceMatrix() const;
			/** This function will update the world and local space matrix of the TransformComponent.
			 * Normally the world space matrices of all TransformComponent objects are updated
			 * each frame from the TransformSystem in the 'postUpdate' phase.
			 * The calculation is cached, which means it is internally only updated
			 */
			void updateWorldSpaceMatrix();
			/** This function will connect this TransformComponent to the given one.
			 * This will change the calculation of the world space matrix for the 
			 * TransformComponent to WS = PLS * LS;
			 * @param parent The parent TransformComponent.
			 * @return The TransformComponent itself.
			 * @note This function will perform a circular dependency check on the parent
			 * to prevent endless loops.
			 */
			TransformComponent* setParent(TransformComponent* parent);
			/** This function will return the Entity-ID of the parent TransformComponent 
			 * if some is attached.
			 * @return The Entity-ID of the parent TransformComponent 
			 * if some is attached, 0 otherwise.
			 */
			unsigned int getParentEntityId() const;
		private:

			TransformComponent(TransformComponent const& rhs) {}
			TransformComponent& operator=(TransformComponent const& rhs) { return *this; }

			/** This function will check the TransformComponents and all parents
			 * dirty flags.
			 */
			bool _isDirty() const;
			void _setDirty();
			bool _hasCircularDependency(unsigned int initialParentEntityId) const;

			unsigned int	mParentEntityId;	// The Entity id of the parent TransformComponent
			bool			mIsDirty;			// Indicates whether something has changed since last recalculation of the local space matrix
			glm::vec3		mPosition;			// The position
			glm::vec3		mScale;				// The scale vector
			glm::quat		mRotation;			// The rotation quaternion
			glm::mat4		mLocalSpace;		// The composed local space matrix
			glm::mat4		mWorldSpace;		// The composed world space matrix
			std::vector<unsigned int> mChildEntityIds; // The Entity-IDs of the TransformComponents, on which this TC is the parent
	};
};