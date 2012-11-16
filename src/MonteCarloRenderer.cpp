#include "MonteCarloRenderer.h"


inline vec3 reinhardTonemap(const vec3 &x)
{
    return x/(x + 1.0f);
}

vec3 uncharted2Tonemap(const vec3 &x)
{
    float A = 0.15f;
    float B = 0.50f;
    float C = 0.10f;
    float D = 0.20f;
    float E = 0.02f;
    float F = 0.30f;

    return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

vec3 MonteCarloRenderer::radiance(const Ray &ray)
{
    vec3 rad;

    Hit hit = rayTraceNode(ray,0);
    if(hit < MAXFLOAT)
    {
		const f32 limit = 0.9;
		f32 r = glm::compRand1(0.0f,1.0f);
        rad = matLib[materials[hit.index]].getEmission();
        if(r < limit)
        {
			r = r / limit;
        	// calculate plane vectors
        	vec3 n = interpolateNormal(faces[hit.index], hit.baryCoords);
        	vec3 origin = ray.origin + ray.direction*hit.distance + n*.01f;
        	vec3 a = faces[hit.index].nOrt;
        	vec3 b = glm::cross(n,a);

            vec3 reflection = ray.direction - 2.0f * glm::dot(ray.direction,n) * n;
            f32 phi = glm::compRand1(0.0f,2.0f*PI);
            vec3 diffuse = a*r*glm::cos(phi) + b*r*glm::sin(phi) + n*glm::sqrt(1.0f-r*r);

            f32 t = matLib[materials[hit.index]].getSpecularFactor();

            // Do a linear interpolation between a perfect diffuse and a perfect reflection according to specularFactor

			Ray newRay( origin, glm::normalize(diffuse*(1.0f-t) + reflection*t) );
            rad = matLib[materials[hit.index]].getEmission() + matLib[materials[hit.index]].getDiffuseColor()*radiance(newRay);
        }
    }

    return rad;
}

inline vec3 MonteCarloRenderer::rayTraceBVH(const Ray &ray)
{
    return glm::sqrt(radiance(ray));
}
