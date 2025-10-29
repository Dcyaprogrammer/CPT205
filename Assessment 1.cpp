#define FREEGLUT_STATIC

#ifndef M_PI
#define M_PI 3.1415926535f
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <cstring>
#include <cmath>
#include <GL/freeglut.h>
#include <string>

// three scenes
enum Scene {
    SCENE_COVER,   
    SCENE_LANDSCAPE, 
    SCENE_CAKE        
};
Scene currentScene = SCENE_COVER;

// colors
struct Color {
    float r, g, b;
};
const Color COLOR_WHITE = { 1.0f, 1.0f, 1.0f };
const Color COLOR_XJTLU_BLUE = { 0.0f, 0.0f, 0.5f };
const Color COLOR_XJTLU_BLUE_DARK = { 0.0f, 0.0f, 0.4f };
const Color COLOR_GREY = { 0.7f, 0.7f, 0.7f };
const Color COLOR_BLACK = { 0.1f, 0.1f, 0.1f };

// flags & constants
const float WINDOW_WIDTH = 800.0;
const float WINDOW_HEIGHT = 600.0;

// scene1
float envelopeFlapY;
const float flapStartY = 350.0;
const float flapOpenThresholdY = 580.0;
const float flapMaxY = 650.0;
bool isDraggingFlap = false;
bool isHoveringHandle = false; // <--- 添加这一行，用于高亮
void* evelopeFont = GLUT_STROKE_MONO_ROMAN;

bool isStampPressed = false; // 跟踪邮戳是否“正被按住”
const float stampCX = WINDOW_WIDTH - 100.0f; // 右上角 X
const float stampCY = 500.0f - 100.0f;        // (信封顶部 - 100)
const float stampRadius = 40.0f;             // 邮戳半径

// utils
float getStrokeTextWidth(void* font, const std::string& s) {
    float width = 0.0f;
    for (char ch : s) { // C++11 风格的循环
        width += glutStrokeWidth(font, ch);
    }
    return width;
}


// Drawing Funcs
void drawTextStrokeCentered(float x_center, float y_baseline, const std::string& text, 
                            float scale, float stroke_width, const Color& color) {
    void* font = evelopeFont; // 你可以选择 GLUT_STROKE_MONO_ROMAN

    // 1. 计算文本在缩放前的原始总宽度
    float textWidth = getStrokeTextWidth(font, text);

    // 2. 计算在应用缩放后，我们需要平移多少
    // (中心点 - (总宽度 * 缩放 / 2.0))
    float startX = x_center - (textWidth * scale / 2.0f);

    // 3. 隔离变换
    glPushMatrix();

    // 4. 设置线宽
    glLineWidth(stroke_width);
    
    glColor3f(color.r, color.g, color.b);

    // 5. 进行变换：先平移到 (startX, y_baseline)，再进行缩放
    glTranslatef(startX, y_baseline, 0.0f);
    glScalef(scale, scale, scale);

    // 6. 逐个绘制字符
    for (char ch : text) {
        glutStrokeCharacter(font, ch);
    }

    // 7. 恢复变换和线宽
    glPopMatrix();
    glLineWidth(1.0f); // 恢复默认线宽
}

void drawFilledEllipse(float rx, float ry) {
    int segments = 60; // 60个顶点足以让它看起来很平滑
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        glVertex2f(rx * cosf(theta), ry * sinf(theta));
    }
    glEnd();
}

void drawEnvelope(float left, float right, float bottom, float top, float centerX, float envelopeFlapY)
{
    // 绘制信封主体 (蓝色)
    glColor3f(COLOR_XJTLU_BLUE.r, COLOR_XJTLU_BLUE.g, COLOR_XJTLU_BLUE.b);
    glBegin(GL_POLYGON);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
    glVertex2f(left, top);
    glEnd();

    // 绘制折叠盖 (深蓝色)
    glColor3f(COLOR_XJTLU_BLUE_DARK.r, COLOR_XJTLU_BLUE_DARK.g, COLOR_XJTLU_BLUE_DARK.b);
    glBegin(GL_POLYGON); // 下盖
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right - 100, (top + bottom) / 2.0);
    glVertex2f(left + 100, (top + bottom) / 2.0);
    glEnd();
    glBegin(GL_POLYGON); // 左盖
    glVertex2f(left, bottom);
    glVertex2f(left, top);
    glVertex2f(left + 150, (top + bottom) / 2.0);
    glEnd();
    glBegin(GL_POLYGON); // 右盖
    glVertex2f(right, bottom);
    glVertex2f(right, top);
    glVertex2f(right - 150, (top + bottom) / 2.0);
    glEnd();

    // 绘制顶盖 (蓝色)
    glColor3f(COLOR_XJTLU_BLUE.r, COLOR_XJTLU_BLUE.g, COLOR_XJTLU_BLUE.b);
    glBegin(GL_TRIANGLES);
    glVertex2f(left, top);
    glVertex2f(right, top);
    glVertex2f(centerX, envelopeFlapY);
    glEnd();

    // 绘制顶盖描边 (灰色)
    glColor3f(COLOR_GREY.r, COLOR_GREY.g, COLOR_GREY.b);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(left, top);
    glVertex2f(right, top);
    glVertex2f(centerX, envelopeFlapY);
    glEnd();
}

void drawDragHandle(float centerX, float envelopeFlapY, const Color& color)
{
    // 绘制提示文字 (在盖子上，位置不变)
    drawTextStrokeCentered(centerX, envelopeFlapY - 25.0f, "Drag to Open", 0.15f, 2.0f, color);

    // 在提示文字上方绘制一个向上的小箭头
    glPushMatrix();
    glTranslatef(centerX, envelopeFlapY - 5.0f, 0.0f); // 定位在文字上方
    glLineWidth(2.0f);
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_LINE_STRIP); // 用线条画一个 ^ 符号
    glVertex2f(-10.0f, 0.0f);
    glVertex2f(0.0f, 10.0f);
    glVertex2f(10.0f, 0.0f);
    glEnd();
    glPopMatrix();
    glLineWidth(1.0f); // 恢复默认线宽
}

void drawStamp()
{
    // 1. 根据状态决定颜色
    Color circleColor;
    Color textColor;

    if (isStampPressed) {
        // 按下时：蓝色底，白色字
        circleColor = COLOR_XJTLU_BLUE;
        textColor = COLOR_WHITE;
    }
    else {
        // 正常时：白色底，蓝色字
        circleColor = COLOR_WHITE;
        textColor = COLOR_XJTLU_BLUE;
    }

    // 2. 绘制邮戳的白色/蓝色底
    glPushMatrix();
    glTranslatef(stampCX, stampCY, 0.0f);
    glColor3f(circleColor.r, circleColor.g, circleColor.b);
    drawFilledEllipse(stampRadius, stampRadius); // (假设您有 drawFilledEllipse)
    glPopMatrix();

    // 3. 绘制邮戳上的 "20"
    drawTextStrokeCentered(stampCX, stampCY - 15.0f, "20", 0.3f, 3.0f, textColor);
}

void drawBear(float cx, float cy, float scale, const Color& color) {
    glPushMatrix();
    glTranslatef(cx, cy, 0.0f); // 1. 移动到目标中心点
    glScalef(scale, scale, 1.0f); // 2. 整体缩放

    glColor3f(color.r, color.g, color.b); // 3. 设置颜色 (一次性)

    // 4. 绘制各个部分 (所有坐标都是相对的)
    // (我们先画身体，再画头，最后画耳朵，这样有正确的遮挡)

    // 身体 (一个大的垂直椭圆)
    glPushMatrix();
    glTranslatef(0.0f, -0.1f, 0.0f); // 位置在中心点下方
    drawFilledEllipse(0.5f, 0.6f); // 椭圆半径 (宽0.5, 高0.6)
    glPopMatrix();

    // 头 (一个圆)
    glPushMatrix();
    glTranslatef(0.0f, 0.6f, 0.0f); // 位置在身体上方
    drawFilledEllipse(0.4f, 0.4f); // 半径 (0.4)
    glPopMatrix();

    // 耳朵 (两个小圆)
    glPushMatrix();
    glTranslatef(-0.3f, 0.9f, 0.0f); // 左耳
    drawFilledEllipse(0.15f, 0.15f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.3f, 0.9f, 0.0f); // 右耳
    drawFilledEllipse(0.15f, 0.15f);
    glPopMatrix();

    glPopMatrix(); // 恢复原始矩阵
}

void drawBird(float cx, float cy, float scale, const Color& color) {
    glPushMatrix();
    glTranslatef(cx, cy, 0.0f); // 1. 移动到目标中心点
    glScalef(scale, scale, 1.0f); // 2. 整体缩放

    glColor3f(color.r, color.g, color.b); // 3. 设置颜色

    // 1. 身体 (一个旋转的椭圆)
    glPushMatrix();
    glRotatef(30.0f, 0.0f, 0.0f, 1.0f); // 向上旋转30度
    drawFilledEllipse(0.7f, 0.4f); // 身体椭圆
    glPopMatrix();

    // 2. 头 (一个圆)
    glPushMatrix();
    glTranslatef(0.4f, 0.5f, 0.0f); // 放在身体右上方
    drawFilledEllipse(0.2f, 0.2f);
    glPopMatrix();

    // 3. 鸟喙 (一个三角形)
    glPushMatrix();
    glTranslatef(0.55f, 0.5f, 0.0f); // 从头中心开始
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.05f);
    glVertex2f(0.0f, -0.05f);
    glVertex2f(0.2f, 0.0f); // 喙尖
    glEnd();
    glPopMatrix();

    // 4. 尾巴 (一个三角形)
    glPushMatrix();
    glTranslatef(-0.6f, -0.2f, 0.0f); // 从身体后端开始
    glRotatef(-15.0f, 0.0f, 0.0f, 1.0f); // 尾巴角度
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.15f);
    glVertex2f(0.0f, -0.15f);
    glVertex2f(-0.4f, 0.0f); // 尾尖
    glEnd();
    glPopMatrix();

    glPopMatrix(); // 恢复原始矩阵
}

// Scene1: Cover
/**
 * @brief [已重构] 绘制场景一：封面
 */
void drawSceneCover() {
    // --- 1. 定义坐标 ---
    float left = 50.0, right = 750.0, bottom = 50.0, top = 500.0;
    float centerX = WINDOW_WIDTH / 2.0;

    // --- 2. 绘制信封 ---
    // (已封装，调用新函数)
    drawEnvelope(left, right, bottom, top, centerX, envelopeFlapY);

    // --- 3. 绘制标题 ---
    // (这部分逻辑已经很清晰，保持不变)
    drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 270.0f, "XJTLU 20th Anniversary", 0.22f, 2.0f, COLOR_WHITE);
    drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 240.0f, "A Greeting Card", 0.15f, 2.0f, COLOR_WHITE);

    // --- 4. 绘制交互提示 ---
    // (已封装，调用新函数)
    Color handleColor = COLOR_WHITE;
    if (isHoveringHandle) {
        handleColor = COLOR_GREY;
    }
    drawDragHandle(centerX, envelopeFlapY, handleColor);

    drawStamp();

    // --- 5. 绘制吉祥物 ---
    // (这部分逻辑已经很清晰，保持不变)
    float mascotCenterY = 100.0f;
    float mascotScale = 30.0f;
    float bearCenterX = 120.0f;
    drawBear(bearCenterX, mascotCenterY, mascotScale, COLOR_WHITE);

    float birdCenterX = WINDOW_WIDTH - 120.0f;
    drawBird(birdCenterX, mascotCenterY, mascotScale, COLOR_WHITE);
}



// Scene2: Landscape
void drawSceneLandscape() {
    // 在这里绘制你的校园建筑
    // ...

    // 绘制天空
    glColor3f(0.5, 0.8, 1.0); // 天蓝色
    glBegin(GL_POLYGON);
    glVertex2f(0, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();

    // 绘制一座教学楼 (示例)
    glColor3f(0.9, 0.8, 0.7); // 米色
    glBegin(GL_POLYGON);
    glVertex2f(200, 50);
    glVertex2f(600, 50);
    glVertex2f(600, 400);
    glVertex2f(200, 400);
    glEnd();

    glColor3f(0.5, 0.8, 1.0); // 天蓝色
    glBegin(GL_POLYGON);
    glVertex2f(0, 0); glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT); glVertex2f(0, WINDOW_HEIGHT);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    //drawTextLarge(300, 300, "Scene 2: Landscape");
    //drawTextSmall(300, 250, "Press Space to continue");

}


// Scene3: Cake
void drawSceneCake() {
    // 在这里绘制你的蛋糕
    // ...

    // 绘制蛋糕底层
    glColor3f(0.8, 0.4, 0.2); // 棕色
    glBegin(GL_POLYGON);
    glVertex2f(300, 100);
    glVertex2f(500, 100);
    glVertex2f(500, 200);
    glVertex2f(300, 200);
    glEnd();

    // 绘制蛋糕顶层
    glColor3f(1.0, 0.8, 0.8); // 粉色
    glBegin(GL_POLYGON);
    glVertex2f(325, 200);
    glVertex2f(475, 200);
    glVertex2f(475, 275);
    glVertex2f(325, 275);
    glEnd();

    // 绘制蜡烛 (示例)
    glColor3f(1.0, 1.0, 0.0); // 黄色
    glBegin(GL_POLYGON);
    glVertex2f(395, 275);
    glVertex2f(405, 275);
    glVertex2f(405, 325);
    glVertex2f(395, 325);
    glEnd();

    glColor3f(1.0, 0.8, 0.8); // 粉色
    glBegin(GL_POLYGON);
    glVertex2f(0, 0); glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT); glVertex2f(0, WINDOW_HEIGHT);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    //drawTextLarge(250, 300, "Scene 3: Happy 20th!");
    //drawTextSmall(300, 250, "Press 'r' to restart");
}


// Display
// --- 替换您项目中原来的 'display' 函数 ---
void display(void) {
    // 1. 设置清除颜色 (您可以改成您喜欢的背景色)
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // 2. 每次都重置投影矩阵
    // 这建立了 (0, 800) (0, 600) 的屏幕坐标系
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    // 3. 每次都重置模型视图矩阵
    // 这确保我们从 (0,0) 点开始，没有多余的平移
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 4. --- 核心路由逻辑 ---
    //    现在我们可以安全地调用绘制函数了
    switch (currentScene) {
    case SCENE_COVER:
        drawSceneCover();
        break;
    case SCENE_LANDSCAPE:
        drawSceneLandscape(); // (您场景二的占位符)
        break;
    case SCENE_CAKE:
        drawSceneCake();      // (您场景三的占位符)
        break;
    }

    glutSwapBuffers();
}
// --- 5. 交互回调函数 (更新) ---

/**
 * 鼠标点击回调
 */
 /**
  * @brief 鼠标点击回调 (已更新)
  */
void mouse(int button, int state, int x, int y)
{
    // --- 仅处理左键 ---
    if (button != GLUT_LEFT_BUTTON) {
        return;
    }

    // 将 GLUT 的Y坐标转换为 OpenGL 坐标
    float gl_y = WINDOW_HEIGHT - y;

    // ===================================
    // === 1. 处理按下 (GLUT_DOWN) ===
    // ===================================
    if (state == GLUT_DOWN) {

        // --- (新!) 检查是否按下了邮戳 ---
        // (使用勾股定理计算点到圆心的距离)
        float dx = x - stampCX;
        float dy = gl_y - stampCY;
        if ((dx * dx + dy * dy < stampRadius * stampRadius) && currentScene == SCENE_COVER)
        {
            isStampPressed = true;
            glutPostRedisplay();
        }

        // --- 检查是否按下了信封盖子 ---
        // (注意：使用 else if 来防止同时触发)
        else if (currentScene == SCENE_COVER)
        {
            // (这是您之前的命中检测逻辑)
            float tipX = WINDOW_WIDTH / 2.0;
            if (x > tipX - 50 && x < tipX + 50 &&
                gl_y > envelopeFlapY - 30 && gl_y < envelopeFlapY + 30)
            {
                isDraggingFlap = true;
            }
        }
    }

    // ===================================
    // === 2. 处理松开 (GLUT_UP) ===
    // ===================================
    else if (state == GLUT_UP) {

        // --- (新!) 检查是否松开了邮戳 ---
        if (isStampPressed) {
            isStampPressed = false;
            glutPostRedisplay();
        }

        // --- 检查是否松开了信封盖子 ---
        if (isDraggingFlap) {
            isDraggingFlap = false;

            // 检查“盖子”是否被拖拽到了“打开”的位置
            if (envelopeFlapY > flapOpenThresholdY) {
                currentScene = SCENE_LANDSCAPE;
            }
            else {
                envelopeFlapY = flapStartY;
            }
            glutPostRedisplay();
        }
    }
}

/**
 * 鼠标拖动回调
 */
void motion(int x, int y) {
    // 如果正在拖动盖子
    if (isDraggingFlap) {
        // 转换 Y 坐标
        float y_gl = WINDOW_HEIGHT - y;

        // 更新盖子的Y坐标
        envelopeFlapY = y_gl;

        // 限制拖动范围
        if (envelopeFlapY < flapStartY) { // 不能低于起始位置
            envelopeFlapY = flapStartY;
        }
        if (envelopeFlapY > flapMaxY) { // 不能高于最大位置
            envelopeFlapY = flapMaxY;
        }

        glutPostRedisplay(); // 拖动时强制重绘
    }
}

/**
 * 键盘回调
 */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case ' ': // 按下空格键
        if (currentScene == SCENE_LANDSCAPE) {
            currentScene = SCENE_CAKE;
        }
        break;
    case 'r': // 按下 'r' 键
    case 'R':
        if (currentScene == SCENE_CAKE) {
            currentScene = SCENE_COVER;
            // !! 重置场景一的状态 !!
            envelopeFlapY = flapStartY;
            isDraggingFlap = false;
        }
        break;
    case 27: // 'ESC' 键
        exit(0);
        break;
    }
    glutPostRedisplay();
}

/**
 * @brief (新!) 鼠标被动移动回调 (未按下时)
 * 用于检测是否悬停在热点上
 */
void passiveMotion(int x, int y)
{
    // 仅在场景一时检测
    if (currentScene != SCENE_COVER) {
        // 确保在其他场景，高亮状态被重置
        if (isHoveringHandle) {
            isHoveringHandle = false;
            glutPostRedisplay(); // 如果状态改变了，就重绘
        }
        return;
    }

    // 将 GLUT 的Y坐标 (左上角为0) 转换为 OpenGL 的Y坐标 (左下角为0)
    float gl_y = WINDOW_HEIGHT - y;

    // --- 定义 "Drag to Open" 交互的热区 (Hotspot) ---
    float centerX = WINDOW_WIDTH / 2.0f;
    float handleWidth = 140.0f; // “Drag to Open”的宽度 (估算值，可以大一点)
    float handleBottomY = envelopeFlapY - 35.0f; // 文本下方
    float handleTopY = envelopeFlapY + 15.0f; // 箭头上S方

    // 检查鼠标 (x, gl_y) 是否在热区矩形内
    bool isInside = (x > (centerX - handleWidth / 2.0f)) &&
        (x < (centerX + handleWidth / 2.0f)) &&
        (gl_y > handleBottomY) &&
        (gl_y < handleTopY);

    // 优化：只有在悬停状态 *改变* 时才重绘
    if (isInside != isHoveringHandle) {
        isHoveringHandle = isInside;
        glutPostRedisplay(); // 状态改变，请求重绘
    }
}

// --- 6. Main 函数 ---

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("XJTLU 20th Anniversary Greeting Card");

    // --- 初始化场景一的变量 ---
    envelopeFlapY = flapStartY;

    // --- 注册回调 ---
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glutPassiveMotionFunc(passiveMotion); // <--- 添加这一行

    glutMainLoop();
    return 0;
}