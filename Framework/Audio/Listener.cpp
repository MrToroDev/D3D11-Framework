#include "Listener.h"
#include <AL/al.h>

namespace DX
{
    void Listener::Initialize()
    {
        alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
        alListenerf(AL_GAIN, 0.5f);
    }
    void Listener::SetVolume(float volume)
    {
        alListenerf(AL_GAIN, volume);
    }
    void Listener::SetPosition(float x, float y, float z)
    {
        alListener3f(AL_POSITION, x, y, z);
    }
    
    void Listener::SetPosition(float position[3])
    {
        alListener3f(AL_POSITION, position[0], position[1], position[2]);
    }
    
    void Listener::SetVelocity(float x, float y, float z)
    {
        alListener3f(AL_VELOCITY, x, y , z);
    }
    
    void Listener::SetVelocity(float velocity[3])
    {
        alListener3f(AL_VELOCITY, velocity[0], velocity[1], velocity[2]);
    }
    void Listener::SetOrientation(float lX, float lY, float lZ, float uX, float uY, float uZ)
    {
        float orientation[] = { lX, lY, lZ, uX, uY, uZ };
        alListenerfv(AL_ORIENTATION, orientation);
    }
    void Listener::SetOrientation(float orientation[6])
    {
        alListenerfv(AL_ORIENTATION, orientation);
    }
}