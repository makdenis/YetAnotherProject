#pragma once

#include <array>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "Types.hpp"
#include "Misc.hpp"
#include "ObjectBase.hpp"

#include "Threading/Initialiser.hpp"


class Actor;
class Object;
class ActorComponent;


/** 
 */
class World
{
public:
			 World();
	virtual ~World();

public:

	void OnSimulationStart();
	void OnSimulationStop();

public: //~~~~~~~~~~~~~~| Tick

	void DoTick(float DeltaTime, ETickType type);

	void RegisterTickFunction  (ITickFunction& Tick);
	void UnregisterTickFunction(ITickFunction& Tick);

public: //~~~~~~~~~~~~~~| Construction

	template<class _T>
	_T* CreateObject()
	{
		auto* initialiser = ThreadContext::TopInitialiser();
		assert(initialiser);
		auto* name = initialiser->name;
		assert(name);
		UpdateNameToUnique(*name);

		OUID newOUID = lastOUID++;
		ThreadContext::TopInitialiser()->ouid = newOUID;

		auto base = UNIQUE(ObjectBase)(new _T);

		auto* pointer_base = base.get();
		auto* pointer = static_cast<_T*>(pointer_base);

		objects.emplace(newOUID, std::move(base));
		objects_set.emplace(pointer);

		pointer->OnConstructed();

		if (SimulationState != eUnstarted)
		{
			pointer->OnBeginPlay();
		}

		return pointer;
	}

	void DestroyObject(Object* object);

public:

	bool IsValid(Object* base) const;

public: //~~~~~~~~~~~~~~| 

	const ActorComponent* GetSceneRoot() const	{ return sceneRoot; }
		  ActorComponent* GetSceneRoot()		{ return sceneRoot; }

protected:

	void UpdateNameToUnique(std::string& name);

protected:

	/// >>
	ESimulationState SimulationState;
	/// << 

	/// >> physics
	
	/// << 

	/// >> graphic
	ActorComponent* sceneRoot;
	/// <<

	/// >> indices
	std::unordered_set<Object*> objects_set;
	std::unordered_map<std::string, Index> names;

	OUID lastOUID;

	std::unordered_map<OUID, UNIQUE(ObjectBase), std::hash<size_t>> objects;
	/// <<

	/// >> tick functions
	std::array< 
		std::unordered_set< ITickFunction* >, 
		ETickType::eMAX
	> tickFunctions;
	/// <<

public: //~~~~~~~~~~~~~~| Iteration

	class SceneIterator
	{
	public:
		SceneIterator(ActorComponent* root);

		ActorComponent* operator->();
		ActorComponent& operator* ();
		SceneIterator*  operator++();
		bool operator==(const SceneIterator& r) const;
		bool operator!=(const SceneIterator& r) const;

	private:

		ActorComponent* PrevNode();
		ActorComponent* CurrNode();
		ActorComponent* Brunch(size_t node);
		std::vector<ActorComponent*>& CurrLine();

		size_t CurrIndex();
		size_t MaxIndex();
		void Move();
		
		std::stack<size_t> indices;
		std::stack<ActorComponent*> path;
	};

	SceneIterator begin() { return SceneIterator(sceneRoot); }
	SceneIterator end()   { return SceneIterator(nullptr);   }
};
