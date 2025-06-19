#pragma once
#include "Vector.h"

class CGPSNavigator
{
private:
    bool m_bActive;
    CVector m_vecTarget;
    RwTexture* texId;

public:
    CGPSNavigator();

    void SetTarget(const CVector& vecTarget);
    void Clear();
    bool IsActive() const { return m_bActive; }
    void Render();
};
