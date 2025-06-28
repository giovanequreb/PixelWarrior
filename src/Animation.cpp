#include "Animation.h"

Animation::Animation() 
    : currentFrame(0), frameDelay(0), frameTimer(0), shouldLoop(true), finished(false) {}

void Animation::setFrames(const std::vector<SDL_Texture*>& newFrames, int delay, bool loop) {
    frames = newFrames;
    frameDelay = delay;
    shouldLoop = loop;
    reset();
}

void Animation::update() {
    if (frames.empty() || finished) {
        return;
    }

    frameTimer++;
    if (frameTimer >= frameDelay) {
        frameTimer = 0;
        currentFrame++;
        // La comparazione ora è tra tipi uguali (size_t), il warning è risolto
        if (currentFrame >= frames.size()) {
            if (shouldLoop) {
                currentFrame = 0;
            } else {
                currentFrame = frames.size() - 1; // Rimani sull'ultimo frame
                finished = true;
            }
        }
    }
}

void Animation::reset() {
    currentFrame = 0;
    frameTimer = 0;
    finished = false;
}

SDL_Texture* Animation::getCurrentFrameTexture() const {
    if (!frames.empty()) {
        return frames[currentFrame];
    }
    return nullptr;
}

bool Animation::isFinished() const {
    return finished;
}

bool Animation::isLooping() const {
    return shouldLoop;
}

// ---> IMPLEMENTAZIONE DELLA NUOVA FUNZIONE <---
size_t Animation::getCurrentFrameIndex() const {
    return currentFrame;
}
