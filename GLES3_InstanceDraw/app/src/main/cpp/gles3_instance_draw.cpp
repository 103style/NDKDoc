#include <jni.h>
#include <string>

#include "LogUtils.h"
#include "esUtil.h"
#include <android/log.h>

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <math.h>

#define NUM_INSTANCES 100
#define POS_LOC 0
#define COLOR_LOC 1
#define MVP_LOC 2


auto vertex_shader_source =
        "#version 300 es \n"
        "layout(location = 0) in vec4 a_position;\n"
        "layout(location = 1) in vec4 a_color;\n"
        "layout(location = 2) in mat4 a_mvpMatrix;\n"
        "out vec4 v_color;\n"
        "void main() { \n"
        "   v_color = a_color;\n"
        "   gl_Position = a_position * a_mvpMatrix;\n"
        "}\n";

auto fragment_shader_source =
        "#version 300 es \n"
        "precision mediump float;\n"
        "in vec4 v_color;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "void main() { \n"
        "   outColor = v_color;\n"
        "}\n";


/**
 * 顶点缓冲区对象
 */
GLuint posVBO;
GLuint colorVBO;
GLuint mvpVBO;
GLuint indicesIBO;

/**
 * 索引
 */
int numIndices;

/**
 * 每个图像的旋转角度
 */
GLfloat angels[NUM_INSTANCES];


static void printGlString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGD("GL %s: %s\n", name, v);
}


GLuint createShader(GLenum shaderType, const char *src) {
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        LOGE("%s shader create fail ",
             shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment");
        return 0;
    }

    glShaderSource(shader, 1, &src, nullptr);

    GLint compileRes = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileRes);

    if (!compileRes) {
        GLint logLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

        if (logLen > 0) {
            char *logInfo = static_cast<char *>(malloc(sizeof(char) * logLen));
            if (logInfo) {
                glGetShaderInfoLog(shader, logLen, nullptr, logInfo);
                LOGE("%s shader compile fail --- %s",
                     shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment", logInfo);
                free(logInfo);
            }
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}


/**
 * 着色器程序
 */
GLuint program;

void linkProgram() {
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertex_shader_source);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragment_shader_source);

    program = glCreateProgram();
    if (!program) {
        LOGE("create program error!")
        return;
    }

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus) {
        GLint logLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            char *logInfo = static_cast<char *>(malloc(sizeof(char) * logLen));
            glGetProgramInfoLog(program, logLen, nullptr, logInfo);
            LOGE("linkProgram error --- %s", logInfo);
            free(logInfo);
            glDeleteProgram(program);
        }
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


int genCube(float scale, GLfloat **vertices, GLuint **indices) {
    int i;
    int numVertices = 24;
    int numIndices = 36;

    GLfloat cubeVerts[] =
            {
                    -0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f, 0.5f,
                    0.5f, -0.5f, 0.5f,
                    0.5f, -0.5f, -0.5f,
                    -0.5f, 0.5f, -0.5f,
                    -0.5f, 0.5f, 0.5f,
                    0.5f, 0.5f, 0.5f,
                    0.5f, 0.5f, -0.5f,
                    -0.5f, -0.5f, -0.5f,
                    -0.5f, 0.5f, -0.5f,
                    0.5f, 0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f, 0.5f,
                    -0.5f, 0.5f, 0.5f,
                    0.5f, 0.5f, 0.5f,
                    0.5f, -0.5f, 0.5f,
                    -0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f, 0.5f,
                    -0.5f, 0.5f, 0.5f,
                    -0.5f, 0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f, 0.5f,
                    0.5f, 0.5f, 0.5f,
                    0.5f, 0.5f, -0.5f,
            };

    if (vertices != nullptr) {
        *vertices = static_cast<GLfloat *>(malloc(sizeof(GLfloat) * 3 * numVertices));
        memcpy(*vertices, cubeVerts, sizeof(cubeVerts));

        for (i = 0; i < numVertices * 3; i++) {
            (*vertices)[i] *= scale;
        }
    }

    if (indices != nullptr) {
        GLuint cubeIndices[] =
                {
                        0, 2, 1,
                        0, 3, 2,
                        4, 5, 6,
                        4, 6, 7,
                        8, 9, 10,
                        8, 10, 11,
                        12, 15, 14,
                        12, 14, 13,
                        16, 17, 18,
                        16, 18, 19,
                        20, 23, 22,
                        20, 22, 21
                };

        *indices = static_cast<GLuint *>(malloc(sizeof(GLuint) * numIndices));
        memcpy(*indices, cubeIndices, sizeof(cubeIndices));
    }

    return numIndices;
}


void init() {
    printGlString("Version", GL_VERSION);
    printGlString("Vendor", GL_VENDOR);
    printGlString("Renderer", GL_RENDERER);
    printGlString("Extensions", GL_EXTENSIONS);

    linkProgram();

    if (!program) {
        return;
    }

    GLfloat *positions;
    GLuint *indices;
    genCube(0.1f, &positions, &indices);

    glGenBuffers(1, &indicesIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * numIndices, indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    free(indices);


    glGenBuffers(1, &posVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(GLfloat) * 3, positions, GL_STATIC_DRAW);
    free(positions);


    {
        GLubyte colors[NUM_INSTANCES][4];
        int instance;
        srand(0);
        for (instance = 0; instance < NUM_INSTANCES; instance++) {
            colors[instance][0] = random() % 255;
            colors[instance][1] = random() % 255;
            colors[instance][2] = random() % 255;
            colors[instance][3] = 0;
        }
        glGenBuffers(1, &colorVBO);
        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glBufferData(GL_ARRAY_BUFFER, NUM_INSTANCES * 4, colors, GL_STATIC_DRAW);
    }


    {
        int instance;
        for (instance = 0; instance < NUM_INSTANCES; instance++) {
            angels[instance] = (float) (random() % 32768) / 32767.0f * 360.0f;
        }
        glGenBuffers(1, &mvpVBO);
        glBindBuffer(GL_ARRAY_BUFFER, mvpVBO);
        glBufferData(GL_ARRAY_BUFFER, NUM_INSTANCES * sizeof(ESMatrix), nullptr, GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}


extern "C" JNIEXPORT void JNICALL
Java_com_lxk_gles3_1instancedraw_InstanceDrawRenderer_onSurefaceCreated(JNIEnv *env, jobject thiz) {
    init();
}

int w, h;


void update() {
    ESMatrix *matrixBuf;
    ESMatrix perspective;
    float aspect;
    int instance = 0;
    int numRows;
    int numColumns;

    aspect = w / h;
    esMatrixLoadIdentity(&perspective);

    esPerspective(&perspective, 60.0f, aspect, 1.0f, 20.0f);

    glBindBuffer(GL_ARRAY_BUFFER, mvpVBO);
    matrixBuf = (ESMatrix *) glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(ESMatrix) * NUM_INSTANCES,
                                              GL_MAP_WRITE_BIT);

    // Compute a per-instance MVP that translates and rotates each instance differnetly
    numRows = (int) sqrtf(NUM_INSTANCES);
    numColumns = numRows;

    for (instance = 0; instance < NUM_INSTANCES; instance++) {
        ESMatrix modelview;
        float translateX = ((float) (instance % numRows) / (float) numRows) * 2.0f - 1.0f;
        float translateY = ((float) (instance / numColumns) / (float) numColumns) * 2.0f - 1.0f;

        // Generate a model view matrix to rotate/translate the cube
        esMatrixLoadIdentity(&modelview);

        // Per-instance translation
        esTranslate(&modelview, translateX, translateY, -2.0f);

        // Compute a rotation angle based on time to rotate the cube
        angels[instance] += (2.0 * 40.0f);

        if (angels[instance] >= 360.0f) {
            angels[instance] -= 360.0f;
        }

        // Rotate the cube
        esRotate(&modelview, angels[instance], 1.0, 0.0, 1.0);

        // Compute the final MVP by multiplying the
        // modevleiw and perspective matrices together
        esMatrixMultiply(&matrixBuf[instance], &modelview, &perspective);
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
}


void Draw() {
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the program object
    glUseProgram(program);

    // Load the vertex position
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glVertexAttribPointer(POS_LOC, 3, GL_FLOAT,
                          GL_FALSE, 3 * sizeof(GLfloat), (const void *) NULL);
    glEnableVertexAttribArray(POS_LOC);

    // Load the instance color buffer
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glVertexAttribPointer(COLOR_LOC, 4, GL_UNSIGNED_BYTE,
                          GL_TRUE, 4 * sizeof(GLubyte), (const void *) NULL);
    glEnableVertexAttribArray(COLOR_LOC);
    glVertexAttribDivisor(COLOR_LOC, 1); // One color per instance


    // Load the instance MVP buffer
    glBindBuffer(GL_ARRAY_BUFFER, mvpVBO);

    // Load each matrix row of the MVP.  Each row gets an increasing attribute location.
    glVertexAttribPointer(MVP_LOC + 0, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix),
                          (const void *) nullptr);
    glVertexAttribPointer(MVP_LOC + 1, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix),
                          (const void *) (sizeof(GLfloat) * 4));
    glVertexAttribPointer(MVP_LOC + 2, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix),
                          (const void *) (sizeof(GLfloat) * 8));
    glVertexAttribPointer(MVP_LOC + 3, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix),
                          (const void *) (sizeof(GLfloat) * 12));
    glEnableVertexAttribArray(MVP_LOC + 0);
    glEnableVertexAttribArray(MVP_LOC + 1);
    glEnableVertexAttribArray(MVP_LOC + 2);
    glEnableVertexAttribArray(MVP_LOC + 3);

    // One MVP per instance
    glVertexAttribDivisor(MVP_LOC + 0, 1);
    glVertexAttribDivisor(MVP_LOC + 1, 1);
    glVertexAttribDivisor(MVP_LOC + 2, 1);
    glVertexAttribDivisor(MVP_LOC + 3, 1);

    // Bind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesIBO);

    // Draw the cubes
    glDrawElementsInstanced(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, (const void *) nullptr,
                            NUM_INSTANCES);
}


void Shutdown() {
    glDeleteBuffers(1, &posVBO);
    glDeleteBuffers(1, &colorVBO);
    glDeleteBuffers(1, &mvpVBO);
    glDeleteBuffers(1, &indicesIBO);

    //删除着色器程序
    glDeleteProgram(program);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lxk_gles3_1instancedraw_InstanceDrawRenderer_onSurfaceChanged(JNIEnv *env, jobject thiz,
                                                                       jint width, jint height) {
    w = width;
    h = height;
    glViewport(0, 0, w, h);
}

extern "C" JNIEXPORT void JNICALL
Java_com_lxk_gles3_1instancedraw_InstanceDrawRenderer_onDrawFrame(JNIEnv *env, jobject thiz) {

    update();
    Draw();
}

