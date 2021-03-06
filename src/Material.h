#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "Types.h"

/**
 *  Material class
 *  
 */

class Material
{
private:
    vec3 diffuseColor;
    vec3 emission;

    f32 opacity;
    f32 specularFactor; // 0 is perfectly diffuse, 1 is perfectly specular
    f32 refractiveIndex;

public:

    Material()
    {
        //Defaultvalues
        opacity = 1.0f;             // Perfectly opaque
        specularFactor = 0.3f;      // Very diffuse
        refractiveIndex = 1.0f;     // Som refractive index? Air ≈ 1.0?
        diffuseColor = vec3(0.5f);  // Grey color

    }

    inline const vec3 &getDiffuseColor() { return diffuseColor; }
    inline const vec3 &getEmission() { return emission; }
    inline const f32 &getSpecularFactor() { return specularFactor; }
    inline const f32 &getOpacity() { return opacity; }
    inline const f32 &getRefractiveIndex() { return refractiveIndex; }

    void setDiffuseColor(f32 r, f32 g, f32 b) { diffuseColor = glm::clamp(vec3(r,g,b),0.0f,1.0f); }
    void setEmission(f32 r, f32 g, f32 b) { emission = glm::clamp(vec3(r,g,b),0.0f,MAXFLOAT); }

    void setOpacity(f32 val) { opacity = glm::clamp(val,0.0f,1.0f); }
    void setRefractiveIndex(f32 val) { refractiveIndex = glm::clamp(val,0.0f,5.0f); }
    void setSpecularFactor(f32 val) { specularFactor = glm::clamp(val,0.0f,1.0f); }
};

#endif
