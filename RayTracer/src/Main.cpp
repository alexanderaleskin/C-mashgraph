#include "Tracer.h"
#include "stdio.h"
#include <string>

using namespace glm;

void main(int argc, char** argv)
{
	CTracer tracer;
	CScene scene;
	static const float mass = 8.57e36f; // для удобства ввода фиксируем

	int xRes = 1024;  // Default resolution
	int yRes = 768;
	std::string saveto = "../img/";

	if(argc == 2) // There is input file in parameters
	{
	FILE* file = fopen(argv[1], "r");
	if(file)
	{
		int xResFromFile = 0;
		int yResFromFile = 0;
		float pos_x, pos_y, pos_z;
		float dir_x, dir_y, dir_z;
		float up_x, up_y, up_z;
		if(fscanf(file, "%d %d %f %f %f %f %f %f %f %f %f", &xResFromFile, &yResFromFile, 
				  &pos_x, &pos_y, &pos_z, &dir_x, &dir_y, &dir_z, &up_x, &up_y, &up_z) 
			== 11)
		{
			float R, dist;
			vec3 vct;
			saveto += argv[1];
			saveto.pop_back(); saveto.pop_back(); saveto.pop_back();  //delete "txt"
			saveto += "png";
			xRes = xResFromFile;
			yRes = yResFromFile;
			scene.FitImage(true, mass);
			R = scene.R;

			tracer.m_camera.m_pos = vec3(pos_x * R, pos_y * R, pos_z * R);
			dist = length(tracer.m_camera.m_pos) / 10.0f;
			vct = vec3(dir_x, dir_y, dir_z);
			tracer.m_camera.m_forward = normalize(vct) * dist;
			vct =vec3(up_x, up_y, up_z);
			tracer.m_camera.m_up = normalize(vct) * dist;
			tracer.m_camera.m_right= normalize(cross(tracer.m_camera.m_forward, vct)) * dist * ((float)xRes / (float)yRes);
			
			tracer.m_pScene = &scene;
			tracer.RenderImage(xRes, yRes);
			tracer.SaveImageToFile(saveto.c_str());
		}
		else
			printf("Invalid config format! Using default parameters.\r\n");
		fclose(file);
	}
	else
		printf("Invalid config path! Using default parameters.\r\n");
	}
	else
	printf("No config! Using default parameters.\r\n");
}