#pragma once

namespace DX
{
    class Listener
    {
    public:
        static void Initialize();

        static void SetVolume(float volume);

        static void SetPosition(float x, float y, float z);
        static void SetPosition(float position[3]);

        static void SetVelocity(float x, float y, float z);
        static void SetVelocity(float velocity[3]);

        /// <summary>
        /// Set the listener orientation
        /// </summary>
        /// <param name="lX">look at X</param>
        /// <param name="lY">look at Y</param>
        /// <param name="lZ">look at Z</param>
        /// <param name="uX">up X</param>
        /// <param name="uY">up Y</param>
        /// <param name="uZ">up Z</param>
        static void SetOrientation(float lX, float lY, float lZ, float uX, float uY, float uZ);
        static void SetOrientation(float orientation[6]);
    };
}