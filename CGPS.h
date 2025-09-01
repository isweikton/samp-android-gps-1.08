//
// Created by weikton on 01.09.2025.
//

#pragma once
#include "CVector.h"

class CGPS
{
private:
    static inline RwTexture* m_pGPS = nullptr;
    static VECTOR m_vecTarget;

public:
    static void Init();
    static void Render();

    static void SetTarget(const VECTOR& vecTarget);
    static void Clear();
};
