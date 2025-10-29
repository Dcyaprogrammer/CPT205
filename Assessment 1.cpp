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
bool isHoveringHandle = false; // <--- �����һ�У����ڸ���
void* evelopeFont = GLUT_STROKE_MONO_ROMAN;

bool isStampPressed = false; // �����ʴ��Ƿ�������ס��
const float stampCX = WINDOW_WIDTH - 100.0f; // ���Ͻ� X
const float stampCY = 500.0f - 100.0f;        // (�ŷⶥ�� - 100)
const float stampRadius = 40.0f;             // �ʴ��뾶

// utils
float getStrokeTextWidth(void* font, const std::string& s) {
    float width = 0.0f;
    for (char ch : s) { // C++11 ����ѭ��
        width += glutStrokeWidth(font, ch);
    }
    return width;
}


// Drawing Funcs
void drawTextStrokeCentered(float x_center, float y_baseline, const std::string& text, 
                            float scale, float stroke_width, const Color& color) {
    void* font = evelopeFont; // �����ѡ�� GLUT_STROKE_MONO_ROMAN

    // 1. �����ı�������ǰ��ԭʼ�ܿ��
    float textWidth = getStrokeTextWidth(font, text);

    // 2. ������Ӧ�����ź�������Ҫƽ�ƶ���
    // (���ĵ� - (�ܿ�� * ���� / 2.0))
    float startX = x_center - (textWidth * scale / 2.0f);

    // 3. ����任
    glPushMatrix();

    // 4. �����߿�
    glLineWidth(stroke_width);
    
    glColor3f(color.r, color.g, color.b);

    // 5. ���б任����ƽ�Ƶ� (startX, y_baseline)���ٽ�������
    glTranslatef(startX, y_baseline, 0.0f);
    glScalef(scale, scale, scale);

    // 6. ��������ַ�
    for (char ch : text) {
        glutStrokeCharacter(font, ch);
    }

    // 7. �ָ��任���߿�
    glPopMatrix();
    glLineWidth(1.0f); // �ָ�Ĭ���߿�
}

void drawFilledEllipse(float rx, float ry) {
    int segments = 60; // 60����������������������ƽ��
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        glVertex2f(rx * cosf(theta), ry * sinf(theta));
    }
    glEnd();
}

void drawEnvelope(float left, float right, float bottom, float top, float centerX, float envelopeFlapY)
{
    // �����ŷ����� (��ɫ)
    glColor3f(COLOR_XJTLU_BLUE.r, COLOR_XJTLU_BLUE.g, COLOR_XJTLU_BLUE.b);
    glBegin(GL_POLYGON);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
    glVertex2f(left, top);
    glEnd();

    // �����۵��� (����ɫ)
    glColor3f(COLOR_XJTLU_BLUE_DARK.r, COLOR_XJTLU_BLUE_DARK.g, COLOR_XJTLU_BLUE_DARK.b);
    glBegin(GL_POLYGON); // �¸�
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right - 100, (top + bottom) / 2.0);
    glVertex2f(left + 100, (top + bottom) / 2.0);
    glEnd();
    glBegin(GL_POLYGON); // ���
    glVertex2f(left, bottom);
    glVertex2f(left, top);
    glVertex2f(left + 150, (top + bottom) / 2.0);
    glEnd();
    glBegin(GL_POLYGON); // �Ҹ�
    glVertex2f(right, bottom);
    glVertex2f(right, top);
    glVertex2f(right - 150, (top + bottom) / 2.0);
    glEnd();

    // ���ƶ��� (��ɫ)
    glColor3f(COLOR_XJTLU_BLUE.r, COLOR_XJTLU_BLUE.g, COLOR_XJTLU_BLUE.b);
    glBegin(GL_TRIANGLES);
    glVertex2f(left, top);
    glVertex2f(right, top);
    glVertex2f(centerX, envelopeFlapY);
    glEnd();

    // ���ƶ������ (��ɫ)
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
    // ������ʾ���� (�ڸ����ϣ�λ�ò���)
    drawTextStrokeCentered(centerX, envelopeFlapY - 25.0f, "Drag to Open", 0.15f, 2.0f, color);

    // ����ʾ�����Ϸ�����һ�����ϵ�С��ͷ
    glPushMatrix();
    glTranslatef(centerX, envelopeFlapY - 5.0f, 0.0f); // ��λ�������Ϸ�
    glLineWidth(2.0f);
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_LINE_STRIP); // ��������һ�� ^ ����
    glVertex2f(-10.0f, 0.0f);
    glVertex2f(0.0f, 10.0f);
    glVertex2f(10.0f, 0.0f);
    glEnd();
    glPopMatrix();
    glLineWidth(1.0f); // �ָ�Ĭ���߿�
}

void drawStamp()
{
    // 1. ����״̬������ɫ
    Color circleColor;
    Color textColor;

    if (isStampPressed) {
        // ����ʱ����ɫ�ף���ɫ��
        circleColor = COLOR_XJTLU_BLUE;
        textColor = COLOR_WHITE;
    }
    else {
        // ����ʱ����ɫ�ף���ɫ��
        circleColor = COLOR_WHITE;
        textColor = COLOR_XJTLU_BLUE;
    }

    // 2. �����ʴ��İ�ɫ/��ɫ��
    glPushMatrix();
    glTranslatef(stampCX, stampCY, 0.0f);
    glColor3f(circleColor.r, circleColor.g, circleColor.b);
    drawFilledEllipse(stampRadius, stampRadius); // (�������� drawFilledEllipse)
    glPopMatrix();

    // 3. �����ʴ��ϵ� "20"
    drawTextStrokeCentered(stampCX, stampCY - 15.0f, "20", 0.3f, 3.0f, textColor);
}

void drawBear(float cx, float cy, float scale, const Color& color) {
    glPushMatrix();
    glTranslatef(cx, cy, 0.0f); // 1. �ƶ���Ŀ�����ĵ�
    glScalef(scale, scale, 1.0f); // 2. ��������

    glColor3f(color.r, color.g, color.b); // 3. ������ɫ (һ����)

    // 4. ���Ƹ������� (�������궼����Ե�)
    // (�����Ȼ����壬�ٻ�ͷ����󻭶��䣬��������ȷ���ڵ�)

    // ���� (һ����Ĵ�ֱ��Բ)
    glPushMatrix();
    glTranslatef(0.0f, -0.1f, 0.0f); // λ�������ĵ��·�
    drawFilledEllipse(0.5f, 0.6f); // ��Բ�뾶 (��0.5, ��0.6)
    glPopMatrix();

    // ͷ (һ��Բ)
    glPushMatrix();
    glTranslatef(0.0f, 0.6f, 0.0f); // λ���������Ϸ�
    drawFilledEllipse(0.4f, 0.4f); // �뾶 (0.4)
    glPopMatrix();

    // ���� (����СԲ)
    glPushMatrix();
    glTranslatef(-0.3f, 0.9f, 0.0f); // ���
    drawFilledEllipse(0.15f, 0.15f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.3f, 0.9f, 0.0f); // �Ҷ�
    drawFilledEllipse(0.15f, 0.15f);
    glPopMatrix();

    glPopMatrix(); // �ָ�ԭʼ����
}

void drawBird(float cx, float cy, float scale, const Color& color) {
    glPushMatrix();
    glTranslatef(cx, cy, 0.0f); // 1. �ƶ���Ŀ�����ĵ�
    glScalef(scale, scale, 1.0f); // 2. ��������

    glColor3f(color.r, color.g, color.b); // 3. ������ɫ

    // 1. ���� (һ����ת����Բ)
    glPushMatrix();
    glRotatef(30.0f, 0.0f, 0.0f, 1.0f); // ������ת30��
    drawFilledEllipse(0.7f, 0.4f); // ������Բ
    glPopMatrix();

    // 2. ͷ (һ��Բ)
    glPushMatrix();
    glTranslatef(0.4f, 0.5f, 0.0f); // �����������Ϸ�
    drawFilledEllipse(0.2f, 0.2f);
    glPopMatrix();

    // 3. ��� (һ��������)
    glPushMatrix();
    glTranslatef(0.55f, 0.5f, 0.0f); // ��ͷ���Ŀ�ʼ
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.05f);
    glVertex2f(0.0f, -0.05f);
    glVertex2f(0.2f, 0.0f); // ๼�
    glEnd();
    glPopMatrix();

    // 4. β�� (һ��������)
    glPushMatrix();
    glTranslatef(-0.6f, -0.2f, 0.0f); // �������˿�ʼ
    glRotatef(-15.0f, 0.0f, 0.0f, 1.0f); // β�ͽǶ�
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.15f);
    glVertex2f(0.0f, -0.15f);
    glVertex2f(-0.4f, 0.0f); // β��
    glEnd();
    glPopMatrix();

    glPopMatrix(); // �ָ�ԭʼ����
}

// Scene1: Cover
/**
 * @brief [���ع�] ���Ƴ���һ������
 */
void drawSceneCover() {
    // --- 1. �������� ---
    float left = 50.0, right = 750.0, bottom = 50.0, top = 500.0;
    float centerX = WINDOW_WIDTH / 2.0;

    // --- 2. �����ŷ� ---
    // (�ѷ�װ�������º���)
    drawEnvelope(left, right, bottom, top, centerX, envelopeFlapY);

    // --- 3. ���Ʊ��� ---
    // (�ⲿ���߼��Ѿ������������ֲ���)
    drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 270.0f, "XJTLU 20th Anniversary", 0.22f, 2.0f, COLOR_WHITE);
    drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 240.0f, "A Greeting Card", 0.15f, 2.0f, COLOR_WHITE);

    // --- 4. ���ƽ�����ʾ ---
    // (�ѷ�װ�������º���)
    Color handleColor = COLOR_WHITE;
    if (isHoveringHandle) {
        handleColor = COLOR_GREY;
    }
    drawDragHandle(centerX, envelopeFlapY, handleColor);

    drawStamp();

    // --- 5. ���Ƽ����� ---
    // (�ⲿ���߼��Ѿ������������ֲ���)
    float mascotCenterY = 100.0f;
    float mascotScale = 30.0f;
    float bearCenterX = 120.0f;
    drawBear(bearCenterX, mascotCenterY, mascotScale, COLOR_WHITE);

    float birdCenterX = WINDOW_WIDTH - 120.0f;
    drawBird(birdCenterX, mascotCenterY, mascotScale, COLOR_WHITE);
}



// Scene2: Landscape
void drawSceneLandscape() {
    // ������������У԰����
    // ...

    // �������
    glColor3f(0.5, 0.8, 1.0); // ����ɫ
    glBegin(GL_POLYGON);
    glVertex2f(0, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();

    // ����һ����ѧ¥ (ʾ��)
    glColor3f(0.9, 0.8, 0.7); // ��ɫ
    glBegin(GL_POLYGON);
    glVertex2f(200, 50);
    glVertex2f(600, 50);
    glVertex2f(600, 400);
    glVertex2f(200, 400);
    glEnd();

    glColor3f(0.5, 0.8, 1.0); // ����ɫ
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
    // �����������ĵ���
    // ...

    // ���Ƶ���ײ�
    glColor3f(0.8, 0.4, 0.2); // ��ɫ
    glBegin(GL_POLYGON);
    glVertex2f(300, 100);
    glVertex2f(500, 100);
    glVertex2f(500, 200);
    glVertex2f(300, 200);
    glEnd();

    // ���Ƶ��ⶥ��
    glColor3f(1.0, 0.8, 0.8); // ��ɫ
    glBegin(GL_POLYGON);
    glVertex2f(325, 200);
    glVertex2f(475, 200);
    glVertex2f(475, 275);
    glVertex2f(325, 275);
    glEnd();

    // �������� (ʾ��)
    glColor3f(1.0, 1.0, 0.0); // ��ɫ
    glBegin(GL_POLYGON);
    glVertex2f(395, 275);
    glVertex2f(405, 275);
    glVertex2f(405, 325);
    glVertex2f(395, 325);
    glEnd();

    glColor3f(1.0, 0.8, 0.8); // ��ɫ
    glBegin(GL_POLYGON);
    glVertex2f(0, 0); glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT); glVertex2f(0, WINDOW_HEIGHT);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    //drawTextLarge(250, 300, "Scene 3: Happy 20th!");
    //drawTextSmall(300, 250, "Press 'r' to restart");
}


// Display
// --- �滻����Ŀ��ԭ���� 'display' ���� ---
void display(void) {
    // 1. ���������ɫ (�����Ըĳ���ϲ���ı���ɫ)
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // 2. ÿ�ζ�����ͶӰ����
    // �⽨���� (0, 800) (0, 600) ����Ļ����ϵ
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    // 3. ÿ�ζ�����ģ����ͼ����
    // ��ȷ�����Ǵ� (0,0) �㿪ʼ��û�ж����ƽ��
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 4. --- ����·���߼� ---
    //    �������ǿ��԰�ȫ�ص��û��ƺ�����
    switch (currentScene) {
    case SCENE_COVER:
        drawSceneCover();
        break;
    case SCENE_LANDSCAPE:
        drawSceneLandscape(); // (����������ռλ��)
        break;
    case SCENE_CAKE:
        drawSceneCake();      // (����������ռλ��)
        break;
    }

    glutSwapBuffers();
}
// --- 5. �����ص����� (����) ---

/**
 * ������ص�
 */
 /**
  * @brief ������ص� (�Ѹ���)
  */
void mouse(int button, int state, int x, int y)
{
    // --- ��������� ---
    if (button != GLUT_LEFT_BUTTON) {
        return;
    }

    // �� GLUT ��Y����ת��Ϊ OpenGL ����
    float gl_y = WINDOW_HEIGHT - y;

    // ===================================
    // === 1. ������ (GLUT_DOWN) ===
    // ===================================
    if (state == GLUT_DOWN) {

        // --- (��!) ����Ƿ������ʴ� ---
        // (ʹ�ù��ɶ������㵽Բ�ĵľ���)
        float dx = x - stampCX;
        float dy = gl_y - stampCY;
        if ((dx * dx + dy * dy < stampRadius * stampRadius) && currentScene == SCENE_COVER)
        {
            isStampPressed = true;
            glutPostRedisplay();
        }

        // --- ����Ƿ������ŷ���� ---
        // (ע�⣺ʹ�� else if ����ֹͬʱ����)
        else if (currentScene == SCENE_COVER)
        {
            // (������֮ǰ�����м���߼�)
            float tipX = WINDOW_WIDTH / 2.0;
            if (x > tipX - 50 && x < tipX + 50 &&
                gl_y > envelopeFlapY - 30 && gl_y < envelopeFlapY + 30)
            {
                isDraggingFlap = true;
            }
        }
    }

    // ===================================
    // === 2. �����ɿ� (GLUT_UP) ===
    // ===================================
    else if (state == GLUT_UP) {

        // --- (��!) ����Ƿ��ɿ����ʴ� ---
        if (isStampPressed) {
            isStampPressed = false;
            glutPostRedisplay();
        }

        // --- ����Ƿ��ɿ����ŷ���� ---
        if (isDraggingFlap) {
            isDraggingFlap = false;

            // ��顰���ӡ��Ƿ���ק���ˡ��򿪡���λ��
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
 * ����϶��ص�
 */
void motion(int x, int y) {
    // ��������϶�����
    if (isDraggingFlap) {
        // ת�� Y ����
        float y_gl = WINDOW_HEIGHT - y;

        // ���¸��ӵ�Y����
        envelopeFlapY = y_gl;

        // �����϶���Χ
        if (envelopeFlapY < flapStartY) { // ���ܵ�����ʼλ��
            envelopeFlapY = flapStartY;
        }
        if (envelopeFlapY > flapMaxY) { // ���ܸ������λ��
            envelopeFlapY = flapMaxY;
        }

        glutPostRedisplay(); // �϶�ʱǿ���ػ�
    }
}

/**
 * ���̻ص�
 */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case ' ': // ���¿ո��
        if (currentScene == SCENE_LANDSCAPE) {
            currentScene = SCENE_CAKE;
        }
        break;
    case 'r': // ���� 'r' ��
    case 'R':
        if (currentScene == SCENE_CAKE) {
            currentScene = SCENE_COVER;
            // !! ���ó���һ��״̬ !!
            envelopeFlapY = flapStartY;
            isDraggingFlap = false;
        }
        break;
    case 27: // 'ESC' ��
        exit(0);
        break;
    }
    glutPostRedisplay();
}

/**
 * @brief (��!) ��걻���ƶ��ص� (δ����ʱ)
 * ���ڼ���Ƿ���ͣ���ȵ���
 */
void passiveMotion(int x, int y)
{
    // ���ڳ���һʱ���
    if (currentScene != SCENE_COVER) {
        // ȷ������������������״̬������
        if (isHoveringHandle) {
            isHoveringHandle = false;
            glutPostRedisplay(); // ���״̬�ı��ˣ����ػ�
        }
        return;
    }

    // �� GLUT ��Y���� (���Ͻ�Ϊ0) ת��Ϊ OpenGL ��Y���� (���½�Ϊ0)
    float gl_y = WINDOW_HEIGHT - y;

    // --- ���� "Drag to Open" ���������� (Hotspot) ---
    float centerX = WINDOW_WIDTH / 2.0f;
    float handleWidth = 140.0f; // ��Drag to Open���Ŀ�� (����ֵ�����Դ�һ��)
    float handleBottomY = envelopeFlapY - 35.0f; // �ı��·�
    float handleTopY = envelopeFlapY + 15.0f; // ��ͷ��S��

    // ������ (x, gl_y) �Ƿ�������������
    bool isInside = (x > (centerX - handleWidth / 2.0f)) &&
        (x < (centerX + handleWidth / 2.0f)) &&
        (gl_y > handleBottomY) &&
        (gl_y < handleTopY);

    // �Ż���ֻ������ͣ״̬ *�ı�* ʱ���ػ�
    if (isInside != isHoveringHandle) {
        isHoveringHandle = isInside;
        glutPostRedisplay(); // ״̬�ı䣬�����ػ�
    }
}

// --- 6. Main ���� ---

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("XJTLU 20th Anniversary Greeting Card");

    // --- ��ʼ������һ�ı��� ---
    envelopeFlapY = flapStartY;

    // --- ע��ص� ---
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glutPassiveMotionFunc(passiveMotion); // <--- �����һ��

    glutMainLoop();
    return 0;
}