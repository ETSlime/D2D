#include "FloorGO.h"


FloorGO::FloorGO(int floor):floor(floor){}

void FloorGO::Init()
{
	map = new TMap(floor);
}

void FloorGO::Destroy()
{
	SAFE_DELETE(map);

}

void FloorGO::Update()
{
	map->Update();
}

void FloorGO::Render()
{
	map->Render();
}

void FloorGO::PostRender()
{
}

void FloorGO::GUI()
{
}

bool FloorGO::IsValid()
{
	return valid;
}

bool FloorGO::IsDestroyed()
{
	return destroy;
}

void FloorGO::SetIsValid(bool _valid)
{
	this->valid = _valid;
}

void FloorGO::SetIsDestroyed(bool _destroy)
{
	this->destroy = _destroy;
}