#pragma once
#include <Windows.h>
#include "flypokemon.h"

#define TIMERID_INVALIDATE 0
#define ELAPSE_INVALIDATE 10

#define TIMERID_ANIMATION 1
#define ELAPSE_ANIMATION 100

#define TIMERID_MOVE_PLAYER 2
#define ELAPSE_MOVE_PLAYER 10

#define TIMERID_SHOOT_BULLET 3
#define ELAPSE_SHOOT_BULLET 100

#define TIMERID_MOVE_BULLET 4
#define ELAPSE_MOVE_BULLET 10

void CALLBACK T_Invalidate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK T_Animate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK T_MovePlayer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK T_ShotBullet(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK T_MoveBullet(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
