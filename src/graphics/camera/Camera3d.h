#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>

#include "../platform/InputManager.h"

namespace Lengine {
    class Camera3d {
    public:
        Camera3d();
        ~Camera3d();
        void init(float width, float height, InputManager* inputManager, glm::vec3 cameraPos, float FOV);
         const glm::vec3& getCameraPosition() { return position; }
         const glm::vec3& getCameraDirection() { return front; }

         glm::mat4 getViewMatrix();
         glm::mat4 getProjectionMatrix();
         glm::vec3 getRightVector();
         glm::vec3 getForwardVector();
        void update(const float& deltaTime, const glm::vec2& mouseCoords, const bool& fixCam);
        void moveMouse( float xoffset,  float yoffset);
        void moveKeyboard(const float& speed);


    private:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        float yaw, pitch;
        float fov;
        float aspectRatio;

        glm::vec3 direction;
        InputManager* _inputManager;

        void applyGravity();
        bool _applyGravity;

    };

}