#pragma once

#include "Types.h"
#include "glm/glm.hpp"

#include "string"
#include <atlimage.h>

class CScene
{
	private:
		CImage* back;
		int back_x;
		int back_y;
		CImage* disk;
		int disk_x;
		int disk_y;
	public:
		void FitImage(bool IncludeDisk, float mass);
		glm::vec3 GetBackColor(float phi, float teta);
		glm::vec4 GetDiskColor(float x, float y);
		float R;
		float coef;
		glm::vec2 disk_shape;
};
