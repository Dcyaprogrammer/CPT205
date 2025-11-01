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
#include <cmath>
#include <cstdlib>
#include <ctime>

// three scenes
enum Scene {
    SCENE_COVER,   
    SCENE_LANDSCAPE, 
    SCENE_BRIDGE        
};
Scene currentScene = SCENE_COVER;

enum Season {
    SEASON_SPRING,
    SEASON_SUMMER,
    SEASON_AUTUMN,
    SEASON_WINTER
};
Season currentSeason = SEASON_SPRING;

enum Scene3State {
    S3_INITIAL, // 初始状态 (粒子正在汇聚, 牌子是分开的)
    S3_MERGING, // "c" 已按下 (牌子正在移动)
    S3_MERGED   // 动画结束 (显示 "XJTLU")
};
Scene3State scene3State = S3_INITIAL; // 默认状态

// colors
struct Color {
    float r, g, b;
};
const Color COLOR_WHITE = { 1.0f, 1.0f, 1.0f };
const Color COLOR_XJTLU_BLUE = { 0.0f, 0.0f, 0.5f };
const Color COLOR_XJTLU_BLUE_DARK = { 0.0f, 0.0f, 0.4f };
const Color COLOR_GREY = { 0.7f, 0.7f, 0.7f };
const Color COLOR_BLACK = { 0.1f, 0.1f, 0.1f };
const Color COLOR_BUILDING_GREY = { 0.3f, 0.3f, 0.35f }; // (新!) 建筑剪影
const Color COLOR_GLASS_BLUE = { 0.2f, 0.4f, 0.8f }; // (新!) 偏蓝的玻璃色
const Color COLOR_DARK_BLUE = { 0.1f, 0.1f, 0.4f }; // (新!) 建筑主体颜色
const Color COLOR_SUN_YELLOW = { 1.0f, 0.9f, 0.2f }; // (新!) 太阳黄

const Color COLOR_SKY_BLUE = { 0.5f, 0.8f, 1.0f }; // (新!) 天空蓝

// --- (新!) 渐变天空的颜色 ---
const Color COLOR_SKY_SPRING_TOP = { 0.6f, 0.85f, 1.0f }; // 春天: 天顶 (浅蓝)
const Color COLOR_SKY_SPRING_HORIZON = { 0.8f, 0.95f, 1.0f }; // 春天: 地平线 (更亮)

const Color COLOR_SKY_SUMMER_TOP = { 0.3f, 0.7f, 1.0f }; // 夏天: 天顶 (深蓝)
const Color COLOR_SKY_SUMMER_HORIZON = { 0.6f, 0.85f, 1.0f }; // 夏天: 地平线 (稍亮)

const Color COLOR_SKY_AUTUMN_TOP = { 0.7f, 0.8f, 0.9f }; // 秋天: 天顶 (灰)
const Color COLOR_SKY_AUTUMN_HORIZON = { 0.9f, 0.9f, 0.9f }; // 秋天: 地平线 (亮灰)

const Color COLOR_SKY_WINTER_TOP = { 0.8f, 0.9f, 0.95f }; // 冬天: 天顶 (浅灰)
const Color COLOR_SKY_WINTER_HORIZON = { 1.0f, 1.0f, 1.0f }; // 冬天: 地平线 (白色)

// --- (新!) 草地/地面图层 ---
// (您现有的 COLOR_GRASS_GREEN, COLOR_GROUND_AUTUMN 等可以被这些替换)
const Color COLOR_GROUND_SPRING_DARK = { 0.1f, 0.5f, 0.1f };
const Color COLOR_GROUND_SPRING_MID = { 0.2f, 0.6f, 0.2f };
const Color COLOR_GROUND_SPRING_LIGHT = { 0.3f, 0.7f, 0.3f };

const Color COLOR_GROUND_SUMMER_DARK = { 0.1f, 0.6f, 0.1f }; // 更鲜艳
const Color COLOR_GROUND_SUMMER_MID = { 0.2f, 0.7f, 0.2f };
const Color COLOR_GROUND_SUMMER_LIGHT = { 0.3f, 0.8f, 0.3f };

const Color COLOR_GROUND_AUTUMN_DARK = { 0.7f, 0.4f, 0.1f }; // 棕
const Color COLOR_GROUND_AUTUMN_MID = { 0.8f, 0.5f, 0.1f };
const Color COLOR_GROUND_AUTUMN_LIGHT = { 0.9f, 0.6f, 0.2f }; // 橙

const Color COLOR_GROUND_WINTER_DARK = { 0.9f, 0.9f, 1.0f }; // 雪的阴影 (淡蓝)
const Color COLOR_GROUND_WINTER_MID = { 1.0f, 1.0f, 1.0f }; // 雪 (白色)
const Color COLOR_GROUND_WINTER_LIGHT = { 1.0f, 1.0f, 1.0f }; // 雪 (白色)

// --- (新!) 树干渐变 ---
const Color COLOR_TRUNK_LIGHT = { 0.5f, 0.3f, 0.2f }; // 树干亮部 (例如，受光面)
const Color COLOR_TRUNK_DARK = { 0.3f, 0.15f, 0.1f }; // 树干暗部 (例如，背光面)

// --- (新!) 树叶图层 (春/夏/秋) ---
const Color COLOR_LEAF_SPRING_DARK = { 0.3f, 0.7f, 0.3f };  // 春天: 深绿
const Color COLOR_LEAF_SPRING_MID = { 0.4f, 0.8f, 0.4f };   // 春天: 中绿
const Color COLOR_LEAF_SPRING_LIGHT = { 0.5f, 0.9f, 0.5f }; // 春天: 亮绿 (新芽感)

const Color COLOR_LEAF_SUMMER_DARK = { 0.1f, 0.5f, 0.1f };  // 夏天: 深绿色 (茂盛)
const Color COLOR_LEAF_SUMMER_MID = { 0.2f, 0.6f, 0.2f };   // 夏天: 中绿色
const Color COLOR_LEAF_SUMMER_LIGHT = { 0.3f, 0.7f, 0.3f }; // 夏天: 亮绿色

const Color COLOR_LEAF_AUTUMN_DARK = { 0.8f, 0.3f, 0.0f };  // 秋天: 暗橙 (底层)
const Color COLOR_LEAF_AUTUMN_MID = { 1.0f, 0.5f, 0.1f };   // 秋天: 亮橙 (中层)
const Color COLOR_LEAF_AUTUMN_LIGHT = { 1.0f, 0.8f, 0.2f }; // 秋天: 黄 (顶层)

// (别忘了您现有的 COLOR_FLOWER_PINK 用于春天的小花)
const Color COLOR_FLOWER_PINK = { 1.0f, 0.7f, 0.8f };

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

// scene2
struct Balloon {
    float x, y;
    float speed;
    Color color;
};
const int NUM_BALLOONS = 7; // (您可以调整气球的数量)
Balloon balloons[NUM_BALLOONS];

struct Snowflake {
    float x, y;
    float speed;
    float size;
};
const int NUM_SNOWFLAKES = 150; // (您可以调整数量)
Snowflake snowflakes[NUM_SNOWFLAKES];

// --- (新!) 风的全局变量 ---
struct WindLine {
    float x, y;
    float speed_x;     // (从 speed 重命名)
    float speed_y;     // (新!) 垂直速度，用于倾斜的轨迹
    float length;      // (新!) 当前长度，将动态变化
    float base_length; // (新!) 基础长度，用于计算
};
const int NUM_WIND_LINES = 20; // (您可以调整数量)
WindLine windLines[NUM_WIND_LINES];

// --- (新!) 小猫的全局变量 ---
struct Cat {
    float x, y;          // 小猫的中心坐标
    float speed;         // 行走速度
    float animationTime; // 动画计时器，用于腿部摆动
    bool isWalking;      // 是否正在行走
    // (您可以根据需要添加其他如颜色、大小等)
};
Cat scene2Cat; // 场景2中的小猫实例
// (新!) 小猫的颜色 (可根据季节变化)
const Color COLOR_CAT_BODY = { 0.3f, 0.3f, 0.3f }; // 深灰色/黑色
const Color COLOR_CAT_PAW = { 0.4f, 0.4f, 0.4f };  // 爪子颜色
const Color COLOR_CAT_EAR = { 0.2f, 0.2f, 0.2f };  // 耳朵颜色
const Color COLOR_CAT_EYE = { 0.9f, 0.9f, 0.1f };  // 眼睛颜色 (黄)


// scene3
struct Particle {
    float x, y;             // 粒子 *当前* 的 x, y
    float startX, startY;   // 它的出生点
    float targetX, targetY; // 它的最终目标点 (在 "20" 上)

    float t;     // 动画计时器 (0.0 to 1.0)
    float speed; // 动画速度
    float delay; // (新!) 启动延迟，让粒子 staggered
    Color color;
};
// (这个值现在很关键，它将决定 "20" 的粒子密度)
const int NUM_PARTICLES_S3 = 100; // 保持 100
Particle particles[NUM_PARTICLES_S3];

// --- (已修改!) 五彩纸屑的全局变量 ---
struct Confetti {
    float x, y;
    float speed;
    float width;  // <--- (新!) 宽度
    float height; // <--- (新!) 高度
    Color color;
    float angle;  // <--- (新!) 当前旋转角度
    float rotationSpeed; // <--- (新!) 旋转速度
};

const int NUM_CONFETTI = 150;
Confetti confetti[NUM_CONFETTI];

struct Star {
    float x, y;
    float size;
    float phase; // (新!) 用于控制每个星星的闪烁周期
};
const int NUM_STARS = 200; // (您可以调整数量)
Star stars[NUM_STARS];

float mergeProgress = 0.0f; // 动画计时器 (0.0 = 分开, 1.0 = 融合)
const float signWidth = 180.0f;
const float signHeight = 60.0f;
const float signY = 150.0f;
const float signStartX_L = 100.0f;                 // 左牌子起始X
const float signStartX_R = WINDOW_WIDTH - 100.0f;  // 右牌子起始X
const float signTargetX = WINDOW_WIDTH / 2.0f;   // 两个牌子的 *共同* 目标X

// utils
float getStrokeTextWidth(void* font, const std::string& s) {
    float width = 0.0f;
    for (char ch : s) { // C++11 风格的循环
        width += glutStrokeWidth(font, ch);
    }
    return width;
}

void initBalloons() {
    Color balloonColors[] = {
        {1.0f, 0.2f, 0.2f}, // 红
        {0.2f, 0.5f, 1.0f}, // 蓝
        {1.0f, 1.0f, 0.2f}, // 黄
        {1.0f, 0.5f, 0.0f}, // 橙
        {0.8f, 0.3f, 1.0f}  // 紫
    };
    int numColors = 5;

    for (int i = 0; i < NUM_BALLOONS; ++i) {
        balloons[i].x = (rand() % (int)WINDOW_WIDTH); // 随机 X
        balloons[i].y = -50.0f - (rand() % 100);     // 随机 Y (从屏幕下方开始)
        balloons[i].speed = 0.5f + (rand() / (float)RAND_MAX) * 1.0f; // 随机速度
        balloons[i].color = balloonColors[rand() % numColors];
    }
}

void initSnowflakes() {
    for (int i = 0; i < NUM_SNOWFLAKES; ++i) {
        snowflakes[i].x = (rand() % (int)WINDOW_WIDTH);
        snowflakes[i].y = (rand() % (int)WINDOW_HEIGHT); // 随机Y
        snowflakes[i].speed = 0.5f + (rand() / (float)RAND_MAX) * 1.0f; // 随机速度
        snowflakes[i].size = 1.0f + (rand() / (float)RAND_MAX) * 2.0f; // 随机大小
    }
}

/**
 * @brief (新!) 初始化/重置所有风线的状态
 */
void initWindLines() {
    for (int i = 0; i < NUM_WIND_LINES; ++i) {
        windLines[i].x = -50.0f - (rand() % 100);
        windLines[i].y = (rand() % (int)WINDOW_HEIGHT);
        windLines[i].speed_x = 1.0f + (rand() / (float)RAND_MAX) * 2.0f;

        // (新!) 赋予一个小的、随机的垂直速度，使轨迹倾斜
        windLines[i].speed_y = (rand() / (float)RAND_MAX - 0.5f) * 0.5f; // 值在 -0.25 到 +0.25 之间

        windLines[i].base_length = 15.0f + (rand() / (float)RAND_MAX) * 15.0f;
        windLines[i].length = windLines[i].base_length;
    }
}

/**
 * @brief (新!) 初始化小猫的状态
 */
 /**
  * @brief (已修正!) 初始化小猫的状态
  */
void initCat() {
    scene2Cat.x = -50.0f; // 从屏幕左侧外开始

    // (*** 关键修复 ***)
    // 80.0f 是错的，草地/地面的顶部是 150.0f
    scene2Cat.y = 150.0f; // 在草地上行走的高度 (Y=150)

    scene2Cat.speed = 1.0f;
    scene2Cat.animationTime = 0.0f;
    scene2Cat.isWalking = true;
}

/**
 * @brief (已重写!) 初始化粒子
 * (为每个粒子分配一个 "20" 上的随机目标点)
 */
void initParticles() {
    Color particleColors[] = {
        COLOR_WHITE, COLOR_SKY_BLUE, {1.0f, 1.0f, 0.2f}
    };
    int numColors = 3;

    // --- "20" 的坐标 (基于 drawPixel20) ---
    float s = 8.0f; // 像素块大小
    float cx = WINDOW_WIDTH / 2.0f;
    float cy = 500.0f;
    float x2 = cx - s * 5;
    float x0 = cx + s;

    // (*** 已修正：现在有 11 个矩形 ***)
    float quads[11][4] = {
        // "2" (5 个矩形)
        {x2, cy - s, s * 3, s},      // 0: 顶部
        {x2 + s * 2, cy - s * 3, s, s * 2},  // 1: 右上
        {x2, cy - s * 4, s * 3, s},      // 2: 中部
        {x2, cy - s * 6, s, s * 2},      // 3: 左下
        {x2, cy - s * 7, s * 3, s},      // 4: 底部

        // "0" (*** 已修正为 6 个矩形 ***)
        {x0, cy - s, s * 3, s},        // 5: 顶部
        {x0, cy - s * 7, s * 3, s},        // 6: 底部
        // (左侧条被拆分为 2 个)
        {x0, cy - s * 3.5f, s, s * 2.5f}, // 7: 左-上
        {x0, cy - s * 6, s, s * 2.5f},    // 8: 左-下
        // (右侧条被拆分为 2 个)
        {x0 + s * 2, cy - s * 3.5f, s, s * 2.5f}, // 9: 右-上
        {x0 + s * 2, cy - s * 6, s, s * 2.5f}     // 10: 右-下
    };
    // -------------------------------------

    for (int i = 0; i < NUM_PARTICLES_S3; ++i) {

        // 1. 为粒子随机选择一个矩形
        int quadIndex = rand() % 11; // <--- (*** 已修正为 11 ***)

        // 2. 在该矩形内生成一个随机的目标点
        float qx = quads[quadIndex][0];
        float qy = quads[quadIndex][1];
        float qw = quads[quadIndex][2];
        float qh = quads[quadIndex][3];
        particles[i].targetX = qx + (rand() / (float)RAND_MAX) * qw;
        particles[i].targetY = qy + (rand() / (float)RAND_MAX) * qh;

        // 3. 设置粒子的起点 (从底部随机升起)
        particles[i].startX = (rand() % (int)WINDOW_WIDTH);
        particles[i].startY = 150.0f;

        // 4. 设置动画参数
        particles[i].t = 0.0f;
        particles[i].speed = 0.005f + (rand() / (float)RAND_MAX) * 0.005f;
        particles[i].delay = (rand() / (float)RAND_MAX) * 2.0f;

        // 5. 设置颜色
        particles[i].color = particleColors[rand() % numColors];
    }
}
/**
 * @brief (已重写!) 更新场景三的粒子 (使用 LERP)
 */
void updateParticles() {

    // (新!) 获取时间，用于计算振动
    float time = glutGet(GLUT_ELAPSED_TIME) * 0.005f;

    for (int i = 0; i < NUM_PARTICLES_S3; ++i) {

        // 1. 处理延迟 (不变)
        if (particles[i].delay > 0.0f) {
            particles[i].delay -= (16.0f / 1000.0f); // 假设 16ms 间隔
            continue;
        }

        // 2. 更新 t (计时器) (不变)
        if (particles[i].t < 1.0f) {
            particles[i].t += particles[i].speed;
        }

        // --- 3. (*** 关键修改! ***) ---
        //    计算当前位置

        if (particles[i].t >= 1.0f) {
            // --- 状态 2: 振动 (Jitter) ---
            // 粒子已到达，让它在 targetX, targetY 附近振动
            // "i" 确保每个粒子的振动相位不同
            particles[i].x = particles[i].targetX + sin(time * 3.0f + i) * 0.5f; // 水平振动 +/- 1.5px
            particles[i].y = particles[i].targetY + cos(time * 3.0f + i) * 0.5f; // 垂直振动 +/- 1.5px

        }
        else {
            // --- 状态 1: 飞入 (Fly-in) ---
            // (LERP 逻辑保持不变)
            float t = particles[i].t;
            float one_minus_t = 1.0f - t;

            particles[i].x = one_minus_t * particles[i].startX + t * particles[i].targetX;
            particles[i].y = one_minus_t * particles[i].startY + t * particles[i].targetY;
        }
    }
}

/**
 * @brief (已修改!) 初始化/重置所有五彩纸屑的状态
 * (现在是矩形，并有旋转属性)
 */
void initConfetti() {
    Color festiveColors[] = {
        {1.0f, 0.2f, 0.2f}, // 红
        {1.0f, 1.0f, 0.2f}, // 黄
        {0.2f, 0.7f, 1.0f}, // 蓝
        {1.0f, 0.5f, 0.0f}, // 橙
        {0.5f, 1.0f, 0.3f}  // 绿
    };
    int numColors = 5;

    for (int i = 0; i < NUM_CONFETTI; ++i) {
        confetti[i].x = (rand() % (int)WINDOW_WIDTH);
        confetti[i].y = WINDOW_HEIGHT + (rand() % 50);
        confetti[i].speed = 0.5f + (rand() / (float)RAND_MAX) * 1.0f;

        // --- (*** 已修改 ***) ---
        // (不再使用 .size)
        confetti[i].width = 2.0f + (rand() / (float)RAND_MAX) * 3.0f; // 宽度 (2.0 到 5.0)
        confetti[i].height = 5.0f + (rand() / (float)RAND_MAX) * 5.0f; // 高度 (5.0 到 10.0)
        confetti[i].angle = (rand() / (float)RAND_MAX) * 360.0f; // 随机起始角度
        // (随机旋转速度，-5 到 +5)
        confetti[i].rotationSpeed = (rand() / (float)RAND_MAX - 0.5f) * 10.0f;
        // --- (修改结束) ---

        confetti[i].color = festiveColors[rand() % numColors];
    }
}

void initStars() {
    for (int i = 0; i < NUM_STARS; ++i) {
        stars[i].x = (rand() % (int)WINDOW_WIDTH);
        stars[i].y = (rand() % (int)WINDOW_HEIGHT);
        stars[i].size = 0.5f + (rand() / (float)RAND_MAX) * 1.0f; // 随机大小
        // (新!) 赋予一个随机相位，让它们不同步闪烁
        stars[i].phase = (rand() / (float)RAND_MAX) * M_PI * 2.0f;
    }
}


void onAnimationTimer(int value)
{
    // 只在场景二时才更新动画
    if (currentScene == SCENE_LANDSCAPE &&
        (currentSeason == SEASON_SPRING || currentSeason == SEASON_SUMMER))
    {
        // 更新每个气球的位置
        for (int i = 0; i < NUM_BALLOONS; ++i) {
            balloons[i].y += balloons[i].speed;

            // 如果气球飘出屏幕顶部
            if (balloons[i].y > WINDOW_HEIGHT + 50.0f) {
                // 重置到屏幕底部
                balloons[i].y = -50.0f;
                // 重新随机化 X 位置
                balloons[i].x = (rand() % (int)WINDOW_WIDTH);
            }
        }
        glutPostRedisplay(); // 强制重绘
    }

    if (currentScene == SCENE_LANDSCAPE) {
        if (scene2Cat.isWalking) {
            scene2Cat.x += scene2Cat.speed;
            scene2Cat.animationTime += 0.05f; // 动画速度 (您可以调整)

            // 如果小猫走出屏幕右侧，重置到左侧，象征走完一圈四季
            if (scene2Cat.x > WINDOW_WIDTH + 50.0f) {
                scene2Cat.x = -50.0f;
                // (可选) 可以在这里改变季节，或者只在按 'n' 键时改变季节
                // currentSeason = static_cast<Season>((currentSeason + 1) % NUM_SEASONS);
            }
        }
    }

    if (currentScene == SCENE_LANDSCAPE && currentSeason == SEASON_AUTUMN)
    {
        // (新!) 获取时间，用于计算脉冲
        float time = glutGet(GLUT_ELAPSED_TIME) * 0.005f;

        for (int i = 0; i < NUM_WIND_LINES; ++i) {
            windLines[i].x += windLines[i].speed_x; // 更新 X 轨迹
            windLines[i].y += windLines[i].speed_y; // (新!) 更新 Y 轨迹

            // (新!) 动态改变线条长度，使其"脉冲"
            // (在 75% 到 125% 基础长度之间变化)
            windLines[i].length = windLines[i].base_length * (1.0f + sin(time + i) * 0.25f);

            // 如果飘出屏幕 (X 或 Y)
            if (windLines[i].x > WINDOW_WIDTH + windLines[i].length ||
                windLines[i].y < -10.0f || windLines[i].y > WINDOW_HEIGHT + 10.0f)
            {
                // 重置到屏幕左侧
                windLines[i].x = -50.0f - (rand() % 100);
                // 重置到新的随机高度
                windLines[i].y = (rand() % (int)WINDOW_HEIGHT);
                // (新!) 重置 Y 速度，下一次的轨迹将不同
                windLines[i].speed_y = (rand() / (float)RAND_MAX - 0.5f) * 0.5f;
            }
        }
    }

    if (currentScene == SCENE_LANDSCAPE && currentSeason == SEASON_WINTER) {
        for (int i = 0; i < NUM_SNOWFLAKES; ++i) {
            snowflakes[i].y -= snowflakes[i].speed; // 向下移动

            // (可选) 添加左右摇摆
            snowflakes[i].x += sin(snowflakes[i].y * 0.1f) * 0.3f;

            // 如果飘出屏幕底部
            if (snowflakes[i].y < -10.0f) {
                // 重置到屏幕顶部
                snowflakes[i].y = WINDOW_HEIGHT + 10.0f;
                snowflakes[i].x = (rand() % (int)WINDOW_WIDTH);
            }
        }
    }


    if (currentScene == SCENE_BRIDGE) {

        // 1. 粒子汇聚 (这个总是在运行)
        updateParticles();

        // 2. (新!) 牌子融合
        if (scene3State == S3_MERGING && mergeProgress < 1.0f) {
            mergeProgress += 0.008f; // (这是融合的速度, 您可以调整)
            if (mergeProgress >= 1.0f) {
                mergeProgress = 1.0f;
                scene3State = S3_MERGED; // 动画结束
            }
        }
        if (scene3State == S3_MERGED) {
            for (int i = 0; i < NUM_CONFETTI; ++i) {
                confetti[i].y -= confetti[i].speed;
                confetti[i].x += sin(confetti[i].y * 0.1f + i) * 0.3f;

                confetti[i].angle += confetti[i].rotationSpeed; // <--- (*** 新增! ***)

                if (confetti[i].y < -10.0f) {
                    confetti[i].y = WINDOW_HEIGHT + 10.0f;
                    confetti[i].x = (rand() % (int)WINDOW_WIDTH);
                }
            }
        }
    }

    if (currentScene == SCENE_LANDSCAPE || currentScene == SCENE_BRIDGE)
    {
        glutPostRedisplay();
    }

    // 无论如何都重新注册下一次
    glutTimerFunc(16, onAnimationTimer, 0); // 16ms 约等于 60 FPS
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

/**
 * @brief (已重构!) 绘制场景二的静态背景 (渐变天空 + 渐变草地)
 */
void drawLandscapeBackground()
{
    // (这些颜色定义来自上一步，我们将使用它们)
    Color skyTop, skyHorizon, groundHorizon, groundNear;

    // 根据全局季节变量选择颜色
    switch (currentSeason) {
    case SEASON_SPRING:
        skyTop = COLOR_SKY_SPRING_TOP;
        skyHorizon = COLOR_SKY_SPRING_HORIZON;
        groundHorizon = COLOR_GROUND_SPRING_DARK; // 地平线 (暗)
        groundNear = COLOR_GROUND_SPRING_LIGHT; // 靠近屏幕 (亮)
        break;
    case SEASON_SUMMER:
        skyTop = COLOR_SKY_SUMMER_TOP;
        skyHorizon = COLOR_SKY_SUMMER_HORIZON;
        groundHorizon = COLOR_GROUND_SUMMER_DARK;
        groundNear = COLOR_GROUND_SUMMER_LIGHT;
        break;
    case SEASON_AUTUMN:
        skyTop = COLOR_SKY_AUTUMN_TOP;
        skyHorizon = COLOR_SKY_AUTUMN_HORIZON;
        groundHorizon = COLOR_GROUND_AUTUMN_DARK;
        groundNear = COLOR_GROUND_AUTUMN_LIGHT;
        break;
    case SEASON_WINTER:
        skyTop = COLOR_SKY_WINTER_TOP;
        skyHorizon = COLOR_SKY_WINTER_HORIZON;
        groundHorizon = COLOR_GROUND_WINTER_DARK; // 雪 (阴影)
        groundNear = COLOR_GROUND_WINTER_MID;  // 雪 (亮)
        break;
    }

    // 1. 绘制渐变背景 (天空)
    glBegin(GL_QUADS);
    // 顶部 (天顶颜色)
    glColor3f(skyTop.r, skyTop.g, skyTop.b);
    glVertex2f(0, WINDOW_HEIGHT);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    // 底部 (地平线颜色)
    glColor3f(skyHorizon.r, skyHorizon.g, skyHorizon.b);
    glVertex2f(WINDOW_WIDTH, 150); // 地平线 Y=150
    glVertex2f(0, 150);
    glEnd();

    // 2. 绘制渐变草地 (*** 已修改：不再是山丘 ***)
    glBegin(GL_QUADS);
    // 顶部 (地平线处，颜色较暗)
    glColor3f(groundHorizon.r, groundHorizon.g, groundHorizon.b);
    glVertex2f(0, 150);
    glVertex2f(WINDOW_WIDTH, 150);
    // 底部 (靠近屏幕，颜色较亮)
    glColor3f(groundNear.r, groundNear.g, groundNear.b);
    glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(0, 0);
    glEnd();
}

void drawBalloon(float x, float y, const Color& color) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    // 1. 绘制气球主体 (使用您已有的函数)
    glColor3f(color.r, color.g, color.b);
    drawFilledEllipse(15.0f, 20.0f); // 宽 15, 高 20

    // 2. 绘制气球绳子
    glColor3f(COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glVertex2f(0.0f, -20.0f); // 从椭圆底部
    glVertex2f(0.0f, -60.0f); // 绳子长度
    glEnd();

    glPopMatrix();
}

/**
 * @brief (坐标修正!) 绘制图书馆的"我们的面"
 * (仿照用户提供的 "另一个面" 的代码风格，并使用 800x600 像素坐标系)
 */
/**
 * @brief (最终版!) 绘制图书馆的"我们的面"
 * (图层 1: 灰色墙体 + 图层 2: 白色纹理 + 图层 3: 天空蓝镂空 + 图层 4: 黑色边框)
 */
void drawLibrary()
{
    // --- 1. 定义我们这个面 (Our Face) 的坐标 ---
    float frameLeft = 100.0f, frameRight = 350.0f;
    float frameBottom = 150.0f, frameTop = 430.0f;

    // (A) 顶部倒梯形 坐标
    float top_L_x1 = 160.0f, top_R_x1 = 230.0f;
    float top_L_x2 = 175.0f, top_R_x2 = 215.0f;
    float top_B = 365.0f;
    
    // (B) "直角梯形" 坐标
    float midL_T_y = 310.0f, midL_B_y = 270.0f;
    float midL_R_x_top = 115.0f, midL_R_x_bottom = 120.0f;

    // (C) 底部Z形 坐标
    float z_V1_x = 155.0f, z_V1_y = 310.0f; 
    float z_V2_x = 155.0f, z_V2_y = 265.0f; 
    float z_V3_x = 290.0f, z_V3_y = 310.0f; 
    float z_V4_x = 210.0f, z_V4_y = 285.0f; 
    float z_V5_x = 350.0f, z_V5_y = 340.0f; 
    float z_V6_x = 350.0f, z_V6_y = 285.0f;

    // --- (图层 1) 绘制实心灰色墙体 ---
    glColor3f(COLOR_BUILDING_GREY.r, COLOR_BUILDING_GREY.g, COLOR_BUILDING_GREY.b);
    glBegin(GL_POLYGON);
        glVertex2f(frameLeft, frameBottom);
        glVertex2f(frameRight, frameBottom);
        glVertex2f(frameRight, frameTop);
        glVertex2f(frameLeft, frameTop);
    glEnd();

    // --- (图层 2) 绘制白色水平纹理 ---
    glColor3f(COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b);
    glLineWidth(1.0f); // 纹理用细线
    for (float y = frameBottom + 5.0f; y < frameTop; y += 4.0f) { // (我把步长改为 4.0f)
        glBegin(GL_LINES);
            glVertex2f(frameLeft, y);
            glVertex2f(frameRight, y);
        glEnd();
    }

    // --- (图层 3) 绘制天空蓝色 "镂空" 填充 ---
    // (这将覆盖在灰色墙体和白色纹理之上)
    
    // (A) 顶部倒梯形 填充
    glBegin(GL_POLYGON);
        glColor3f(COLOR_GLASS_BLUE.r, COLOR_GLASS_BLUE.g, COLOR_GLASS_BLUE.b);
        glVertex2f(top_L_x2, top_B);
        glVertex2f(top_R_x2, top_B);
        glVertex2f(top_R_x1, frameTop);
        glVertex2f(top_L_x1, frameTop);
    glEnd();
    
    // (B) "直角梯形" 填充
    glBegin(GL_POLYGON);
        glColor3f(COLOR_GLASS_BLUE.r, COLOR_GLASS_BLUE.g, COLOR_GLASS_BLUE.b);
        glVertex2f(frameLeft, midL_B_y);
        glVertex2f(midL_R_x_bottom, midL_B_y);
        glVertex2f(midL_R_x_top, midL_T_y);
        glVertex2f(frameLeft, midL_T_y);
    glEnd();
    
    // 多边形 C1 (左侧的四边形)
    glBegin(GL_POLYGON);
        glColor3f(COLOR_GLASS_BLUE.r, COLOR_GLASS_BLUE.g, COLOR_GLASS_BLUE.b);
        glVertex2f(z_V1_x, z_V1_y); // V1
        glVertex2f(z_V2_x, z_V2_y); // V2
        glVertex2f(z_V4_x, z_V4_y); // V4
        glVertex2f(z_V3_x, z_V3_y); // V3
    glEnd();

    // 多边形 C2 (右侧的四边形)
    glBegin(GL_POLYGON);
        glColor3f(COLOR_GLASS_BLUE.r, COLOR_GLASS_BLUE.g, COLOR_GLASS_BLUE.b);
        glVertex2f(z_V4_x, z_V4_y); // V4
        glVertex2f(z_V6_x, z_V6_y); // V6
        glVertex2f(z_V5_x, z_V5_y); // V5
        glVertex2f(z_V3_x, z_V3_y); // V3
    glEnd();

    // --- (图层 4) 绘制所有黑色边框 (在最顶层) ---
    glColor3f(COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b);
    glLineWidth(3.0f);

    // (外框)
    glBegin(GL_LINE_LOOP);
        glVertex2f(frameLeft, frameBottom);
        glVertex2f(frameRight, frameBottom);
        glVertex2f(frameRight, frameTop);
        glVertex2f(frameLeft, frameTop);
    glEnd();
    
    // (A) 顶部倒梯形 内部边框
    glBegin(GL_LINE_STRIP);
        glVertex2f(top_L_x1, frameTop);
        glVertex2f(top_L_x2, top_B);
        glVertex2f(top_R_x2, top_B);
        glVertex2f(top_R_x1, frameTop);
    glEnd();
    
    // (B) "直角梯形" 内部边框
    glBegin(GL_LINE_STRIP);
        glVertex2f(frameLeft, midL_T_y);
        glVertex2f(midL_R_x_top, midL_T_y);
        glVertex2f(midL_R_x_bottom, midL_B_y);
        glVertex2f(frameLeft, midL_B_y);
    glEnd();
    
    // (C) 底部Z形 完整边框
    glBegin(GL_LINE_LOOP);
        glVertex2f(z_V1_x, z_V1_y);
        glVertex2f(z_V2_x, z_V2_y);
        glVertex2f(z_V4_x, z_V4_y);
        glVertex2f(z_V6_x, z_V6_y);
        glVertex2f(z_V5_x, z_V5_y);
        glVertex2f(z_V3_x, z_V3_y);
    glEnd();

    // 恢复默认线宽
    glLineWidth(1.0f);
}


/**
 * @brief (已重构!) 绘制随季节变化的树 (带渐变树干和分层树冠)
 * @param x 树干的中心 x 坐标
 * @param y 树干的底部 y 坐标
 */
void drawSeasonalTree(float x, float y)
{
    // 1. 绘制树干 (使用渐变来模拟立体感)
    glPushMatrix();
    glTranslatef(x, y, 0); // 将原点移到树干底部中央

    glBegin(GL_QUADS);
    // 左侧 (暗)
    glColor3f(COLOR_TRUNK_DARK.r, COLOR_TRUNK_DARK.g, COLOR_TRUNK_DARK.b);
    glVertex2f(-7.0f, 0.0f);   // 底部左
    glVertex2f(-5.0f, 50.0f);  // 顶部左
    // 右侧 (亮) - 稍微向外倾斜以模拟树干的曲度
    glColor3f(COLOR_TRUNK_LIGHT.r, COLOR_TRUNK_LIGHT.g, COLOR_TRUNK_LIGHT.b);
    glVertex2f(5.0f, 50.0f);   // 顶部右
    glVertex2f(7.0f, 0.0f);    // 底部右
    glEnd();
    glPopMatrix();

    // 2. 根据季节绘制树冠或树枝 (使用图层叠加和更丰富的线条)
    switch (currentSeason)
    {
    case SEASON_SPRING:
        glPushMatrix();
        glTranslatef(x, y + 60.0f, 0); // 将原点移到树冠底部中央
        // (后层, 深绿)
        glColor3f(COLOR_LEAF_SPRING_DARK.r, COLOR_LEAF_SPRING_DARK.g, COLOR_LEAF_SPRING_DARK.b);
        drawFilledEllipse(35.0f, 30.0f); // 底部最大片叶子
        // (中层, 中绿，稍微向上和右偏移，模拟阳光照射)
        glColor3f(COLOR_LEAF_SPRING_MID.r, COLOR_LEAF_SPRING_MID.g, COLOR_LEAF_SPRING_MID.b);
        glTranslatef(7.0f, 7.0f, 0.0f);
        drawFilledEllipse(30.0f, 25.0f);
        // (前层, 亮绿，模拟新芽和最受光部分)
        glColor3f(COLOR_LEAF_SPRING_LIGHT.r, COLOR_LEAF_SPRING_LIGHT.g, COLOR_LEAF_SPRING_LIGHT.b);
        glTranslatef(-15.0f, 5.0f, 0.0f);
        drawFilledEllipse(25.0f, 20.0f);
        glPopMatrix();

        // 绘制小花 (在最上层)
        glColor3f(COLOR_FLOWER_PINK.r, COLOR_FLOWER_PINK.g, COLOR_FLOWER_PINK.b);
        glPushMatrix();
        glTranslatef(x - 10, y + 65.0f, 0); drawFilledEllipse(5.0f, 5.0f);
        glTranslatef(x + 10, y + 55.0f, 0); drawFilledEllipse(5.0f, 5.0f);
        glTranslatef(x, y + 75.0f, 0); drawFilledEllipse(4.0f, 4.0f);
        glPopMatrix();
        break;

    case SEASON_SUMMER:
        glPushMatrix();
        glTranslatef(x, y + 60.0f, 0);
        // (后层, 深绿色，最茂盛)
        glColor3f(COLOR_LEAF_SUMMER_DARK.r, COLOR_LEAF_SUMMER_DARK.g, COLOR_LEAF_SUMMER_DARK.b);
        drawFilledEllipse(40.0f, 35.0f);
        // (中层, 中绿色)
        glColor3f(COLOR_LEAF_SUMMER_MID.r, COLOR_LEAF_SUMMER_MID.g, COLOR_LEAF_SUMMER_MID.b);
        glTranslatef(8.0f, 8.0f, 0.0f);
        drawFilledEllipse(35.0f, 30.0f);
        // (前层, 亮绿色)
        glColor3f(COLOR_LEAF_SUMMER_LIGHT.r, COLOR_LEAF_SUMMER_LIGHT.g, COLOR_LEAF_SUMMER_LIGHT.b);
        glTranslatef(-18.0f, 6.0f, 0.0f);
        drawFilledEllipse(30.0f, 25.0f);
        glPopMatrix();
        break;

    case SEASON_AUTUMN:
        glPushMatrix();
        glTranslatef(x, y + 60.0f, 0);
        // (后层, 暗橙)
        glColor3f(COLOR_LEAF_AUTUMN_DARK.r, COLOR_LEAF_AUTUMN_DARK.g, COLOR_LEAF_AUTUMN_DARK.b);
        drawFilledEllipse(35.0f, 30.0f);
        // (中层, 亮橙)
        glColor3f(COLOR_LEAF_AUTUMN_MID.r, COLOR_LEAF_AUTUMN_MID.g, COLOR_LEAF_AUTUMN_MID.b);
        glTranslatef(7.0f, 7.0f, 0.0f);
        drawFilledEllipse(30.0f, 25.0f);
        // (前层, 黄色)
        glColor3f(COLOR_LEAF_AUTUMN_LIGHT.r, COLOR_LEAF_AUTUMN_LIGHT.g, COLOR_LEAF_AUTUMN_LIGHT.b);
        glTranslatef(-15.0f, 5.0f, 0.0f);
        drawFilledEllipse(25.0f, 20.0f);
        glPopMatrix();
        break;

    case SEASON_WINTER:
        // 绘制更密集的裸露树枝
        glColor3f(COLOR_TRUNK_DARK.r, COLOR_TRUNK_DARK.g, COLOR_TRUNK_DARK.b); // 树枝颜色与树干暗部一致
        glLineWidth(2.5f); // 稍微粗一点
        glPushMatrix();
        glTranslatef(x, y + 40.0f, 0); // 从树干顶部开始

        glBegin(GL_LINES);
        // 主干分叉 (更自然)
        glVertex2f(0.0f, 0.0f); glVertex2f(-12.0f, 25.0f);
        glVertex2f(0.0f, 0.0f); glVertex2f(12.0f, 25.0f);

        // 左侧分支
        glVertex2f(-12.0f, 25.0f); glVertex2f(-20.0f, 40.0f);
        glVertex2f(-12.0f, 25.0f); glVertex2f(-5.0f, 45.0f);

        glVertex2f(-20.0f, 40.0f); glVertex2f(-25.0f, 50.0f);
        glVertex2f(-20.0f, 40.0f); glVertex2f(-15.0f, 48.0f);

        // 右侧分支
        glVertex2f(12.0f, 25.0f); glVertex2f(20.0f, 40.0f);
        glVertex2f(12.0f, 25.0f); glVertex2f(5.0f, 45.0f);

        glVertex2f(20.0f, 40.0f); glVertex2f(25.0f, 50.0f);
        glVertex2f(20.0f, 40.0f); glVertex2f(15.0f, 48.0f);
        glEnd();
        glPopMatrix();
        glLineWidth(1.0f); // 恢复默认线宽
        break;
    }
}

/**
 * @brief (已重写并修正!) 绘制小猫
 * @param x 小猫的中心 x 坐标
 * @param y 小猫脚踩的 *地面* y 坐标 (例如 150.0f)
 * @param animationTime 用于腿部摆动的动画时间
 */
void drawCat(float x, float y, float animationTime) {
    glPushMatrix();
    glTranslatef(x, y, 0); // 1. 将原点移动到小猫的地面中心 (x, y)

    // --- 定义身体参数 ---
    float legLength = 12.0f;
    float legWidth = 4.0f;
    float bodyRadiusY = 15.0f;
    float bodyRadiusX = 25.0f;

    // (*** 关键修复! ***)
    // 身体的中心 Y 应该更低，以便与腿重叠
    float bodyCenterY = legLength + (bodyRadiusY * 0.6f); // (12 + 9 = 21)

    // --- 颜色 ---
    Color catColor = COLOR_CAT_BODY;

    // --- 4. 绘制四条腿 (相对地面) ---
    glColor3f(COLOR_CAT_PAW.r, COLOR_CAT_PAW.g, COLOR_CAT_PAW.b);
    float legOscillation = sin(animationTime * 5.0f);
    float legOscillationInv = sin(animationTime * 5.0f + M_PI);

    // (腿的逻辑不变)
    glPushMatrix();
    glTranslatef(15.0f, legLength / 2.0f, 0);
    glRotatef(legOscillation * 15.0f, 0, 0, 1);
    drawFilledEllipse(legWidth, legLength / 2.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(20.0f, legLength / 2.0f, 0);
    glRotatef(legOscillationInv * 15.0f, 0, 0, 1);
    drawFilledEllipse(legWidth, legLength / 2.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-15.0f, legLength / 2.0f, 0);
    glRotatef(legOscillationInv * 15.0f, 0, 0, 1);
    drawFilledEllipse(legWidth, legLength / 2.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-10.0f, legLength / 2.0f, 0);
    glRotatef(legOscillation * 15.0f, 0, 0, 1);
    drawFilledEllipse(legWidth, legLength / 2.0f);
    glPopMatrix();


    // --- 1. 绘制身体 (在腿上方) ---
    glColor3f(catColor.r, catColor.g, catColor.b);
    glPushMatrix();
    // (*** 已使用修正后的 bodyCenterY ***)
    glTranslatef(0.0f, bodyCenterY, 0.0f);
    drawFilledEllipse(bodyRadiusX, bodyRadiusY);
    glPopMatrix();


    // --- 2. 绘制头部 (相对地面) ---
    // (*** 已使用修正后的 bodyCenterY ***)
    float headCenterY = bodyCenterY + 8.0f;

    glPushMatrix();
    glTranslatef(20.0f, headCenterY, 0.0f);

    // (头部绘制逻辑不变)
    glColor3f(catColor.r, catColor.g, catColor.b);
    drawFilledEllipse(10.0f, 8.0f);
    glColor3f(COLOR_CAT_EAR.r, COLOR_CAT_EAR.g, COLOR_CAT_EAR.b);
    glBegin(GL_TRIANGLES);
    glVertex2f(-7.0f, 6.0f);  glVertex2f(-5.0f, 12.0f); glVertex2f(-3.0f, 6.0f);
    glVertex2f(3.0f, 6.0f);  glVertex2f(5.0f, 12.0f);  glVertex2f(7.0f, 6.0f);
    glEnd();
    glColor3f(COLOR_CAT_EYE.r, COLOR_CAT_EYE.g, COLOR_CAT_EYE.b);
    glPushMatrix();
    glTranslatef(-3.0f, 2.0f, 0.0f);
    drawFilledEllipse(1.5f, 1.5f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(3.0f, 2.0f, 0.0f);
    drawFilledEllipse(1.5f, 1.5f);
    glPopMatrix();

    glPopMatrix(); // 结束头部变换


    // --- 3. 绘制尾巴 (相对地面) ---
    glColor3f(catColor.r, catColor.g, catColor.b);
    glPushMatrix();
    // (*** 已使用修正后的 bodyCenterY ***)
    glTranslatef(-25.0f, bodyCenterY, 0); // 尾巴根部
    glRotatef(sin(animationTime * 2.0f) * 15.0f, 0, 0, 1);
    drawFilledEllipse(8.0f, 4.0f);

    glTranslatef(-10.0f, -5.0f, 0);
    glRotatef(sin(animationTime * 2.0f) * 15.0f, 0, 0, 1);
    drawFilledEllipse(8.0f, 4.0f);
    glPopMatrix();


    glPopMatrix(); // 结束小猫的
}


void drawSun() {
    // (新!) 获取时间，用于计算闪烁
    float time = glutGet(GLUT_ELAPSED_TIME) * 0.005f;
    // (新!) 计算闪烁 (在 90% 到 110% 大小之间脉冲)
    float shimmer = 1.0f + sin(time * 2.0f) * 0.1f;

    // 放在左上角
    glPushMatrix();
    glTranslatef(80.0f, WINDOW_HEIGHT - 80.0f, 0.0f);

    // (假设您有 drawFilledEllipse 函数)
    glColor3f(COLOR_SUN_YELLOW.r, COLOR_SUN_YELLOW.g, COLOR_SUN_YELLOW.b);
    drawFilledEllipse(40.0f, 40.0f); // 太阳主体保持不变

    // 绘制光芒 (应用闪烁)
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < 12; ++i) {
        float angle = 2.0f * M_PI * i / 12.0f;
        float outer_r = 60.0f * shimmer; // (新!) 光芒的外部半径在变化

        glVertex2f(cos(angle) * 45.0f, sin(angle) * 45.0f);
        glVertex2f(cos(angle) * outer_r, sin(angle) * outer_r); // (新!) 使用变化后的半径
    }
    glEnd();
    glLineWidth(1.0f);

    glPopMatrix();
}


void drawWind() {
    // 使用白色细线
    glColor3f(COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < NUM_WIND_LINES; ++i) {
        glVertex2f(windLines[i].x, windLines[i].y);
        glVertex2f(windLines[i].x - windLines[i].length, windLines[i].y); // 向左吹
    }
    glEnd();
}

void drawSnow() {
    // 使用白色小方块 (比 drawFilledEllipse 更快)
    glColor3f(COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b);
    glBegin(GL_QUADS);
    for (int i = 0; i < NUM_SNOWFLAKES; ++i) {
        float s = snowflakes[i].size;
        float x = snowflakes[i].x;
        float y = snowflakes[i].y;
        glVertex2f(x - s, y - s);
        glVertex2f(x + s, y - s);
        glVertex2f(x + s, y + s);
        glVertex2f(x - s, y + s);
    }
    glEnd();
}


void drawSign(float cx, float cy, float width, float height, const std::string& text)
{
    // 1. 绘制牌子背景 (使用深海军蓝，匹配场景)
    glColor3f(COLOR_DARK_BLUE.r, COLOR_DARK_BLUE.g, COLOR_DARK_BLUE.b);
    glBegin(GL_POLYGON);
    glVertex2f(cx - width / 2.0f, cy - height / 2.0f);
    glVertex2f(cx + width / 2.0f, cy - height / 2.0f);
    glVertex2f(cx + width / 2.0f, cy + height / 2.0f);
    glVertex2f(cx - width / 2.0f, cy + height / 2.0f);
    glEnd();

    // 2. 绘制牌子边框 (使用白色，匹配粒子)
    glColor3f(COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(cx - width / 2.0f, cy - height / 2.0f);
    glVertex2f(cx + width / 2.0f, cy - height / 2.0f);
    glVertex2f(cx + width / 2.0f, cy + height / 2.0f);
    glVertex2f(cx - width / 2.0f, cy + height / 2.0f);
    glEnd();
    glLineWidth(1.0f); // 恢复默认线宽

    // 3. 绘制文字 (白色, 居中)
    float textScale = 0.18f; // 牌子上文字的缩放
    float textStroke = 2.0f;

    // 调整 Y 坐标以实现垂直居中 (因为 drawTextStrokeCentered 是基于基线对齐的)
    float textY_baseline = cy - (textScale * 25.0f); // 这是一个用于垂直居中的近似值

    drawTextStrokeCentered(cx, textY_baseline, text, textScale, textStroke, COLOR_WHITE);
}

/**
 * @brief (已重构!) 绘制五彩纸屑 (现在是旋转的矩形)
 */
void drawConfetti() {

    for (int i = 0; i < NUM_CONFETTI; ++i) {
        float w = confetti[i].width;
        float h = confetti[i].height;
        float x = confetti[i].x;
        float y = confetti[i].y;

        glPushMatrix(); // 保存矩阵

        // 1. 移动到五彩纸屑的位置
        glTranslatef(x, y, 0.0f);
        // 2. 绕着它的中心旋转
        glRotatef(confetti[i].angle, 0.0f, 0.0f, 1.0f);

        // 3. 设置颜色
        glColor3f(confetti[i].color.r, confetti[i].color.g, confetti[i].color.b);

        // 4. 绘制一个 *以(0,0)为中心* 的矩形
        glBegin(GL_QUADS);
        glVertex2f(-w / 2.0f, -h / 2.0f);
        glVertex2f(w / 2.0f, -h / 2.0f);
        glVertex2f(w / 2.0f, h / 2.0f);
        glVertex2f(-w / 2.0f, h / 2.0f);
        glEnd();

        glPopMatrix(); // 恢复矩阵
    }
}

void drawStarfield() {
    // (新!) 获取时间，用于计算闪烁
    float time = glutGet(GLUT_ELAPSED_TIME) * 0.001f; // (使用 0.001 让闪烁更慢)

    // (*** 关键! ***)
    // 我们必须启用 BLEND 才能让透明度生效
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    for (int i = 0; i < NUM_STARS; ++i) {
        float s = stars[i].size;
        float x = stars[i].x;
        float y = stars[i].y;

        // (新!) 计算亮度/透明度 (使用 sin 函数在 0.2 到 1.0 之间平滑脉冲)
        float brightness = 0.6f + (sin(time + stars[i].phase) + 1.0f) * 0.2f;

        // (使用带透明度的颜色)
        glColor4f(1.0f, 1.0f, 0.8f, brightness); // 柔和的黄白色

        glVertex2f(x - s, y - s);
        glVertex2f(x + s, y - s);
        glVertex2f(x + s, y + s);
        glVertex2f(x - s, y + s);
    }
    glEnd();

    // (可选，但推荐) 关闭 BLEND，以防它影响其他不透明的绘制
    glDisable(GL_BLEND);
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

    // --- 1. 绘制静态背景 (天和草) ---
    drawLandscapeBackground();

    // --- 2. 绘制太阳 (仅夏季，在天空) ---
    if (currentSeason == SEASON_SUMMER) {
        drawSun();
    }

    // --- 3. 绘制静态风景 (在小猫后面) ---
    drawLibrary();
    drawSeasonalTree(700.0f, 150.0f);

    // --- 4. 绘制风或雪 (大气效果，在小猫后面) ---
    if (currentSeason == SEASON_AUTUMN) {
        drawWind();
    }
    if (currentSeason == SEASON_WINTER) {
        drawSnow();
    }

    // --- 5. (新图层!) 绘制小猫 ---
    // (现在它会出现在图书馆和树的前面)
    drawCat(scene2Cat.x, scene2Cat.y, scene2Cat.animationTime);

    // --- 6. 绘制所有气球 (动态，在最前面) ---
    if (currentSeason == SEASON_SPRING || currentSeason == SEASON_SUMMER)
    {
        for (int i = 0; i < NUM_BALLOONS; ++i) {
            drawBalloon(balloons[i].x, balloons[i].y, balloons[i].color);
        }
    }

    // --- 7. 绘制文字 (UI，总在最顶层) ---
    drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 540.0f, "20 Years of Dedication", 0.15f, 3.0f, COLOR_WHITE);
    drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 520.0f, "Walk through four seasons with you...", 0.15f, 3.0f, COLOR_WHITE);
    Color instructionColor = (currentSeason == SEASON_WINTER) ? COLOR_BLACK : COLOR_WHITE;
    drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 30.0f, "Press 'n' for next season | 'Space' to continue | 'Esc' to exit", 0.1f, 2.0f, instructionColor);
}


// Scene3: Cake
void drawSceneBridge() {
    // 1. 绘制背景 (深海军蓝)
    glColor3f(0.05f, 0.05f, 0.1f);
    glBegin(GL_POLYGON);
    glVertex2f(0, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();

    drawStarfield();
    // 2. 绘制牌子 (*** 已修改为动态 ***)

        // (我们使用 LERP: B(t) = (1-t)*P0 + t*P1)
    float t = mergeProgress;
    float one_minus_t = 1.0f - t;

    // 计算左牌子的当前 X
    float currentLeftX = one_minus_t * signStartX_L + t * signTargetX;
    // 计算右牌子的当前 X
    float currentRightX = one_minus_t * signStartX_R + t * signTargetX;

    if (scene3State == S3_MERGED) {
        // --- 状态 3: 动画结束 ---
        // 只绘制一个 "XJTLU" 牌子
        drawSign(signTargetX, signY, signWidth, signHeight, "XJTLU");
    }
    else {
        // --- 状态 1 和 2: 初始 & 移动中 ---
        // 绘制两个移动中的牌子
        drawSign(currentLeftX, signY, signWidth, signHeight, "Suzhou");
        drawSign(currentRightX, signY, signWidth, signHeight, "Liverpool");
    }

    // 4. 绘制 Slogan
    if (scene3State == S3_INITIAL) {
        drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 412.0f, "Years of", 0.2f, 2.0f, COLOR_WHITE);
        drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 390.0f, "Connecting Worlds", 0.2f, 2.0f, COLOR_WHITE);
        drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 25.0f, "Press 'r' to Go Back | Press 'c' to continue | 'Esc' to exit", 0.1f, 2.0f, COLOR_WHITE);
    }
    else if (scene3State == S3_MERGING) {
        // 状态 2: 牌子融合时 (保持安静)
        drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 412.0f, "Years of", 0.2f, 2.0f, COLOR_WHITE);
        drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 390.0f, "Connecting Worlds", 0.2f, 2.0f, COLOR_WHITE);

    }
    else if (scene3State == S3_MERGED) {
        // 状态 3: 动画结束后
        drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 350.0f, "Happy 20th Anniversary!", 0.25f, 3.0f, COLOR_WHITE);
        drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 50.0f, "Press 'r' to Go Back", 0.12f, 2.0f, COLOR_WHITE);
    }

    // 5. 绘制所有粒子 (它们现在 *就是* "20")
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < NUM_PARTICLES_S3; ++i) {
        // (可选: 只有在开始移动后才绘制)
        // if (particles[i].delay <= 0.0f) { 
        glColor3f(particles[i].color.r, particles[i].color.g, particles[i].color.b);
        glVertex2f(particles[i].x, particles[i].y);
        // }
    }

    if (scene3State == S3_MERGED) {
        drawConfetti();
    }
    glEnd();
}


// Display
// --- 替换您项目中原来的 'display' 函数 ---
void display(void) {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
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
    case SCENE_BRIDGE:
        drawSceneBridge();      // (您场景三的占位符)
        break;
    }
    glDisable(GL_BLEND);
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
 * 键盘回调 (*** 已修正并补全 ***)
 */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {

    case ' ':
        if (currentScene == SCENE_LANDSCAPE) {
            currentScene = SCENE_BRIDGE;
            initBalloons();
            initParticles();
            initConfetti();

            // (新!) 重置场景三的状态
            scene3State = S3_INITIAL;
            mergeProgress = 0.0f;

            glutPostRedisplay();
        }
        break;

    case 'r':
    case 'R':
        if (currentScene == SCENE_BRIDGE) {
            currentScene = SCENE_LANDSCAPE;
            initCat();
            // 重置 场景2
            currentSeason = SEASON_SPRING;
            initBalloons();
            initWindLines();
            initSnowflakes();

            glutPostRedisplay();
        }
        else if (currentScene == SCENE_LANDSCAPE) {
            // (重置场景2的逻辑)
            currentSeason = SEASON_SPRING;
            initBalloons();
            initWindLines();
            initSnowflakes();
            initCat();
            glutPostRedisplay();
        }
        // ... (您可能还有 SCENE_COVER 的重置逻辑)
        break;

        // --- ( 'c' 键交互) ---
    case 'c':
    case 'C':
        // 只有在场景三, 并且在 "初始" 状态时才触发
        if (currentScene == SCENE_BRIDGE && scene3State == S3_INITIAL) {
            scene3State = S3_MERGING;
            // (onAnimationTimer 会自动处理动画, 无需 PostRedisplay)
        }
        break;

    case 27:
        exit(0);
        break;

    case 'n':
    case 'N':
        // --- (*** 已补全! ***) ---
        if (currentScene == SCENE_LANDSCAPE) {

            // 按顺序循环
            switch (currentSeason) {
            case SEASON_SPRING:
                currentSeason = SEASON_SUMMER;
                break;
            case SEASON_SUMMER:
                currentSeason = SEASON_AUTUMN;
                initWindLines(); // 切换到秋季时，初始化风
                break;
            case SEASON_AUTUMN:
                currentSeason = SEASON_WINTER;
                initSnowflakes(); // 切换到冬季时，初始化雪
                break;
            case SEASON_WINTER:
                currentSeason = SEASON_SPRING;
                initBalloons(); // 切换回春天时，初始化气球
                break;
            }

            glutPostRedisplay();
        }
        break;
    }
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

    glShadeModel(GL_SMOOTH);
    // --- 初始化场景一的变量 ---
    envelopeFlapY = flapStartY;
    srand(static_cast<unsigned int>(time(NULL))); // 初始化随机数种子
    initBalloons(); // 初始化气球的起始位置
    initSnowflakes();
    initWindLines();
    initCat();
    initParticles();
    initConfetti();
    initStars();

    // --- 注册回调 ---
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glutPassiveMotionFunc(passiveMotion); // <--- 添加这一行
    glutTimerFunc(0, onAnimationTimer, 0); // 立即启动定时器

    glutMainLoop();
    return 0;
}