//
//  main.cpp
//  L-SystemTest
//
//  Created by 山崎 慎太郎 on 2014/12/11.
//  Copyright (c) 2014年 山崎 慎太郎. All rights reserved.
//

#include <iostream>
#include <GLUT/glut.h>

/// 画面高さ
#define WINDOW_HEIGHT 800
/// 画面幅
#define WINDOW_WIDTH 800
/// 目盛り数
#define TICK_NUM 100

char title[] = "Life Game";

/// セル色情報(白黒)
bool cells[TICK_NUM * TICK_NUM + 1];

/// セル幅
static double interval = (double) WINDOW_WIDTH / (double) TICK_NUM;

/// 世代数
int count;

/// 初期処理
void init() {
    srand((unsigned int)time(NULL));
    // セル色情報の初期化
    for (int i = 0; i < TICK_NUM; i++) {
        if (rand() % 2) {
            cells[i] = true;
            
            int x = i % TICK_NUM;
            int y = i / TICK_NUM;

            glBegin(GL_QUADS);
            glColor4d(0.0, 0.0, 0.0, 1.0);
            glVertex2d(interval * x, interval * y);
            glVertex2d(interval * x, interval * (y + 1));
            glVertex2d(interval * (x + 1), interval * (y + 1));
            glVertex2d(interval * (x + 1), interval * y);
            glEnd();
            
        } else {
            cells[i] = false;
        }
    }
    for (int i = TICK_NUM; i < TICK_NUM * (TICK_NUM - 1); i++) {
        cells[i] = false;
    }
    
    glFlush();
}

/// マス目描画
void drawGlid() {
    // マス目を書く
    for (int i = 0; i < TICK_NUM; i++) {
        // 縦線
        glBegin(GL_LINES);
        glColor3b(0.0, 0.0, 0.0);
        glVertex2d(interval * i, 0);
        glVertex2d(interval * i, WINDOW_HEIGHT);
        glEnd();
        
        // 横線
        glBegin(GL_LINES);
        glColor3b(0.0, 0.0, 0.0);
        glVertex2d(0, interval * i);
        glVertex2d(WINDOW_WIDTH, interval * i);
        glEnd();
    }
    
    glFlush();
}

/// ルールチェック
bool checkRule(bool left, bool center, bool right) {
    int result = 0;
    
    if (left) {
        result += 1;
    }
    if (center) {
        result += 1;
    }
    if (right) {
        result += 1;
    }
    
    if (result == 2 || result == 3) {
        return true;
    } else {
        return false;
    }
}

/// 世代更新
void updateGeneration() {
    count++;
    
    if (count > TICK_NUM) {
        return;
    }
    
    /// 色のつくセル
    int newCells[TICK_NUM];
    int index = 0;
    bool left, center, right;
    
    for (int i = 0; i < TICK_NUM; i++) {
        // 左セル
        if (i != 0) {
            left = cells[TICK_NUM * (count - 1) + i - 1];
        } else {
            left = false;
        }
        // 中央セル
        center = cells[TICK_NUM * (count - 1) + i];
        // 右セル
        if (i != TICK_NUM - 1) {
            right = cells[TICK_NUM * (count - 1) + i + 1];
        } else {
            right = false;
        }
        
        if (checkRule(left, center, right)) {
            newCells[index] = i;
            index += 1;
        }
    }
    
    for (int i = 0; i < index; i++) {
        cells[TICK_NUM * count + newCells[i]] = true;
        
        int x = newCells[i];
        int y = count;
        
        glBegin(GL_QUADS);
        glColor4d(0.0, 0.0, 0.0, 1.0);
        glVertex2d(interval * x, interval * y);
        glVertex2d(interval * x, interval * (y + 1));
        glVertex2d(interval * (x + 1), interval * (y + 1));
        glVertex2d(interval * (x + 1), interval * y);
        glEnd();
    }
    
    glFlush();
}

/// リセット
void reset() {
    // 画面背景を白に
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    count = 0;
    
    drawGlid();
    
    init();
}

void disp(void)
{
    reset();
    
}

void resize(int w, int h)
{
    /* ウィンドウ全体をビューポートにする */
    glViewport(0, 0, w, h);
    
    /* 変換行列の初期化 */
    glLoadIdentity();
    
    /* スクリーン上の座標系をマウスの座標系に一致させる */
    glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
}

/// マウスアクション
void mouse(int button, int state, int x, int y)
{
    switch (button) {
            // 左ボタン
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                int i = x / interval;
                int j = y / interval;
                
                // クリックされたセルをtrueに
                cells[j * TICK_NUM + i] = true;
                
                // セル描画
                glBegin(GL_QUADS);
                glColor4d(0.0, 0.0, 0.0, 1.0);
                glVertex2d(interval * i, interval * j);
                glVertex2d(interval * i, interval * (j + 1));
                glVertex2d(interval * (i + 1), interval * (j + 1));
                glVertex2d(interval * (i + 1), interval * j);
                glEnd();
                
                glFlush();
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN) {
                int i = x / interval;
                int j = y / interval;
                
                // クリックされたセルをfalseに
                cells[j * TICK_NUM + i] = false;
                
                
                // セル描画
                glBegin(GL_QUADS);
                glColor4d(1.0, 1.0, 1.0, 1.0);
                glVertex2d(interval * i, interval * j);
                glVertex2d(interval * i, interval * (j + 1));
                glVertex2d(interval * (i + 1), interval * (j + 1));
                glVertex2d(interval * (i + 1), interval * j);
                glEnd();
                
                // 枠線描画
                glBegin(GL_LINE_LOOP);
                glColor4d(0.0, 0.0, 0.0, 1.0);
                glVertex2d(interval * i, interval * j);
                glVertex2d(interval * i, interval * (j + 1));
                glVertex2d(interval * (i + 1), interval * (j + 1));
                glVertex2d(interval * (i + 1), interval * j);
                glEnd();
                
                glFlush();
            }
            
            break;
    }
}

/// キーアクション
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case ' ':
            
            if (count < TICK_NUM) {
                updateGeneration();
            }
            
            break;
        case 'r':
            reset();
            
            break;
        default:
            break;
    }
}

int main(int argc , char ** argv) {
    glutInit(&argc , argv);
    glutInitWindowPosition(100 , 50);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    
    glutCreateWindow(title);
    glutDisplayFunc(disp);
    
    glutReshapeFunc(resize);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
    return 0;
}