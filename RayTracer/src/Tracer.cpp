#include "Tracer.h"

using namespace glm;

SRay CTracer::MakeRay(glm::uvec2 pixelPos)
{
	static const float c = 3e+8;

	auto ray = SRay();
	ray.m_start = m_camera.m_pos;
	ray.m_dir = m_camera.m_forward + (float)((pixelPos.x + 0.5) / m_camera.m_resolution.x - 0.5) * m_camera.m_right
								   + (float)((pixelPos.y + 0.5) / m_camera.m_resolution.y - 0.5) * m_camera.m_up;
	ray.m_dir = normalize(ray.m_dir) * c;
	return ray;
}

glm::vec3 CTracer::TraceRay(SRay ray)
{
	static const float c = 3e+8;

	float coef2 = c / 200; 
	bool executing = true;
	//float a_n;
	float time;
	auto color = vec3(0, 0, 0);
	vec3 a = -(float)(m_pScene->coef / pow(length(ray.m_start), 3)) * ray.m_start;
	vec3 last_p;
	float gamma = 0; // alfa in the help book

	int it = 0;
	while (executing)
	{
		time = coef2 / length(a);
		last_p = ray.m_start;
		ray.m_start += ray.m_dir * time + a * time * time / 2.0f;
		ray.m_dir += a * time;
		ray.m_dir = normalize(ray.m_dir) * c;
		a = -(float)(m_pScene->coef / pow(length(ray.m_start), 3)) * ray.m_start;
		executing = (length(ray.m_start) < m_pScene->R * 200) && (length(ray.m_start) > m_pScene->R);
		if (executing && ray.m_start.x * last_p.x <= 0)
		{
			float alfa = abs(last_p.x / (ray.m_start.x - last_p.x));
			float pos_x = (ray.m_start.y - last_p.y) * alfa + last_p.y;
			float pos_y = (ray.m_start.z - last_p.z) * alfa + last_p.z;

			if (abs(pos_x) <= m_pScene->disk_shape.x && abs(pos_y) <= m_pScene->disk_shape.y)
			{
				auto vector = m_pScene->GetDiskColor(pos_x, pos_y);
				gamma = vector.w;
				vec3 color2(vector);
				color += gamma * color2;
			}
		}
		it++;
	}
	it++;
	if (length(ray.m_start) >= m_pScene->R * 200)
	{
		vec3 dir = normalize(ray.m_dir);
		color += (1 - gamma) * m_pScene->GetBackColor(atan2(dir.x, dir.y), asin(dir.z));
	}
	return color;
};

void CTracer::RenderImage(int xRes, int yRes)
{
  // Reading input texture sample
/*  CImage* pImage = LoadImageFromFile("data/disk_32.png");
  if(pImage->GetBPP() == 32)
  {

    auto pData = (unsigned char*)pImage->GetBits();
    auto pCurrentLine = pData;
    int pitch = pImage->GetPitch();

    for(int i = 0; i < pImage->GetHeight(); i++) // Image lines
    {
      for(int j = 0; j < pImage->GetWidth(); j++) // Pixels in line
      {
        unsigned char b = pCurrentLine[j * 4];
        unsigned char g = pCurrentLine[j * 4 + 1];
        unsigned char r = pCurrentLine[j * 4 + 2];
        unsigned char alpha = pCurrentLine[j * 4 + 3];
      }
	  pCurrentLine += pitch;
    }

  }
 */ 
	static const float pi = 3.14159265359;
  // Rendering
  m_camera.m_resolution = uvec2(xRes, yRes);
  m_camera.m_pixels.resize(xRes * yRes);

  for(int i = 0; i < yRes; i++)
    for(int j = 0; j < xRes; j++)
    {
      SRay ray = MakeRay(uvec2(j, i));
      m_camera.m_pixels[i * xRes + j] = TraceRay(ray);
    }
}

void CTracer::SaveImageToFile(std::string fileName)
{
  CImage image;

  int width = m_camera.m_resolution.x;
  int height = m_camera.m_resolution.y;

  image.Create(width, height, 24);
    
	int pitch = image.GetPitch();
	unsigned char* imageBuffer = (unsigned char*)image.GetBits();

	if (pitch < 0)
	{
		imageBuffer += pitch * (height - 1);
		pitch =- pitch;
	}

	int i, j;
	int imageDisplacement = 0;
	int textureDisplacement = 0;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
		  vec3 color = m_camera.m_pixels[textureDisplacement + j];

		  imageBuffer[imageDisplacement + j * 3] = clamp(color.b, 0.0f, 1.0f) * 255.0f;
		  imageBuffer[imageDisplacement + j * 3 + 1] = clamp(color.g, 0.0f, 1.0f) * 255.0f;
		  imageBuffer[imageDisplacement + j * 3 + 2] = clamp(color.r, 0.0f, 1.0f) * 255.0f;
		}
		imageDisplacement += pitch;
		textureDisplacement += width;
	}

  image.Save(fileName.c_str());
	image.Destroy();
}

CImage* CTracer::LoadImageFromFile(std::string fileName)
{
  CImage* pImage = new CImage;

  if(SUCCEEDED(pImage->Load(fileName.c_str())))
    return pImage;
  else
  {
    delete pImage;
    return NULL;
  }
}