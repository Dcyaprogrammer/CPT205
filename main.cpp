#include <GL/freeglut.h>
#include <string>
#include <vector> // 仅用于 getStrokeTextWidth

// --- 1. 全局变量 ---
const float WINDOW_WIDTH = 800.0;
const float WINDOW_HEIGHT = 600.0;

// --- 2. 矢量字体辅助函数 ---

/**
 * @brief 辅助函数：计算矢量字体的宽度
 * (使用您选择的 索引for循环 方式)
 */
float getStrokeTextWidth(void* font, const std::string& s) {
    float width = 0.0f;
    for (size_t i = 0; i < s.length(); ++i) {
        width += glutStrokeWidth(font, static_cast<unsigned char>(s[i]));
    }
    return width;
}

/**
 * @brief [矢量字体版] 在屏幕上“水平居中”绘制文本
 */
void drawTextStrokeCentered(float x_center, float y_baseline, const std::string& text, float scale, float stroke_width) {
    void* font = GLUT_STROKE_ROMAN; // 使用标准罗马字体
    
    // 1. 计算文本在缩放前的原始总宽度
    float textWidth = getStrokeTextWidth(font, text);

    // 2. 计算在应用缩放后，我们的起始X坐标
    // (中心点 - (总宽度 * 缩放 / 2.0))
    float startX = x_center - (textWidth * scale / 2.0f);

    // 3. 隔离变换：保存当前矩阵
    glPushMatrix();

    // 4. 设置线宽和颜色
    glLineWidth(stroke_width);
    glColor3f(0.1, 0.1, 0.1); // 深灰色

    // 5. 进行变换：
    //    a. 先平移到我们计算好的 (startX, y_baseline)
    //    b. 再以这个新原点为中心进行缩放
    glTranslatef(startX, y_baseline, 0.0f);
    glScalef(scale, scale, scale);

    // 6. 逐个绘制字符 (现在是在缩放后的坐标系中)
    for (char ch : text) {
        glutStrokeCharacter(font, ch);
    }

    // 7. 恢复变换和线宽
    glPopMatrix();      // 恢复矩阵
    glLineWidth(1.0f); // 恢复默认线宽
}

// --- 3. 简化的 Display 函数 ---
/**
 * 这是我们唯一的绘制函数
 * 它只做三件事：清屏、设置坐标系、绘制文本
 */
void display(void) {
    
    // 1. 将清除颜色（背景）设为信封的米色
    glClearColor(0.95, 0.9, 0.85, 1.0); 
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

    // --- 4. 立即绘制文本 ---
    
    // 绘制标题 (中心X, 基线Y, 文本, 缩放, 线粗)
    drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 300.0f, "XJTLU 20th Anniversary", 0.22f, 2.0f);
    drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 270.0f, "A Greeting Card", 0.15f, 2.0f);

    // 为了测试，我们把 "Drag to Open" 放在上方
    float flapTextY = 400.0f;
    drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, flapTextY, "Drag to Open", 0.15f, 2.0f);

    glutSwapBuffers();
}

// --- 4. Main 函数 (和空的回调) ---

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0); // 按 ESC 键退出
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    
    // 确保窗口大小和全局变量一致
    glutInitWindowSize(static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT));
    glutInitWindowPosition(100, 100);
    glutCreateWindow("XJTLU - Text Centering Test");

    glutDisplayFunc(display);     // 注册 display
    glutKeyboardFunc(keyboard); // 注册键盘 (用于退出)

    glutMainLoop();
    return 0;
}

