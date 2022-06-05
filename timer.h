#pragma once

#define TIMERID_INVALIDATE 0
#define ELAPSE_INVALIDATE 10

#define TIMERID_ANIMATION 1
#define ELAPSE_ANIMATION 50

#define TIMERID_ANIMATION_BOSS 2
#define ELAPSE_ANIMATION_BOSS 100

#define TIMERID_MOVE_PLAYER 3
#define ELAPSE_MOVE_PLAYER 10

#define TIMERID_EFFECT 4
#define ELAPSE_EFFECT 50

#define TIMERID_GUI 5
#define ELAPSE_GUI 10

void CALLBACK T_Invalidate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK T_Animate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK T_AnimateBoss(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK T_MovePlayer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK T_Effect(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK T_GUI(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
