#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"

#include "Box2D/Box2D/Box2D.h"
class b2World;
class b2Vec2;
class b2Body;
class BodyData;
ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");

	circle_rect = { 0,0,50,50 };
	rectangle_rect = { 0,0,100,50 };
	ricky_rect = { 0,0,117,150 };
	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	// TODO 5: Move all creation of bodies on 1,2,3 key press here in the scene
		// On space bar press, create a circle on mouse position
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25);
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		// TODO 1: When pressing 2, create a box on the mouse position
		App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 50, 20);
		// TODO 2: To have the box behave normally, set fixture's density to 1.0f
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// TODO 3: Create a chain shape using those vertices
		// remember to convert them from pixels to meters!

		App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY());

	}
	// TODO 7: Draw all the circles using "circle" texture

	for (int i = 0; i < MAX_BODIES && App->physics->bodyData[i].body != nullptr; i++)
	{
		
		b2Shape::Type type;
		type = App->physics->bodyData[i].type;
		switch (type)
		{
		case b2Shape::e_circle:
		App->renderer->Blit(circle,METERS_TO_PIXELS(App->physics->bodyData[i].GetPosX() - App->physics->bodyData[i].m_radius),METERS_TO_PIXELS(App->physics->bodyData[i].GetPosY()-App->physics->bodyData[i].m_radius),&circle_rect,1.0f,App->physics->bodyData[i].GetRotation()*RADTODEG);
			break;
		case b2Shape::e_edge:
			break;
		case b2Shape::e_polygon:
		App->renderer->Blit(box,METERS_TO_PIXELS(App->physics->bodyData[i].GetPosX()-rectangle_rect.w/2),METERS_TO_PIXELS(App->physics->bodyData[i].GetPosY() -rectangle_rect.h/2),&rectangle_rect,1.0f,App->physics->bodyData[i].GetRotation()*RADTODEG);
			break;
		case b2Shape::e_chain:
		App->renderer->Blit(rick,METERS_TO_PIXELS(App->physics->bodyData[i].GetPosX()),METERS_TO_PIXELS(App->physics->bodyData[i].GetPosY()),&ricky_rect,1.0f,App->physics->bodyData[i].GetRotation()*RADTODEG);
			break;
		case b2Shape::e_typeCount:
			break;
		default:
			break;
		}
	}
	return UPDATE_CONTINUE;
}
