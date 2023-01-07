#pragma once

extern "C" {
    _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001; // Enable NVIDIA Dedicated GPU
    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;   // Enable AMD Dedicated GPU
}