#pragma once

#include <raylib.h>

class DeltaTime {
private:
    float deltaTime = 0.0f;

public:
    void update() {
        deltaTime = GetFrameTime();

        if (deltaTime > 1.0f / 5.0f) {
            deltaTime = 1.0f / 5.0f;
        }
    }

    float getDeltaTime() const {
        return deltaTime;
    }

    int getFrameRate() const {
        return GetFPS();
    }
};