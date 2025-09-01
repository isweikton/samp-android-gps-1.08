//
// Created by weikton on 01.09.2025.
//

#include "../main.h"
#include "RW/RenderWare.h"

#include "CGPS.h"

#include "../gui/gui.h"
extern CGUI* pGUI;

#include "game.h"
extern CGame* pGame;

VECTOR CGPS::m_vecTarget;

void CGPS::Init() {
    Clear();
    m_pGPS = (RwTexture*)LoadTextureFromDB("samp", "afk_icon"); // .. set gps_icon, or anything else.
}

void CGPS::SetTarget(const VECTOR& vecTarget) {
    m_vecTarget = vecTarget;
}

void CGPS::Clear() {
    m_vecTarget = {0.f, 0.f, 0.f};
}

void CGPS::Render() {
    if (m_vecTarget.X == 0.f && m_vecTarget.Y == 0.f) {
        return;
    }

    MATRIX4X4 vecScreen;
    ((void (*)(VECTOR*, VECTOR*, float*, float*, bool, bool))(g_libGTASA + 0x54EEC0 + 1))(&m_vecTarget, &vecScreen.pos, 0, 0, 0, 0); // CSprite::CalcScreenCoors

    if (vecScreen.pos.Z < 1.0f) {
        return;
    }

    float size = 64.0f;
    float x = vecScreen.pos.X - size * 0.5f;
    float y = vecScreen.pos.Y - size * 0.5f;

    if (!pGame || !pGame->FindPlayerPed() || !pGame->FindPlayerPed()->m_pPed) {
        return;
    }

    MATRIX4X4 playerPos;
    pGame->FindPlayerPed()->GetMatrix(&playerPos);
    float dist = sqrtf(
        (playerPos.pos.X - m_vecTarget.X) * (playerPos.pos.X - m_vecTarget.X) +
        (playerPos.pos.Y - m_vecTarget.Y) * (playerPos.pos.Y - m_vecTarget.Y) +
        (playerPos.pos.Z - m_vecTarget.Z) * (playerPos.pos.Z - m_vecTarget.Z)
    );

    char szText[32];

    if(dist <= 5.0f) // .. you are near icon = delete this real sh*t!
        Clear();

    if (dist < 1000.0f) {
        snprintf(szText, sizeof(szText), "%.0f M", dist); // .. 1000 M
    } else {
        snprintf(szText, sizeof(szText), "%.1f KM", dist / 1000.0f); // .. 1.7 KM
    }
    ImVec2 textSize = ImGui::CalcTextSize(szText);
    float textX = vecScreen.pos.X - textSize.x * 0.5f;
    float textY = y + size + 4.0f;

    ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)m_pGPS->raster, ImVec2(x, y), ImVec2(x + size, y + size));
    ImGui::GetBackgroundDrawList()->AddText(ImVec2(textX, textY), IM_COL32(255, 255, 255, 255), szText);
}
