#pragma once
#include "Module.h"
#include "Globals.h"

#define METERS_TO_PIXELS(x) x *100.f
#define PIXELS_TO_METERS(x) x/100.f

class b2World;


class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	void CreateCircle(float posX, float posY, float radius,int isDynamic);
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

private:
	b2World* world = nullptr;
	bool debug;
};