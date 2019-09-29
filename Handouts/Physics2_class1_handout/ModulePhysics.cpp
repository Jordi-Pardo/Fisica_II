#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include "Box2D/Box2D/Box2D.h"

// TODO 1: Include Box 2 header and library
#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/b2_Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/2_Release/Box2D.lib" )
#endif // _DEBUG


ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	
	// TODO 2: Create a private variable for the world
	// - You need to send it a default gravity
	// - You need init the world in the constructor
	// - Remember to destroy the world after using it
	b2Vec2 gravity(0.0f,10.0f);
	world = new b2World(gravity);
	// TODO 4: Create a a big static circle as "ground"
	CreateCircle(PIXELS_TO_METERS(SCREEN_WIDTH /2.f), PIXELS_TO_METERS(SCREEN_HEIGHT /2.f),2.f,0);
	CreateBox(PIXELS_TO_METERS(SCREEN_WIDTH / 2.f), PIXELS_TO_METERS(SCREEN_HEIGHT / 2.f), 2.f, 2.f, 0);
	return true;
}



void ModulePhysics::CreateCircle(float posX, float posY, float radius,int isDynamic)
{	
	b2BodyType type;
	
	switch (isDynamic)
	{
	case 0:
		type = b2BodyType::b2_staticBody;
		break;
	case 1:
		type = b2BodyType::b2_dynamicBody;
		break;
	}
	b2BodyDef circleBody_def;
	circleBody_def.position.Set(posX, posY);
	circleBody_def.type = type;
	b2Body *groundBody = world->CreateBody(&circleBody_def);
	
	b2CircleShape shape;
	shape.m_radius = radius;

	groundBody->CreateFixture(&shape, 0.0f);

}void ModulePhysics::CreateBox(float posX, float posY, float32 width,float32 height,int isDynamic)
{	
	b2BodyType type;
	
	switch (isDynamic)
	{
	case 0:
		type = b2BodyType::b2_staticBody;
		break;
	case 1:
		type = b2BodyType::b2_dynamicBody;
		break;
	}
	b2BodyDef groundBody_def;
	groundBody_def.position.Set(posX, posY);
	groundBody_def.type = type;
	b2Body *groundBody = world->CreateBody(&groundBody_def);

	b2PolygonShape groundBox;
	groundBox.m_width = 4.f;
	groundBox.SetAsBox(width, height);

	groundBody->CreateFixture(&groundBox, 0.0f);
}

// 
update_status ModulePhysics::PreUpdate()
{
	// TODO 3: Update the simulation ("step" the world)
	world->Step(1.0f / 60.0f, 8, 3);
	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	b2Vec2 mousePos;
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		CreateCircle(PIXELS_TO_METERS(App->input->GetMouseX()), PIXELS_TO_METERS(App->input->GetMouseY()), 0.2f,1);
	}
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;
	b2Body*body = world->GetBodyList();
	
	
	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;
				case b2Shape::e_polygon:
				{

					b2PolygonShape* shape = (b2PolygonShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();

					SDL_Rect rect;
					int posX = METERS_TO_PIXELS(pos.x);
					int posY = METERS_TO_PIXELS(pos.y);
					LOG("%f", shape->m_width);
					rect.x = posX;
					rect.y = posY;
					rect.w = 500;
					rect.h = 100;
					App->renderer->DrawQuad(rect, 205, 255, 255, 255, false, false);
					break;
				}

				// You will have to add more cases to draw boxes, edges, and polygons ...
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;
	return true;
}
