#pragma once

#include "Common.h"

#include "Vector4f.h"
#include "Object3D.h"
#include "Camera.h"
#include "Light.h"

namespace libMath {
    class Render {
        private:
            int width, height;
            char** buffer;

        public:
            Render(int width, int height) : width(width), height(height) {
                buffer = new char*[height];

                for(int i = 0; i < height; ++i) 
                    buffer[i] = new char[width];

                resetBuffer();
            }

            ~Render() {
                for(int i = 0; i < height; ++i)
                    delete [] buffer[i];
                delete [] buffer;
            }

            void putPixel(libMath::vector4f position) {
                int x = (int)(position.x) + width/2;
                int y = (int)(position.y) + height/2;
       
                if (x >= 0 && x < width && y >= 0 && y < height) {
                    buffer[y][x] = '1';
                }
            }

            void resetBuffer() {
                for(int i = 0; i < height; ++i){
                    for(int j = 0; j < width; ++j){
                        buffer[i][j] = '0';
                    }
                }
            }

            void draw() {
                for(int i = 0; i < height; ++i) {
                    for(int j = 0; j < width; ++j) {
                        if (buffer[i][j] == '0') {
                            std::cout << " ";
                        } else {
                            std::cout << ".";
                        }
                    }
                    std::cout << std::endl;
                }
            }

            void plotLine(vector4f v1, vector4f v2) {
                float dx = abs(v2.x - v1.x);
                int sx = (v1.x < v2.x) ? 1 : -1;
                float dy = -abs(v2.y - v1.y);
                int sy = (v1.y < v2.y) ? 1 : -1;
                float error = dx + dy + 0.25f;

                while (true) {
                    putPixel(v1);
                    if (v1.x == v2.x && v1.y == v2.y) break;

                    float e2 = 2 * error;
                    if (e2 >= dy) {
                        if (v1.x == v2.x) break;
                        error += dy;
                        v1.x += (float)sx;
                    }

                    if (e2 <= dx) {
                        if (v1.y == v2.y) break;
                        error += dx;
                        v1.y += (float) sy;
                    }
                }
            }
    };
}

/**
 * @class Render
 * @brief The Render class represents a rendering engine for graphics programming.
 * 
 * This class provides functionality to initialize OpenGL, manage objects and cameras,
 * and perform rendering operations.
 */
class Render{
    public:
        static inline Render* r; /**< The Render instance. */

        GLFWwindow* window; /**< The GLFW window used for rendering. */
        std::vector<Object3D*> objectList; /**< The list of 3D objects to be rendered. */
        std::vector<Light*> lightList; /**< The list of lights to be rendered. */
        std::vector<Object3D*> nextObjectList; /**< The list of 3D objects to be rendered in the next frame. */

        /**
         * @struct bufferObject_t
         * @brief A struct representing a buffer object.
         */
        typedef struct {
            unsigned int bufferID; /**< The ID of the buffer object. */
            unsigned int vertexBufferID; /**< The ID of the vertex buffer object. */
            unsigned int edgeBufferID; /**< The ID of the edge buffer object. */
        } bufferObject_t;

        std::map<int, bufferObject_t> bufferObjectList; /**< The map of buffer objects. */

        Camera* cam; /**< The camera used for rendering. */

        Render();

        bool gameOver = false;

        /**
         * @brief Initializes the OpenGL context and creates a window for rendering.
         * @param windowName The name of the window.
         * @param sizeX The width of the window.
         * @param sizeY The height of the window.
         */
        void initGL(const char* windowName, int sizeX, int sizeY);

        /**
         * @brief Adds an object to the list of objects to be rendered.
         * @param obj The object to be added.
         */
        void putObject(Object3D* obj);

        /**
         * @brief Sets the camera to be used for rendering.
         * @param cam The camera to be set.
         */
        void putCamera(Camera* cam);

        void putLight(Light* light);

        void moveLights(float deltaTime);

        /**
         * @brief Sets up the object for rendering.
         * @param obj The object to be set up.
         */
        void drawGL(int objectPosition, const std::vector<int>& lightPositions);

        void setupObject(Object3D* obj);

        void move(float deltaTime);

        void drawObjects();

        bool collisionType(Object3D* obj, int objType);

        bool cameraCollision(int objType);

        void deleteObject(Object3D* obj);

        /**
         * @brief Starts the main rendering loop.
         */
        void mainLoop();
};
