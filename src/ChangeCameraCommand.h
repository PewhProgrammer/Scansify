#pragma once
#include "ICommand.h"
class ChangeCameraCommand :
	public ICommand
{
public:
	float eye[3];
	float pos[3];
	float up[3];

	ChangeCameraCommand(float eyex, float eyey, float eyez, float posx, float posy, float posz, float upx, float upy, float upz);
	~ChangeCameraCommand();
	void execute() override;
	bool verify() override;
};

