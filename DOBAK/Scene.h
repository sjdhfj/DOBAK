#pragma once
class Scene
{
public:
	virtual void Init() abstract;
	virtual void Update() abstract;
	virtual void Render() abstract;
};
