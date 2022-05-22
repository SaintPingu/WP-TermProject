#pragma once

#define TIMERID_INVALIDATE 0
#define ELAPSE_INVALIDATE 10

#define TIMERID_ANIMATION 1
#define ELAPSE_ANIMATION 100

#define TIMERID_MOVE_PLAYER 2
#define ELAPSE_MOVE_PLAYER 10

#define TIMERID_SHOOT_BULLET 3
#define ELAPSE_SHOOT_BULLET 100

#define TIMERID_MOVE_OBJECT 4
#define ELAPSE_MOVE_OBJECT 10

#define TIMERID_CREATE_ENEMY 5
#define ELAPSE_CREATE_ENEMY 2000

#define TIMERID_EFFECT 6
#define ELAPSE_EFFECT 50

void CALLBACK T_Invalidate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK T_Animate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK T_MovePlayer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK T_ShotBullet(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK T_MoveObject(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK T_CreateEnemy(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void CALLBACK T_Effect(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
