#pragma once

#include "World.hpp"
#include "ObjectBase.hpp"

class Object : public ObjectBase
{
	GENERATED_BODY(Object, ObjectBase)
public:

	Object();

public: //~~~~~~~~~~~~~~| Common

	virtual void OnConstructed() override;
	virtual void OnEndPlay()     override;

	bool IsValid(Object* object) const;

public: //~~~~~~~~~~~~~~| Tick

	virtual void Tick(float DeltaTime, ETickType type);

	void RegisterTick  (ITickFunction& tick);
	void UnregisterTick(ITickFunction& tick);

protected:

	/// >> tIck
	TickFunction PrimatyTick;
	/// << 
};