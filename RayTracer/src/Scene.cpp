#include "Scene.h"
#include "stdio.h"

using namespace glm;

void CScene::FitImage(bool IncludeDisk, float mass)
{
	static const char* backfilename = "../data/stars2.jpg";
	static const char* diskfilename = "../data/disk_32.png";
	static const float G = 6.674e-11f;
	static const float c = 3e+8f;

	R = 2 * mass * G / (c * c);
	coef = mass * G;
	disk_shape = vec2(5 * R, 5 * R);
	back = new CImage;
	if(!SUCCEEDED(back->Load(backfilename)))
	{
		delete back;
		back = nullptr;
		printf("Can't read from %s", backfilename);
	}
	else
	{
		back_x = back->GetWidth();
		back_y = back->GetHeight();
		if(IncludeDisk)
		{
			disk = new CImage;
			if(!SUCCEEDED(disk->Load(diskfilename)))
			{
				delete disk;
				delete back;
				disk = nullptr;
				back = nullptr;
				printf("Can't read from %s", diskfilename);
			}
			else
			{
				disk_x = disk->GetWidth();
				disk_y = disk->GetHeight();
			}
		}
	}
	return;
}

glm::vec3 CScene::GetBackColor(float phi, float teta)
{
	static const float pi = 3.14159265359;

	int x = round((phi / pi / 2) * back_x);
	int y = round((teta + pi / 2) / pi * back_y);

	auto pData = (unsigned char*)back->GetBits();
	int pitch = back->GetPitch();
	pData += pitch * y;
	float b = (float)pData[x * 3] / 255;
	float g = (float)pData[x * 3 + 1] / 255;
    float r = (float)pData[x * 3 + 2] / 255;
	return vec3(r, g, b);
}

glm::vec4 CScene::GetDiskColor(float x, float y)
{
	int p_x = round((x / disk_shape.x / 2 + 0.5) * disk_x);
	int p_y = round((y / disk_shape.y / 2 + 0.5) * disk_y);

	if (p_x == disk_x) p_x--;
	if (p_y == disk_y) p_y--;
	auto pData = (unsigned char*)disk->GetBits();
	int pitch = disk->GetPitch();

	pData += pitch * p_y;
	float alfa = (float)pData[p_x * 4 + 3] / 255;
	float b = (float)pData[p_x * 4] / 255;
	float g = (float)pData[p_x * 4 + 1] / 255;
    float r = (float)pData[p_x * 4 + 2] / 255;
	return vec4(r, g, b, alfa);
}







