#include "CGPSNavigator.h"
#include <imgui.h>
#include "../game/game.h"
#include "../game/RW/RenderWare.h"
#include "../util/CUtil.h"
#include "../main.h"

extern CGame* pGame;

CGPSNavigator::CGPSNavigator()
{
    m_bActive = false;
    m_vecTarget.x = 0.0f;
    m_vecTarget.y = 0.0f;
    m_vecTarget.z = 0.0f;

    // โหลด texture จาก TXD (samp.txt)
    texId = CUtil::LoadTextureFromDB("samp", "gps_icon");  
    FLog("CGPSNavigator::LoadTexture : gps_icon ...");
}

void CGPSNavigator::SetTarget(const CVector& vecTarget)
{
    m_vecTarget = vecTarget;
    m_bActive = true;
    FLog("CGPSNavigator::SetTarget");
}

void CGPSNavigator::Clear()
{
    m_bActive = false;
    FLog("CGPSNavigator::Clear");
}

void CGPSNavigator::Render()
{
    if (!m_bActive) return;

    // แปลงตำแหน่ง 3D -> 2D
    CVector vecScreen;
    ((void (*)(CVector*, CVector*, float*, float*, bool, bool))(g_libGTASA + 0x6E9DF8))(&m_vecTarget, &vecScreen, 0, 0, 0, 0);

    if (vecScreen.z < 1.0f) return;

    // ขนาด icon
    float size = 64.0f;
    float x = vecScreen.x - size * 0.5f;
    float y = vecScreen.y - size * 0.5f;

    // วาด icon
    ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)texId->raster, ImVec2(x, y), ImVec2(x + size, y + size));

    // คำนวณระยะ
    if (!pGame || !pGame->FindPlayerPed() || !pGame->FindPlayerPed()->m_pPed) return;

    CVector playerPos = pGame->FindPlayerPed()->m_pPed->m_matrix->m_pos;
    float dist = sqrtf(
        (playerPos.x - m_vecTarget.x) * (playerPos.x - m_vecTarget.x) +
        (playerPos.y - m_vecTarget.y) * (playerPos.y - m_vecTarget.y) +
        (playerPos.z - m_vecTarget.z) * (playerPos.z - m_vecTarget.z)
    );

    // สร้างข้อความ
    char szText[32];
    // แปลงข้อความตามระยะ (m หรือ km)
    if (dist < 1000.0f)
    snprintf(szText, sizeof(szText), "ระยะ: %.2f m", dist);
    else
    snprintf(szText, sizeof(szText), "ระยะ: %.2f km", dist / 1000.0f);

    // แสดงข้อความตรงกลางไอคอน
    ImVec2 textSize = ImGui::CalcTextSize(szText);
    float textX = vecScreen.x - textSize.x * 0.5f;
    float textY = y + size + 4.0f;

    ImGui::GetBackgroundDrawList()->AddText(ImVec2(textX, textY), IM_COL32(255, 255, 255, 255), szText);

    FLog("CGPSNavigator::Render");
}
