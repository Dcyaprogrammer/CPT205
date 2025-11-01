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
    S3_INITIAL, // ��ʼ״̬ (�������ڻ��, �����Ƿֿ���)
    S3_MERGING, // "c" �Ѱ��� (���������ƶ�)
    S3_MERGED   // �������� (��ʾ "XJTLU")
};
Scene3State scene3State = S3_INITIAL; // Ĭ��״̬

// colors
struct Color {
    float r, g, b;
};
const Color COLOR_WHITE = { 1.0f, 1.0f, 1.0f };
const Color COLOR_XJTLU_BLUE = { 0.0f, 0.0f, 0.5f };
const Color COLOR_XJTLU_BLUE_DARK = { 0.0f, 0.0f, 0.4f };
const Color COLOR_GREY = { 0.7f, 0.7f, 0.7f };
const Color COLOR_BLACK = { 0.1f, 0.1f, 0.1f };
const Color COLOR_BUILDING_GREY = { 0.3f, 0.3f, 0.35f }; // (��!) ������Ӱ
const Color COLOR_GLASS_BLUE = { 0.2f, 0.4f, 0.8f }; // (��!) ƫ���Ĳ���ɫ
const Color COLOR_DARK_BLUE = { 0.1f, 0.1f, 0.4f }; // (��!) ����������ɫ
const Color COLOR_SUN_YELLOW = { 1.0f, 0.9f, 0.2f }; // (��!) ̫����

const Color COLOR_SKY_BLUE = { 0.5f, 0.8f, 1.0f }; // (��!) �����

// --- (��!) ������յ���ɫ ---
const Color COLOR_SKY_SPRING_TOP = { 0.6f, 0.85f, 1.0f }; // ����: �춥 (ǳ��)
const Color COLOR_SKY_SPRING_HORIZON = { 0.8f, 0.95f, 1.0f }; // ����: ��ƽ�� (����)

const Color COLOR_SKY_SUMMER_TOP = { 0.3f, 0.7f, 1.0f }; // ����: �춥 (����)
const Color COLOR_SKY_SUMMER_HORIZON = { 0.6f, 0.85f, 1.0f }; // ����: ��ƽ�� (����)

const Color COLOR_SKY_AUTUMN_TOP = { 0.7f, 0.8f, 0.9f }; // ����: �춥 (��)
const Color COLOR_SKY_AUTUMN_HORIZON = { 0.9f, 0.9f, 0.9f }; // ����: ��ƽ�� (����)

const Color COLOR_SKY_WINTER_TOP = { 0.8f, 0.9f, 0.95f }; // ����: �춥 (ǳ��)
const Color COLOR_SKY_WINTER_HORIZON = { 1.0f, 1.0f, 1.0f }; // ����: ��ƽ�� (��ɫ)

// --- (��!) �ݵ�/����ͼ�� ---
// (�����е� COLOR_GRASS_GREEN, COLOR_GROUND_AUTUMN �ȿ��Ա���Щ�滻)
const Color COLOR_GROUND_SPRING_DARK = { 0.1f, 0.5f, 0.1f };
const Color COLOR_GROUND_SPRING_MID = { 0.2f, 0.6f, 0.2f };
const Color COLOR_GROUND_SPRING_LIGHT = { 0.3f, 0.7f, 0.3f };

const Color COLOR_GROUND_SUMMER_DARK = { 0.1f, 0.6f, 0.1f }; // ������
const Color COLOR_GROUND_SUMMER_MID = { 0.2f, 0.7f, 0.2f };
const Color COLOR_GROUND_SUMMER_LIGHT = { 0.3f, 0.8f, 0.3f };

const Color COLOR_GROUND_AUTUMN_DARK = { 0.7f, 0.4f, 0.1f }; // ��
const Color COLOR_GROUND_AUTUMN_MID = { 0.8f, 0.5f, 0.1f };
const Color COLOR_GROUND_AUTUMN_LIGHT = { 0.9f, 0.6f, 0.2f }; // ��

const Color COLOR_GROUND_WINTER_DARK = { 0.9f, 0.9f, 1.0f }; // ѩ����Ӱ (����)
const Color COLOR_GROUND_WINTER_MID = { 1.0f, 1.0f, 1.0f }; // ѩ (��ɫ)
const Color COLOR_GROUND_WINTER_LIGHT = { 1.0f, 1.0f, 1.0f }; // ѩ (��ɫ)

// --- (��!) ���ɽ��� ---
const Color COLOR_TRUNK_LIGHT = { 0.5f, 0.3f, 0.2f }; // �������� (���磬�ܹ���)
const Color COLOR_TRUNK_DARK = { 0.3f, 0.15f, 0.1f }; // ���ɰ��� (���磬������)

// --- (��!) ��Ҷͼ�� (��/��/��) ---
const Color COLOR_LEAF_SPRING_DARK = { 0.3f, 0.7f, 0.3f };  // ����: ����
const Color COLOR_LEAF_SPRING_MID = { 0.4f, 0.8f, 0.4f };   // ����: ����
const Color COLOR_LEAF_SPRING_LIGHT = { 0.5f, 0.9f, 0.5f }; // ����: ���� (��ѿ��)

const Color COLOR_LEAF_SUMMER_DARK = { 0.1f, 0.5f, 0.1f };  // ����: ����ɫ (ïʢ)
const Color COLOR_LEAF_SUMMER_MID = { 0.2f, 0.6f, 0.2f };   // ����: ����ɫ
const Color COLOR_LEAF_SUMMER_LIGHT = { 0.3f, 0.7f, 0.3f }; // ����: ����ɫ

const Color COLOR_LEAF_AUTUMN_DARK = { 0.8f, 0.3f, 0.0f };  // ����: ���� (�ײ�)
const Color COLOR_LEAF_AUTUMN_MID = { 1.0f, 0.5f, 0.1f };   // ����: ���� (�в�)
const Color COLOR_LEAF_AUTUMN_LIGHT = { 1.0f, 0.8f, 0.2f }; // ����: �� (����)

// (�����������е� COLOR_FLOWER_PINK ���ڴ����С��)
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
bool isHoveringHandle = false; // <--- �����һ�У����ڸ���
void* evelopeFont = GLUT_STROKE_MONO_ROMAN;

bool isStampPressed = false; // �����ʴ��Ƿ�������ס��
const float stampCX = WINDOW_WIDTH - 100.0f; // ���Ͻ� X
const float stampCY = 500.0f - 100.0f;        // (�ŷⶥ�� - 100)
const float stampRadius = 40.0f;             // �ʴ��뾶

// scene2
struct Balloon {
    float x, y;
    float speed;
    Color color;
};
const int NUM_BALLOONS = 7; // (�����Ե������������)
Balloon balloons[NUM_BALLOONS];

struct Snowflake {
    float x, y;
    float speed;
    float size;
};
const int NUM_SNOWFLAKES = 150; // (�����Ե�������)
Snowflake snowflakes[NUM_SNOWFLAKES];

// --- (��!) ���ȫ�ֱ��� ---
struct WindLine {
    float x, y;
    float speed_x;     // (�� speed ������)
    float speed_y;     // (��!) ��ֱ�ٶȣ�������б�Ĺ켣
    float length;      // (��!) ��ǰ���ȣ�����̬�仯
    float base_length; // (��!) �������ȣ����ڼ���
};
const int NUM_WIND_LINES = 20; // (�����Ե�������)
WindLine windLines[NUM_WIND_LINES];

// --- (��!) Сè��ȫ�ֱ��� ---
struct Cat {
    float x, y;          // Сè����������
    float speed;         // �����ٶ�
    float animationTime; // ������ʱ���������Ȳ��ڶ�
    bool isWalking;      // �Ƿ���������
    // (�����Ը�����Ҫ�����������ɫ����С��)
};
Cat scene2Cat; // ����2�е�Сèʵ��
// (��!) Сè����ɫ (�ɸ��ݼ��ڱ仯)
const Color COLOR_CAT_BODY = { 0.3f, 0.3f, 0.3f }; // ���ɫ/��ɫ
const Color COLOR_CAT_PAW = { 0.4f, 0.4f, 0.4f };  // צ����ɫ
const Color COLOR_CAT_EAR = { 0.2f, 0.2f, 0.2f };  // ������ɫ
const Color COLOR_CAT_EYE = { 0.9f, 0.9f, 0.1f };  // �۾���ɫ (��)


// scene3
struct Particle {
    float x, y;             // ���� *��ǰ* �� x, y
    float startX, startY;   // ���ĳ�����
    float targetX, targetY; // ��������Ŀ��� (�� "20" ��)

    float t;     // ������ʱ�� (0.0 to 1.0)
    float speed; // �����ٶ�
    float delay; // (��!) �����ӳ٣������� staggered
    Color color;
};
// (���ֵ���ںܹؼ����������� "20" �������ܶ�)
const int NUM_PARTICLES_S3 = 100; // ���� 100
Particle particles[NUM_PARTICLES_S3];

// --- (���޸�!) ���ֽм��ȫ�ֱ��� ---
struct Confetti {
    float x, y;
    float speed;
    float width;  // <--- (��!) ���
    float height; // <--- (��!) �߶�
    Color color;
    float angle;  // <--- (��!) ��ǰ��ת�Ƕ�
    float rotationSpeed; // <--- (��!) ��ת�ٶ�
};

const int NUM_CONFETTI = 150;
Confetti confetti[NUM_CONFETTI];

struct Star {
    float x, y;
    float size;
    float phase; // (��!) ���ڿ���ÿ�����ǵ���˸����
};
const int NUM_STARS = 200; // (�����Ե�������)
Star stars[NUM_STARS];

float mergeProgress = 0.0f; // ������ʱ�� (0.0 = �ֿ�, 1.0 = �ں�)
const float signWidth = 180.0f;
const float signHeight = 60.0f;
const float signY = 150.0f;
const float signStartX_L = 100.0f;                 // ��������ʼX
const float signStartX_R = WINDOW_WIDTH - 100.0f;  // ��������ʼX
const float signTargetX = WINDOW_WIDTH / 2.0f;   // �������ӵ� *��ͬ* Ŀ��X

// utils
float getStrokeTextWidth(void* font, const std::string& s) {
    float width = 0.0f;
    for (char ch : s) { // C++11 ����ѭ��
        width += glutStrokeWidth(font, ch);
    }
    return width;
}

void initBalloons() {
    Color balloonColors[] = {
        {1.0f, 0.2f, 0.2f}, // ��
        {0.2f, 0.5f, 1.0f}, // ��
        {1.0f, 1.0f, 0.2f}, // ��
        {1.0f, 0.5f, 0.0f}, // ��
        {0.8f, 0.3f, 1.0f}  // ��
    };
    int numColors = 5;

    for (int i = 0; i < NUM_BALLOONS; ++i) {
        balloons[i].x = (rand() % (int)WINDOW_WIDTH); // ��� X
        balloons[i].y = -50.0f - (rand() % 100);     // ��� Y (����Ļ�·���ʼ)
        balloons[i].speed = 0.5f + (rand() / (float)RAND_MAX) * 1.0f; // ����ٶ�
        balloons[i].color = balloonColors[rand() % numColors];
    }
}

void initSnowflakes() {
    for (int i = 0; i < NUM_SNOWFLAKES; ++i) {
        snowflakes[i].x = (rand() % (int)WINDOW_WIDTH);
        snowflakes[i].y = (rand() % (int)WINDOW_HEIGHT); // ���Y
        snowflakes[i].speed = 0.5f + (rand() / (float)RAND_MAX) * 1.0f; // ����ٶ�
        snowflakes[i].size = 1.0f + (rand() / (float)RAND_MAX) * 2.0f; // �����С
    }
}

/**
 * @brief (��!) ��ʼ��/�������з��ߵ�״̬
 */
void initWindLines() {
    for (int i = 0; i < NUM_WIND_LINES; ++i) {
        windLines[i].x = -50.0f - (rand() % 100);
        windLines[i].y = (rand() % (int)WINDOW_HEIGHT);
        windLines[i].speed_x = 1.0f + (rand() / (float)RAND_MAX) * 2.0f;

        // (��!) ����һ��С�ġ�����Ĵ�ֱ�ٶȣ�ʹ�켣��б
        windLines[i].speed_y = (rand() / (float)RAND_MAX - 0.5f) * 0.5f; // ֵ�� -0.25 �� +0.25 ֮��

        windLines[i].base_length = 15.0f + (rand() / (float)RAND_MAX) * 15.0f;
        windLines[i].length = windLines[i].base_length;
    }
}

/**
 * @brief (��!) ��ʼ��Сè��״̬
 */
 /**
  * @brief (������!) ��ʼ��Сè��״̬
  */
void initCat() {
    scene2Cat.x = -50.0f; // ����Ļ����⿪ʼ

    // (*** �ؼ��޸� ***)
    // 80.0f �Ǵ�ģ��ݵ�/����Ķ����� 150.0f
    scene2Cat.y = 150.0f; // �ڲݵ������ߵĸ߶� (Y=150)

    scene2Cat.speed = 1.0f;
    scene2Cat.animationTime = 0.0f;
    scene2Cat.isWalking = true;
}

/**
 * @brief (����д!) ��ʼ������
 * (Ϊÿ�����ӷ���һ�� "20" �ϵ����Ŀ���)
 */
void initParticles() {
    Color particleColors[] = {
        COLOR_WHITE, COLOR_SKY_BLUE, {1.0f, 1.0f, 0.2f}
    };
    int numColors = 3;

    // --- "20" ������ (���� drawPixel20) ---
    float s = 8.0f; // ���ؿ��С
    float cx = WINDOW_WIDTH / 2.0f;
    float cy = 500.0f;
    float x2 = cx - s * 5;
    float x0 = cx + s;

    // (*** �������������� 11 ������ ***)
    float quads[11][4] = {
        // "2" (5 ������)
        {x2, cy - s, s * 3, s},      // 0: ����
        {x2 + s * 2, cy - s * 3, s, s * 2},  // 1: ����
        {x2, cy - s * 4, s * 3, s},      // 2: �в�
        {x2, cy - s * 6, s, s * 2},      // 3: ����
        {x2, cy - s * 7, s * 3, s},      // 4: �ײ�

        // "0" (*** ������Ϊ 6 ������ ***)
        {x0, cy - s, s * 3, s},        // 5: ����
        {x0, cy - s * 7, s * 3, s},        // 6: �ײ�
        // (����������Ϊ 2 ��)
        {x0, cy - s * 3.5f, s, s * 2.5f}, // 7: ��-��
        {x0, cy - s * 6, s, s * 2.5f},    // 8: ��-��
        // (�Ҳ��������Ϊ 2 ��)
        {x0 + s * 2, cy - s * 3.5f, s, s * 2.5f}, // 9: ��-��
        {x0 + s * 2, cy - s * 6, s, s * 2.5f}     // 10: ��-��
    };
    // -------------------------------------

    for (int i = 0; i < NUM_PARTICLES_S3; ++i) {

        // 1. Ϊ�������ѡ��һ������
        int quadIndex = rand() % 11; // <--- (*** ������Ϊ 11 ***)

        // 2. �ڸþ���������һ�������Ŀ���
        float qx = quads[quadIndex][0];
        float qy = quads[quadIndex][1];
        float qw = quads[quadIndex][2];
        float qh = quads[quadIndex][3];
        particles[i].targetX = qx + (rand() / (float)RAND_MAX) * qw;
        particles[i].targetY = qy + (rand() / (float)RAND_MAX) * qh;

        // 3. �������ӵ���� (�ӵײ��������)
        particles[i].startX = (rand() % (int)WINDOW_WIDTH);
        particles[i].startY = 150.0f;

        // 4. ���ö�������
        particles[i].t = 0.0f;
        particles[i].speed = 0.005f + (rand() / (float)RAND_MAX) * 0.005f;
        particles[i].delay = (rand() / (float)RAND_MAX) * 2.0f;

        // 5. ������ɫ
        particles[i].color = particleColors[rand() % numColors];
    }
}
/**
 * @brief (����д!) ���³����������� (ʹ�� LERP)
 */
void updateParticles() {

    // (��!) ��ȡʱ�䣬���ڼ�����
    float time = glutGet(GLUT_ELAPSED_TIME) * 0.005f;

    for (int i = 0; i < NUM_PARTICLES_S3; ++i) {

        // 1. �����ӳ� (����)
        if (particles[i].delay > 0.0f) {
            particles[i].delay -= (16.0f / 1000.0f); // ���� 16ms ���
            continue;
        }

        // 2. ���� t (��ʱ��) (����)
        if (particles[i].t < 1.0f) {
            particles[i].t += particles[i].speed;
        }

        // --- 3. (*** �ؼ��޸�! ***) ---
        //    ���㵱ǰλ��

        if (particles[i].t >= 1.0f) {
            // --- ״̬ 2: �� (Jitter) ---
            // �����ѵ�������� targetX, targetY ������
            // "i" ȷ��ÿ�����ӵ�����λ��ͬ
            particles[i].x = particles[i].targetX + sin(time * 3.0f + i) * 0.5f; // ˮƽ�� +/- 1.5px
            particles[i].y = particles[i].targetY + cos(time * 3.0f + i) * 0.5f; // ��ֱ�� +/- 1.5px

        }
        else {
            // --- ״̬ 1: ���� (Fly-in) ---
            // (LERP �߼����ֲ���)
            float t = particles[i].t;
            float one_minus_t = 1.0f - t;

            particles[i].x = one_minus_t * particles[i].startX + t * particles[i].targetX;
            particles[i].y = one_minus_t * particles[i].startY + t * particles[i].targetY;
        }
    }
}

/**
 * @brief (���޸�!) ��ʼ��/�����������ֽм��״̬
 * (�����Ǿ��Σ�������ת����)
 */
void initConfetti() {
    Color festiveColors[] = {
        {1.0f, 0.2f, 0.2f}, // ��
        {1.0f, 1.0f, 0.2f}, // ��
        {0.2f, 0.7f, 1.0f}, // ��
        {1.0f, 0.5f, 0.0f}, // ��
        {0.5f, 1.0f, 0.3f}  // ��
    };
    int numColors = 5;

    for (int i = 0; i < NUM_CONFETTI; ++i) {
        confetti[i].x = (rand() % (int)WINDOW_WIDTH);
        confetti[i].y = WINDOW_HEIGHT + (rand() % 50);
        confetti[i].speed = 0.5f + (rand() / (float)RAND_MAX) * 1.0f;

        // --- (*** ���޸� ***) ---
        // (����ʹ�� .size)
        confetti[i].width = 2.0f + (rand() / (float)RAND_MAX) * 3.0f; // ��� (2.0 �� 5.0)
        confetti[i].height = 5.0f + (rand() / (float)RAND_MAX) * 5.0f; // �߶� (5.0 �� 10.0)
        confetti[i].angle = (rand() / (float)RAND_MAX) * 360.0f; // �����ʼ�Ƕ�
        // (�����ת�ٶȣ�-5 �� +5)
        confetti[i].rotationSpeed = (rand() / (float)RAND_MAX - 0.5f) * 10.0f;
        // --- (�޸Ľ���) ---

        confetti[i].color = festiveColors[rand() % numColors];
    }
}

void initStars() {
    for (int i = 0; i < NUM_STARS; ++i) {
        stars[i].x = (rand() % (int)WINDOW_WIDTH);
        stars[i].y = (rand() % (int)WINDOW_HEIGHT);
        stars[i].size = 0.5f + (rand() / (float)RAND_MAX) * 1.0f; // �����С
        // (��!) ����һ�������λ�������ǲ�ͬ����˸
        stars[i].phase = (rand() / (float)RAND_MAX) * M_PI * 2.0f;
    }
}


void onAnimationTimer(int value)
{
    // ֻ�ڳ�����ʱ�Ÿ��¶���
    if (currentScene == SCENE_LANDSCAPE &&
        (currentSeason == SEASON_SPRING || currentSeason == SEASON_SUMMER))
    {
        // ����ÿ�������λ��
        for (int i = 0; i < NUM_BALLOONS; ++i) {
            balloons[i].y += balloons[i].speed;

            // �������Ʈ����Ļ����
            if (balloons[i].y > WINDOW_HEIGHT + 50.0f) {
                // ���õ���Ļ�ײ�
                balloons[i].y = -50.0f;
                // ��������� X λ��
                balloons[i].x = (rand() % (int)WINDOW_WIDTH);
            }
        }
        glutPostRedisplay(); // ǿ���ػ�
    }

    if (currentScene == SCENE_LANDSCAPE) {
        if (scene2Cat.isWalking) {
            scene2Cat.x += scene2Cat.speed;
            scene2Cat.animationTime += 0.05f; // �����ٶ� (�����Ե���)

            // ���Сè�߳���Ļ�Ҳ࣬���õ���࣬��������һȦ�ļ�
            if (scene2Cat.x > WINDOW_WIDTH + 50.0f) {
                scene2Cat.x = -50.0f;
                // (��ѡ) ����������ı伾�ڣ�����ֻ�ڰ� 'n' ��ʱ�ı伾��
                // currentSeason = static_cast<Season>((currentSeason + 1) % NUM_SEASONS);
            }
        }
    }

    if (currentScene == SCENE_LANDSCAPE && currentSeason == SEASON_AUTUMN)
    {
        // (��!) ��ȡʱ�䣬���ڼ�������
        float time = glutGet(GLUT_ELAPSED_TIME) * 0.005f;

        for (int i = 0; i < NUM_WIND_LINES; ++i) {
            windLines[i].x += windLines[i].speed_x; // ���� X �켣
            windLines[i].y += windLines[i].speed_y; // (��!) ���� Y �켣

            // (��!) ��̬�ı��������ȣ�ʹ��"����"
            // (�� 75% �� 125% ��������֮��仯)
            windLines[i].length = windLines[i].base_length * (1.0f + sin(time + i) * 0.25f);

            // ���Ʈ����Ļ (X �� Y)
            if (windLines[i].x > WINDOW_WIDTH + windLines[i].length ||
                windLines[i].y < -10.0f || windLines[i].y > WINDOW_HEIGHT + 10.0f)
            {
                // ���õ���Ļ���
                windLines[i].x = -50.0f - (rand() % 100);
                // ���õ��µ�����߶�
                windLines[i].y = (rand() % (int)WINDOW_HEIGHT);
                // (��!) ���� Y �ٶȣ���һ�εĹ켣����ͬ
                windLines[i].speed_y = (rand() / (float)RAND_MAX - 0.5f) * 0.5f;
            }
        }
    }

    if (currentScene == SCENE_LANDSCAPE && currentSeason == SEASON_WINTER) {
        for (int i = 0; i < NUM_SNOWFLAKES; ++i) {
            snowflakes[i].y -= snowflakes[i].speed; // �����ƶ�

            // (��ѡ) �������ҡ��
            snowflakes[i].x += sin(snowflakes[i].y * 0.1f) * 0.3f;

            // ���Ʈ����Ļ�ײ�
            if (snowflakes[i].y < -10.0f) {
                // ���õ���Ļ����
                snowflakes[i].y = WINDOW_HEIGHT + 10.0f;
                snowflakes[i].x = (rand() % (int)WINDOW_WIDTH);
            }
        }
    }


    if (currentScene == SCENE_BRIDGE) {

        // 1. ���ӻ�� (�������������)
        updateParticles();

        // 2. (��!) �����ں�
        if (scene3State == S3_MERGING && mergeProgress < 1.0f) {
            mergeProgress += 0.008f; // (�����ںϵ��ٶ�, �����Ե���)
            if (mergeProgress >= 1.0f) {
                mergeProgress = 1.0f;
                scene3State = S3_MERGED; // ��������
            }
        }
        if (scene3State == S3_MERGED) {
            for (int i = 0; i < NUM_CONFETTI; ++i) {
                confetti[i].y -= confetti[i].speed;
                confetti[i].x += sin(confetti[i].y * 0.1f + i) * 0.3f;

                confetti[i].angle += confetti[i].rotationSpeed; // <--- (*** ����! ***)

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

    // ������ζ�����ע����һ��
    glutTimerFunc(16, onAnimationTimer, 0); // 16ms Լ���� 60 FPS
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

/**
 * @brief (���ع�!) ���Ƴ������ľ�̬���� (������� + ����ݵ�)
 */
void drawLandscapeBackground()
{
    // (��Щ��ɫ����������һ�������ǽ�ʹ������)
    Color skyTop, skyHorizon, groundHorizon, groundNear;

    // ����ȫ�ּ��ڱ���ѡ����ɫ
    switch (currentSeason) {
    case SEASON_SPRING:
        skyTop = COLOR_SKY_SPRING_TOP;
        skyHorizon = COLOR_SKY_SPRING_HORIZON;
        groundHorizon = COLOR_GROUND_SPRING_DARK; // ��ƽ�� (��)
        groundNear = COLOR_GROUND_SPRING_LIGHT; // ������Ļ (��)
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
        groundHorizon = COLOR_GROUND_WINTER_DARK; // ѩ (��Ӱ)
        groundNear = COLOR_GROUND_WINTER_MID;  // ѩ (��)
        break;
    }

    // 1. ���ƽ��䱳�� (���)
    glBegin(GL_QUADS);
    // ���� (�춥��ɫ)
    glColor3f(skyTop.r, skyTop.g, skyTop.b);
    glVertex2f(0, WINDOW_HEIGHT);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    // �ײ� (��ƽ����ɫ)
    glColor3f(skyHorizon.r, skyHorizon.g, skyHorizon.b);
    glVertex2f(WINDOW_WIDTH, 150); // ��ƽ�� Y=150
    glVertex2f(0, 150);
    glEnd();

    // 2. ���ƽ���ݵ� (*** ���޸ģ�������ɽ�� ***)
    glBegin(GL_QUADS);
    // ���� (��ƽ�ߴ�����ɫ�ϰ�)
    glColor3f(groundHorizon.r, groundHorizon.g, groundHorizon.b);
    glVertex2f(0, 150);
    glVertex2f(WINDOW_WIDTH, 150);
    // �ײ� (������Ļ����ɫ����)
    glColor3f(groundNear.r, groundNear.g, groundNear.b);
    glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(0, 0);
    glEnd();
}

void drawBalloon(float x, float y, const Color& color) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    // 1. ������������ (ʹ�������еĺ���)
    glColor3f(color.r, color.g, color.b);
    drawFilledEllipse(15.0f, 20.0f); // �� 15, �� 20

    // 2. ������������
    glColor3f(COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glVertex2f(0.0f, -20.0f); // ����Բ�ײ�
    glVertex2f(0.0f, -60.0f); // ���ӳ���
    glEnd();

    glPopMatrix();
}

/**
 * @brief (��������!) ����ͼ��ݵ�"���ǵ���"
 * (�����û��ṩ�� "��һ����" �Ĵ����񣬲�ʹ�� 800x600 ��������ϵ)
 */
/**
 * @brief (���հ�!) ����ͼ��ݵ�"���ǵ���"
 * (ͼ�� 1: ��ɫǽ�� + ͼ�� 2: ��ɫ���� + ͼ�� 3: ������ο� + ͼ�� 4: ��ɫ�߿�)
 */
void drawLibrary()
{
    // --- 1. ������������� (Our Face) ������ ---
    float frameLeft = 100.0f, frameRight = 350.0f;
    float frameBottom = 150.0f, frameTop = 430.0f;

    // (A) ���������� ����
    float top_L_x1 = 160.0f, top_R_x1 = 230.0f;
    float top_L_x2 = 175.0f, top_R_x2 = 215.0f;
    float top_B = 365.0f;
    
    // (B) "ֱ������" ����
    float midL_T_y = 310.0f, midL_B_y = 270.0f;
    float midL_R_x_top = 115.0f, midL_R_x_bottom = 120.0f;

    // (C) �ײ�Z�� ����
    float z_V1_x = 155.0f, z_V1_y = 310.0f; 
    float z_V2_x = 155.0f, z_V2_y = 265.0f; 
    float z_V3_x = 290.0f, z_V3_y = 310.0f; 
    float z_V4_x = 210.0f, z_V4_y = 285.0f; 
    float z_V5_x = 350.0f, z_V5_y = 340.0f; 
    float z_V6_x = 350.0f, z_V6_y = 285.0f;

    // --- (ͼ�� 1) ����ʵ�Ļ�ɫǽ�� ---
    glColor3f(COLOR_BUILDING_GREY.r, COLOR_BUILDING_GREY.g, COLOR_BUILDING_GREY.b);
    glBegin(GL_POLYGON);
        glVertex2f(frameLeft, frameBottom);
        glVertex2f(frameRight, frameBottom);
        glVertex2f(frameRight, frameTop);
        glVertex2f(frameLeft, frameTop);
    glEnd();

    // --- (ͼ�� 2) ���ư�ɫˮƽ���� ---
    glColor3f(COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b);
    glLineWidth(1.0f); // ������ϸ��
    for (float y = frameBottom + 5.0f; y < frameTop; y += 4.0f) { // (�ҰѲ�����Ϊ 4.0f)
        glBegin(GL_LINES);
            glVertex2f(frameLeft, y);
            glVertex2f(frameRight, y);
        glEnd();
    }

    // --- (ͼ�� 3) ���������ɫ "�ο�" ��� ---
    // (�⽫�����ڻ�ɫǽ��Ͱ�ɫ����֮��)
    
    // (A) ���������� ���
    glBegin(GL_POLYGON);
        glColor3f(COLOR_GLASS_BLUE.r, COLOR_GLASS_BLUE.g, COLOR_GLASS_BLUE.b);
        glVertex2f(top_L_x2, top_B);
        glVertex2f(top_R_x2, top_B);
        glVertex2f(top_R_x1, frameTop);
        glVertex2f(top_L_x1, frameTop);
    glEnd();
    
    // (B) "ֱ������" ���
    glBegin(GL_POLYGON);
        glColor3f(COLOR_GLASS_BLUE.r, COLOR_GLASS_BLUE.g, COLOR_GLASS_BLUE.b);
        glVertex2f(frameLeft, midL_B_y);
        glVertex2f(midL_R_x_bottom, midL_B_y);
        glVertex2f(midL_R_x_top, midL_T_y);
        glVertex2f(frameLeft, midL_T_y);
    glEnd();
    
    // ����� C1 (�����ı���)
    glBegin(GL_POLYGON);
        glColor3f(COLOR_GLASS_BLUE.r, COLOR_GLASS_BLUE.g, COLOR_GLASS_BLUE.b);
        glVertex2f(z_V1_x, z_V1_y); // V1
        glVertex2f(z_V2_x, z_V2_y); // V2
        glVertex2f(z_V4_x, z_V4_y); // V4
        glVertex2f(z_V3_x, z_V3_y); // V3
    glEnd();

    // ����� C2 (�Ҳ���ı���)
    glBegin(GL_POLYGON);
        glColor3f(COLOR_GLASS_BLUE.r, COLOR_GLASS_BLUE.g, COLOR_GLASS_BLUE.b);
        glVertex2f(z_V4_x, z_V4_y); // V4
        glVertex2f(z_V6_x, z_V6_y); // V6
        glVertex2f(z_V5_x, z_V5_y); // V5
        glVertex2f(z_V3_x, z_V3_y); // V3
    glEnd();

    // --- (ͼ�� 4) �������к�ɫ�߿� (�����) ---
    glColor3f(COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b);
    glLineWidth(3.0f);

    // (���)
    glBegin(GL_LINE_LOOP);
        glVertex2f(frameLeft, frameBottom);
        glVertex2f(frameRight, frameBottom);
        glVertex2f(frameRight, frameTop);
        glVertex2f(frameLeft, frameTop);
    glEnd();
    
    // (A) ���������� �ڲ��߿�
    glBegin(GL_LINE_STRIP);
        glVertex2f(top_L_x1, frameTop);
        glVertex2f(top_L_x2, top_B);
        glVertex2f(top_R_x2, top_B);
        glVertex2f(top_R_x1, frameTop);
    glEnd();
    
    // (B) "ֱ������" �ڲ��߿�
    glBegin(GL_LINE_STRIP);
        glVertex2f(frameLeft, midL_T_y);
        glVertex2f(midL_R_x_top, midL_T_y);
        glVertex2f(midL_R_x_bottom, midL_B_y);
        glVertex2f(frameLeft, midL_B_y);
    glEnd();
    
    // (C) �ײ�Z�� �����߿�
    glBegin(GL_LINE_LOOP);
        glVertex2f(z_V1_x, z_V1_y);
        glVertex2f(z_V2_x, z_V2_y);
        glVertex2f(z_V4_x, z_V4_y);
        glVertex2f(z_V6_x, z_V6_y);
        glVertex2f(z_V5_x, z_V5_y);
        glVertex2f(z_V3_x, z_V3_y);
    glEnd();

    // �ָ�Ĭ���߿�
    glLineWidth(1.0f);
}


/**
 * @brief (���ع�!) �����漾�ڱ仯���� (���������ɺͷֲ�����)
 * @param x ���ɵ����� x ����
 * @param y ���ɵĵײ� y ����
 */
void drawSeasonalTree(float x, float y)
{
    // 1. �������� (ʹ�ý�����ģ�������)
    glPushMatrix();
    glTranslatef(x, y, 0); // ��ԭ���Ƶ����ɵײ�����

    glBegin(GL_QUADS);
    // ��� (��)
    glColor3f(COLOR_TRUNK_DARK.r, COLOR_TRUNK_DARK.g, COLOR_TRUNK_DARK.b);
    glVertex2f(-7.0f, 0.0f);   // �ײ���
    glVertex2f(-5.0f, 50.0f);  // ������
    // �Ҳ� (��) - ��΢������б��ģ�����ɵ�����
    glColor3f(COLOR_TRUNK_LIGHT.r, COLOR_TRUNK_LIGHT.g, COLOR_TRUNK_LIGHT.b);
    glVertex2f(5.0f, 50.0f);   // ������
    glVertex2f(7.0f, 0.0f);    // �ײ���
    glEnd();
    glPopMatrix();

    // 2. ���ݼ��ڻ������ڻ���֦ (ʹ��ͼ����Ӻ͸��ḻ������)
    switch (currentSeason)
    {
    case SEASON_SPRING:
        glPushMatrix();
        glTranslatef(x, y + 60.0f, 0); // ��ԭ���Ƶ����ڵײ�����
        // (���, ����)
        glColor3f(COLOR_LEAF_SPRING_DARK.r, COLOR_LEAF_SPRING_DARK.g, COLOR_LEAF_SPRING_DARK.b);
        drawFilledEllipse(35.0f, 30.0f); // �ײ����ƬҶ��
        // (�в�, ���̣���΢���Ϻ���ƫ�ƣ�ģ����������)
        glColor3f(COLOR_LEAF_SPRING_MID.r, COLOR_LEAF_SPRING_MID.g, COLOR_LEAF_SPRING_MID.b);
        glTranslatef(7.0f, 7.0f, 0.0f);
        drawFilledEllipse(30.0f, 25.0f);
        // (ǰ��, ���̣�ģ����ѿ�����ܹⲿ��)
        glColor3f(COLOR_LEAF_SPRING_LIGHT.r, COLOR_LEAF_SPRING_LIGHT.g, COLOR_LEAF_SPRING_LIGHT.b);
        glTranslatef(-15.0f, 5.0f, 0.0f);
        drawFilledEllipse(25.0f, 20.0f);
        glPopMatrix();

        // ����С�� (�����ϲ�)
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
        // (���, ����ɫ����ïʢ)
        glColor3f(COLOR_LEAF_SUMMER_DARK.r, COLOR_LEAF_SUMMER_DARK.g, COLOR_LEAF_SUMMER_DARK.b);
        drawFilledEllipse(40.0f, 35.0f);
        // (�в�, ����ɫ)
        glColor3f(COLOR_LEAF_SUMMER_MID.r, COLOR_LEAF_SUMMER_MID.g, COLOR_LEAF_SUMMER_MID.b);
        glTranslatef(8.0f, 8.0f, 0.0f);
        drawFilledEllipse(35.0f, 30.0f);
        // (ǰ��, ����ɫ)
        glColor3f(COLOR_LEAF_SUMMER_LIGHT.r, COLOR_LEAF_SUMMER_LIGHT.g, COLOR_LEAF_SUMMER_LIGHT.b);
        glTranslatef(-18.0f, 6.0f, 0.0f);
        drawFilledEllipse(30.0f, 25.0f);
        glPopMatrix();
        break;

    case SEASON_AUTUMN:
        glPushMatrix();
        glTranslatef(x, y + 60.0f, 0);
        // (���, ����)
        glColor3f(COLOR_LEAF_AUTUMN_DARK.r, COLOR_LEAF_AUTUMN_DARK.g, COLOR_LEAF_AUTUMN_DARK.b);
        drawFilledEllipse(35.0f, 30.0f);
        // (�в�, ����)
        glColor3f(COLOR_LEAF_AUTUMN_MID.r, COLOR_LEAF_AUTUMN_MID.g, COLOR_LEAF_AUTUMN_MID.b);
        glTranslatef(7.0f, 7.0f, 0.0f);
        drawFilledEllipse(30.0f, 25.0f);
        // (ǰ��, ��ɫ)
        glColor3f(COLOR_LEAF_AUTUMN_LIGHT.r, COLOR_LEAF_AUTUMN_LIGHT.g, COLOR_LEAF_AUTUMN_LIGHT.b);
        glTranslatef(-15.0f, 5.0f, 0.0f);
        drawFilledEllipse(25.0f, 20.0f);
        glPopMatrix();
        break;

    case SEASON_WINTER:
        // ���Ƹ��ܼ�����¶��֦
        glColor3f(COLOR_TRUNK_DARK.r, COLOR_TRUNK_DARK.g, COLOR_TRUNK_DARK.b); // ��֦��ɫ�����ɰ���һ��
        glLineWidth(2.5f); // ��΢��һ��
        glPushMatrix();
        glTranslatef(x, y + 40.0f, 0); // �����ɶ�����ʼ

        glBegin(GL_LINES);
        // ���ɷֲ� (����Ȼ)
        glVertex2f(0.0f, 0.0f); glVertex2f(-12.0f, 25.0f);
        glVertex2f(0.0f, 0.0f); glVertex2f(12.0f, 25.0f);

        // ����֧
        glVertex2f(-12.0f, 25.0f); glVertex2f(-20.0f, 40.0f);
        glVertex2f(-12.0f, 25.0f); glVertex2f(-5.0f, 45.0f);

        glVertex2f(-20.0f, 40.0f); glVertex2f(-25.0f, 50.0f);
        glVertex2f(-20.0f, 40.0f); glVertex2f(-15.0f, 48.0f);

        // �Ҳ��֧
        glVertex2f(12.0f, 25.0f); glVertex2f(20.0f, 40.0f);
        glVertex2f(12.0f, 25.0f); glVertex2f(5.0f, 45.0f);

        glVertex2f(20.0f, 40.0f); glVertex2f(25.0f, 50.0f);
        glVertex2f(20.0f, 40.0f); glVertex2f(15.0f, 48.0f);
        glEnd();
        glPopMatrix();
        glLineWidth(1.0f); // �ָ�Ĭ���߿�
        break;
    }
}

/**
 * @brief (����д������!) ����Сè
 * @param x Сè������ x ����
 * @param y Сè�Ųȵ� *����* y ���� (���� 150.0f)
 * @param animationTime �����Ȳ��ڶ��Ķ���ʱ��
 */
void drawCat(float x, float y, float animationTime) {
    glPushMatrix();
    glTranslatef(x, y, 0); // 1. ��ԭ���ƶ���Сè�ĵ������� (x, y)

    // --- ����������� ---
    float legLength = 12.0f;
    float legWidth = 4.0f;
    float bodyRadiusY = 15.0f;
    float bodyRadiusX = 25.0f;

    // (*** �ؼ��޸�! ***)
    // ��������� Y Ӧ�ø��ͣ��Ա������ص�
    float bodyCenterY = legLength + (bodyRadiusY * 0.6f); // (12 + 9 = 21)

    // --- ��ɫ ---
    Color catColor = COLOR_CAT_BODY;

    // --- 4. ���������� (��Ե���) ---
    glColor3f(COLOR_CAT_PAW.r, COLOR_CAT_PAW.g, COLOR_CAT_PAW.b);
    float legOscillation = sin(animationTime * 5.0f);
    float legOscillationInv = sin(animationTime * 5.0f + M_PI);

    // (�ȵ��߼�����)
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


    // --- 1. �������� (�����Ϸ�) ---
    glColor3f(catColor.r, catColor.g, catColor.b);
    glPushMatrix();
    // (*** ��ʹ��������� bodyCenterY ***)
    glTranslatef(0.0f, bodyCenterY, 0.0f);
    drawFilledEllipse(bodyRadiusX, bodyRadiusY);
    glPopMatrix();


    // --- 2. ����ͷ�� (��Ե���) ---
    // (*** ��ʹ��������� bodyCenterY ***)
    float headCenterY = bodyCenterY + 8.0f;

    glPushMatrix();
    glTranslatef(20.0f, headCenterY, 0.0f);

    // (ͷ�������߼�����)
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

    glPopMatrix(); // ����ͷ���任


    // --- 3. ����β�� (��Ե���) ---
    glColor3f(catColor.r, catColor.g, catColor.b);
    glPushMatrix();
    // (*** ��ʹ��������� bodyCenterY ***)
    glTranslatef(-25.0f, bodyCenterY, 0); // β�͸���
    glRotatef(sin(animationTime * 2.0f) * 15.0f, 0, 0, 1);
    drawFilledEllipse(8.0f, 4.0f);

    glTranslatef(-10.0f, -5.0f, 0);
    glRotatef(sin(animationTime * 2.0f) * 15.0f, 0, 0, 1);
    drawFilledEllipse(8.0f, 4.0f);
    glPopMatrix();


    glPopMatrix(); // ����Сè��
}


void drawSun() {
    // (��!) ��ȡʱ�䣬���ڼ�����˸
    float time = glutGet(GLUT_ELAPSED_TIME) * 0.005f;
    // (��!) ������˸ (�� 90% �� 110% ��С֮������)
    float shimmer = 1.0f + sin(time * 2.0f) * 0.1f;

    // �������Ͻ�
    glPushMatrix();
    glTranslatef(80.0f, WINDOW_HEIGHT - 80.0f, 0.0f);

    // (�������� drawFilledEllipse ����)
    glColor3f(COLOR_SUN_YELLOW.r, COLOR_SUN_YELLOW.g, COLOR_SUN_YELLOW.b);
    drawFilledEllipse(40.0f, 40.0f); // ̫�����屣�ֲ���

    // ���ƹ�â (Ӧ����˸)
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < 12; ++i) {
        float angle = 2.0f * M_PI * i / 12.0f;
        float outer_r = 60.0f * shimmer; // (��!) ��â���ⲿ�뾶�ڱ仯

        glVertex2f(cos(angle) * 45.0f, sin(angle) * 45.0f);
        glVertex2f(cos(angle) * outer_r, sin(angle) * outer_r); // (��!) ʹ�ñ仯��İ뾶
    }
    glEnd();
    glLineWidth(1.0f);

    glPopMatrix();
}


void drawWind() {
    // ʹ�ð�ɫϸ��
    glColor3f(COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < NUM_WIND_LINES; ++i) {
        glVertex2f(windLines[i].x, windLines[i].y);
        glVertex2f(windLines[i].x - windLines[i].length, windLines[i].y); // ����
    }
    glEnd();
}

void drawSnow() {
    // ʹ�ð�ɫС���� (�� drawFilledEllipse ����)
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
    // 1. �������ӱ��� (ʹ���������ƥ�䳡��)
    glColor3f(COLOR_DARK_BLUE.r, COLOR_DARK_BLUE.g, COLOR_DARK_BLUE.b);
    glBegin(GL_POLYGON);
    glVertex2f(cx - width / 2.0f, cy - height / 2.0f);
    glVertex2f(cx + width / 2.0f, cy - height / 2.0f);
    glVertex2f(cx + width / 2.0f, cy + height / 2.0f);
    glVertex2f(cx - width / 2.0f, cy + height / 2.0f);
    glEnd();

    // 2. �������ӱ߿� (ʹ�ð�ɫ��ƥ������)
    glColor3f(COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(cx - width / 2.0f, cy - height / 2.0f);
    glVertex2f(cx + width / 2.0f, cy - height / 2.0f);
    glVertex2f(cx + width / 2.0f, cy + height / 2.0f);
    glVertex2f(cx - width / 2.0f, cy + height / 2.0f);
    glEnd();
    glLineWidth(1.0f); // �ָ�Ĭ���߿�

    // 3. �������� (��ɫ, ����)
    float textScale = 0.18f; // ���������ֵ�����
    float textStroke = 2.0f;

    // ���� Y ������ʵ�ִ�ֱ���� (��Ϊ drawTextStrokeCentered �ǻ��ڻ��߶����)
    float textY_baseline = cy - (textScale * 25.0f); // ����һ�����ڴ�ֱ���еĽ���ֵ

    drawTextStrokeCentered(cx, textY_baseline, text, textScale, textStroke, COLOR_WHITE);
}

/**
 * @brief (���ع�!) �������ֽм (��������ת�ľ���)
 */
void drawConfetti() {

    for (int i = 0; i < NUM_CONFETTI; ++i) {
        float w = confetti[i].width;
        float h = confetti[i].height;
        float x = confetti[i].x;
        float y = confetti[i].y;

        glPushMatrix(); // �������

        // 1. �ƶ������ֽм��λ��
        glTranslatef(x, y, 0.0f);
        // 2. ��������������ת
        glRotatef(confetti[i].angle, 0.0f, 0.0f, 1.0f);

        // 3. ������ɫ
        glColor3f(confetti[i].color.r, confetti[i].color.g, confetti[i].color.b);

        // 4. ����һ�� *��(0,0)Ϊ����* �ľ���
        glBegin(GL_QUADS);
        glVertex2f(-w / 2.0f, -h / 2.0f);
        glVertex2f(w / 2.0f, -h / 2.0f);
        glVertex2f(w / 2.0f, h / 2.0f);
        glVertex2f(-w / 2.0f, h / 2.0f);
        glEnd();

        glPopMatrix(); // �ָ�����
    }
}

void drawStarfield() {
    // (��!) ��ȡʱ�䣬���ڼ�����˸
    float time = glutGet(GLUT_ELAPSED_TIME) * 0.001f; // (ʹ�� 0.001 ����˸����)

    // (*** �ؼ�! ***)
    // ���Ǳ������� BLEND ������͸������Ч
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    for (int i = 0; i < NUM_STARS; ++i) {
        float s = stars[i].size;
        float x = stars[i].x;
        float y = stars[i].y;

        // (��!) ��������/͸���� (ʹ�� sin ������ 0.2 �� 1.0 ֮��ƽ������)
        float brightness = 0.6f + (sin(time + stars[i].phase) + 1.0f) * 0.2f;

        // (ʹ�ô�͸���ȵ���ɫ)
        glColor4f(1.0f, 1.0f, 0.8f, brightness); // ��͵Ļư�ɫ

        glVertex2f(x - s, y - s);
        glVertex2f(x + s, y - s);
        glVertex2f(x + s, y + s);
        glVertex2f(x - s, y + s);
    }
    glEnd();

    // (��ѡ�����Ƽ�) �ر� BLEND���Է���Ӱ��������͸���Ļ���
    glDisable(GL_BLEND);
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

    // --- 1. ���ƾ�̬���� (��Ͳ�) ---
    drawLandscapeBackground();

    // --- 2. ����̫�� (���ļ��������) ---
    if (currentSeason == SEASON_SUMMER) {
        drawSun();
    }

    // --- 3. ���ƾ�̬�羰 (��Сè����) ---
    drawLibrary();
    drawSeasonalTree(700.0f, 150.0f);

    // --- 4. ���Ʒ��ѩ (����Ч������Сè����) ---
    if (currentSeason == SEASON_AUTUMN) {
        drawWind();
    }
    if (currentSeason == SEASON_WINTER) {
        drawSnow();
    }

    // --- 5. (��ͼ��!) ����Сè ---
    // (�������������ͼ��ݺ�����ǰ��)
    drawCat(scene2Cat.x, scene2Cat.y, scene2Cat.animationTime);

    // --- 6. ������������ (��̬������ǰ��) ---
    if (currentSeason == SEASON_SPRING || currentSeason == SEASON_SUMMER)
    {
        for (int i = 0; i < NUM_BALLOONS; ++i) {
            drawBalloon(balloons[i].x, balloons[i].y, balloons[i].color);
        }
    }

    // --- 7. �������� (UI���������) ---
    drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 540.0f, "20 Years of Dedication", 0.15f, 3.0f, COLOR_WHITE);
    drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 520.0f, "Walk through four seasons with you...", 0.15f, 3.0f, COLOR_WHITE);
    Color instructionColor = (currentSeason == SEASON_WINTER) ? COLOR_BLACK : COLOR_WHITE;
    drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 30.0f, "Press 'n' for next season | 'Space' to continue", 0.1f, 2.0f, instructionColor);
}


// Scene3: Cake
void drawSceneBridge() {
    // 1. ���Ʊ��� (�����)
    glColor3f(0.05f, 0.05f, 0.1f);
    glBegin(GL_POLYGON);
    glVertex2f(0, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();

    drawStarfield();
    // 2. �������� (*** ���޸�Ϊ��̬ ***)

        // (����ʹ�� LERP: B(t) = (1-t)*P0 + t*P1)
    float t = mergeProgress;
    float one_minus_t = 1.0f - t;

    // ���������ӵĵ�ǰ X
    float currentLeftX = one_minus_t * signStartX_L + t * signTargetX;
    // ���������ӵĵ�ǰ X
    float currentRightX = one_minus_t * signStartX_R + t * signTargetX;

    if (scene3State == S3_MERGED) {
        // --- ״̬ 3: �������� ---
        // ֻ����һ�� "XJTLU" ����
        drawSign(signTargetX, signY, signWidth, signHeight, "XJTLU");
    }
    else {
        // --- ״̬ 1 �� 2: ��ʼ & �ƶ��� ---
        // ���������ƶ��е�����
        drawSign(currentLeftX, signY, signWidth, signHeight, "Suzhou");
        drawSign(currentRightX, signY, signWidth, signHeight, "Liverpool");
    }

    // 4. ���� Slogan
    if (scene3State == S3_INITIAL) {
        drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 412.0f, "Years of", 0.2f, 2.0f, COLOR_WHITE);
        drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 390.0f, "Connecting Worlds", 0.2f, 2.0f, COLOR_WHITE);
        drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 25.0f, "Press 'r' to Go Back | Press 'c' to continue", 0.1f, 2.0f, COLOR_WHITE);
    }
    else if (scene3State == S3_MERGING) {
        // ״̬ 2: �����ں�ʱ (���ְ���)
        drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 412.0f, "Years of", 0.2f, 2.0f, COLOR_WHITE);
        drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 390.0f, "Connecting Worlds", 0.2f, 2.0f, COLOR_WHITE);

    }
    else if (scene3State == S3_MERGED) {
        // ״̬ 3: ����������
        drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 350.0f, "Happy 20th Anniversary!", 0.25f, 3.0f, COLOR_WHITE);
        drawTextStrokeCentered(WINDOW_WIDTH / 2.0f, 50.0f, "Press 'r' to Go Back", 0.12f, 2.0f, COLOR_WHITE);
    }

    // 5. ������������ (�������� *����* "20")
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < NUM_PARTICLES_S3; ++i) {
        // (��ѡ: ֻ���ڿ�ʼ�ƶ���Ż���)
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
// --- �滻����Ŀ��ԭ���� 'display' ���� ---
void display(void) {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
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
    case SCENE_BRIDGE:
        drawSceneBridge();      // (����������ռλ��)
        break;
    }
    glDisable(GL_BLEND);
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
 * ���̻ص� (*** ����������ȫ ***)
 */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {

    case ' ':
        if (currentScene == SCENE_LANDSCAPE) {
            currentScene = SCENE_BRIDGE;
            initBalloons();
            initParticles();
            initConfetti();

            // (��!) ���ó�������״̬
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
            // ���� ����2
            currentSeason = SEASON_SPRING;
            initBalloons();
            initWindLines();
            initSnowflakes();

            glutPostRedisplay();
        }
        else if (currentScene == SCENE_LANDSCAPE) {
            // (���ó���2���߼�)
            currentSeason = SEASON_SPRING;
            initBalloons();
            initWindLines();
            initSnowflakes();
            initCat();
            glutPostRedisplay();
        }
        // ... (�����ܻ��� SCENE_COVER �������߼�)
        break;

        // --- ( 'c' ������) ---
    case 'c':
    case 'C':
        // ֻ���ڳ�����, ������ "��ʼ" ״̬ʱ�Ŵ���
        if (currentScene == SCENE_BRIDGE && scene3State == S3_INITIAL) {
            scene3State = S3_MERGING;
            // (onAnimationTimer ���Զ�������, ���� PostRedisplay)
        }
        break;

    case 27:
        exit(0);
        break;

    case 'n':
    case 'N':
        // --- (*** �Ѳ�ȫ! ***) ---
        if (currentScene == SCENE_LANDSCAPE) {

            // ��˳��ѭ��
            switch (currentSeason) {
            case SEASON_SPRING:
                currentSeason = SEASON_SUMMER;
                break;
            case SEASON_SUMMER:
                currentSeason = SEASON_AUTUMN;
                initWindLines(); // �л����＾ʱ����ʼ����
                break;
            case SEASON_AUTUMN:
                currentSeason = SEASON_WINTER;
                initSnowflakes(); // �л�������ʱ����ʼ��ѩ
                break;
            case SEASON_WINTER:
                currentSeason = SEASON_SPRING;
                initBalloons(); // �л��ش���ʱ����ʼ������
                break;
            }

            glutPostRedisplay();
        }
        break;
    }
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

    glShadeModel(GL_SMOOTH);
    // --- ��ʼ������һ�ı��� ---
    envelopeFlapY = flapStartY;
    srand(static_cast<unsigned int>(time(NULL))); // ��ʼ�����������
    initBalloons(); // ��ʼ���������ʼλ��
    initSnowflakes();
    initWindLines();
    initCat();
    initParticles();
    initConfetti();
    initStars();

    // --- ע��ص� ---
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glutPassiveMotionFunc(passiveMotion); // <--- �����һ��
    glutTimerFunc(0, onAnimationTimer, 0); // ����������ʱ��

    glutMainLoop();
    return 0;
}