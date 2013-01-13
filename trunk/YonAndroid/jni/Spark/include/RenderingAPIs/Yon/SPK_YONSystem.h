//////////////////////////////////////////////////////////////////////////////////
// SPARK Yon Rendering library												//
// Copyright (C) 2009															//
// Thibault Lescoat -  info-tibo <at> orange <dot> fr							//
// Julien Fryer - julienfryer@gmail.com											//
//																				//
// This software is provided 'as-is', without any express or implied			//
// warranty.  In no event will the authors be held liable for any damages		//
// arising from the use of this software.										//
//																				//
// Permission is granted to anyone to use this software for any purpose,		//
// including commercial applications, and to alter it and redistribute it		//
// freely, subject to the following restrictions:								//
//																				//
// 1. The origin of this software must not be misrepresented; you must not		//
//    claim that you wrote the original software. If you use this software		//
//    in a product, an acknowledgment in the product documentation would be		//
//    appreciated but is not required.											//
// 2. Altered source versions must be plainly marked as such, and must not be	//
//    misrepresented as being the original software.							//
// 3. This notice may not be removed or altered from any source distribution.	//
//////////////////////////////////////////////////////////////////////////////////

#ifndef H_SPK_YONSYSTEM
#define H_SPK_YONSYSTEM

#include "RenderingAPIs/Yon/SPK_YON_DEF.h"
#include "Core/SPK_System.h"

namespace SPK
{
namespace YON
{
	/**
	* @brief A particle system adapted to Yon
	*
	* This particle system is also a scene node, so you can apply transformations on it.<br>
	* Be aware that only group using Yon as renderer will work correctly.<br>
	* <br>
	* The particle system is rendered automatically in the render process of Yon.<br>
    * If specified it can automatically update all particles (auto-update) on animation pass.<br>
    * It is possible to specify if the system should update particles when not visible.<br>
    * By default, auto-update is enabled only when visible.<br>
	* <br>
	* Note also that an YONSystem takes care of the camera position automatically when distance computation is enabled
	* on one of its Group. Therefore there is no need to call System::setCameraPosition(Vector3D).
	*
	* @since 1.04.00
	*/
    class SPK_YON_PREFIX YONSystem : public System, public yon::scene::ISceneNode
    {
		SPK_IMPLEMENT_REGISTERABLE(YONSystem)

	public:

		//////////////////
		// Constructors //
		//////////////////

		/**
		* @brief Constructor of YONSystem
		* @param parent : the parent node of the particle system
		* @param mgr : the Yon scene manager
		* @param worldTransformed : true to emit particles in world, false to emit them localy
		* @param id : the ID of the node
		*/
        YONSystem(yon::scene::ISceneNode* parent,yon::scene::ISceneManager* mgr,bool worldTransformed = true,yon::s32 id=-1);

		YONSystem(const YONSystem& system);

		/**
		* @brief Creates and registers a new YONSystem
		* @param parent : the parent node of the particle system
		* @param mgr : the Yon scene manager
		* @param worldTransformed : true to emit particles in world, false to emit them localy
		* @param id : the ID of the node
		* @return A new registered YONSystem
		*/
		static YONSystem* create(yon::scene::ISceneNode* parent,yon::scene::ISceneManager* mgr,bool worldTransformed = true,yon::s32 id=-1);

		/////////////
		// Setters //
		/////////////

		/**
		* @brief Enables or disables auto update
		* @param enableState : True to enable auto-update, false to disable it
        * @param onlyWhenVisible : True to perform auto-update only if node is visible. This parameter is ignored if auto-update is set to false.
		*/
        void setAutoUpdateEnabled(bool enableState, bool onlyWhenVisible);

		/////////////
		// Getters //
		/////////////

		/**
		* @brief Returns true if auto-update is enabled
		*
		* If true, the scene node will update particles automatically, else the user must call update().
		*
		* @return true if auto-update is enabled, false if disabled
		*/
        bool isAutoUpdateEnabled() const;

		/**
		* @brief Returns true if auto-update is performed only when visible
		* @return True if particles are updated only if the scene node is visible (use setVisible() to change visibility).
		*/
        bool isUpdateOnlyWhenVisible() const;

		/**
		* @brief Tells whether this system is world transformed or not
		*
		* If a system is transformed in the world, only its emitter and zones will be transformed.<br>
		* The emitted particles will remain independent from the system transformation.<br>
		* <br>
		* If it is transformed locally, the emitted particles will be transformed as well.
		*
		* @return true if this system is world transformed, false if not
		*/
		bool isWorldTransformed() const;

		/** 
		* @brief Gets the bounding box
		*
		* Note that the returned bounding box is invalid if aabb computation is disabled.
		*
		* @return the bounding box containing all particles
		*/
        virtual const yon::core::aabbox3d<yon::f32>& getBoundingBox() const;

		/**
		* @brief Tells whether the system has finished or not
		*
		* This method will return true if :
		* <ul>
		* <li>There is no more active particles in the system</li>
		* <li>All the emitters in the system are sleeping</li>
		* </ul>
		* The YONSystem provides an accessor for this as the update can be done within the Yon scene manager.
		* In that case the user cannot get the returned value of update(float).
		*
		* @return true if the system has finished, false otherwise
		*/
		bool hasFinished() const;

		///////////////
		// Interface //
		///////////////

		virtual bool update(float deltaTime);

		/** 
		* @brief Renders the particles in the system 
		* 
		* (Reimplementation of the yon::scene::SceneNode render method.)
		*/
		virtual void render();

        /** 
		* @brief Renders the particles in the system 
		* 
		* (Reimplementation of the SPK::System render method.)
		*/
        virtual void render() const;

		/** @brief This method is called just before the rendering process of the whole scene */
        virtual void OnRegisterSceneNode();

		/** 
		* @brief This method is called just before rendering the whole scene 
		*
		* It should be called only by engine.
		*
		* @param timeMs : Current time in milliseconds
		**/
        virtual void OnAnimate(yon::u32 timeMs);

		/** @brief Updates the absolute transformation of this Yon Node */
		virtual void updateAbsolutePosition();

    private:

        bool AutoUpdate;
		bool AlwaysUpdate;

		const bool worldTransformed;

		bool finished;

        mutable yon::core::aabbox3d<yon::f32> BBox;
		mutable yon::u32 lastUpdatedTime;

		virtual void onRegister();
		virtual void onUnregister();

		// This sets the right camera position if distance computation is enabled for a group of the system
		void updateCameraPosition() const;
    };


	inline YONSystem* YONSystem::create(yon::scene::ISceneNode* parent,yon::scene::ISceneManager* mgr,bool worldTransformed,yon::s32 id)
	{
		YONSystem* obj = new YONSystem(parent,mgr,worldTransformed,id);
		registerObject(obj);
		return obj;
	}

    inline bool YONSystem::isAutoUpdateEnabled() const
    {
        return AutoUpdate;
    }

    inline bool YONSystem::isUpdateOnlyWhenVisible() const
    {
        return !AlwaysUpdate;
    }

	inline bool YONSystem::isWorldTransformed() const
	{	
		return worldTransformed;
	}

	inline bool YONSystem::hasFinished() const
	{
		return finished;
	}

    inline void YONSystem::setAutoUpdateEnabled(bool enableState, bool onlyWhenVisible)
    {
        AutoUpdate = enableState;
        AlwaysUpdate = !onlyWhenVisible;
    }

	inline bool YONSystem::update(float deltaTime)
	{
		updateCameraPosition();
		finished = !System::update(deltaTime);
		return !finished;
	}
	
	inline void YONSystem::render()
	{
		const_cast<const YONSystem*>(this)->render();
	}

	inline void YONSystem::onRegister()
	{
		grab(); // when the factory registers a system, it grabs it so that it will not be destroyed by Yon
	}

	inline void YONSystem::onUnregister()
	{
		remove(); // Removes the YONSystem from the Yon scene graph
	}
}}

#endif

