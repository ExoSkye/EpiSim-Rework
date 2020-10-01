﻿// EpiSim.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include "render/sdlRender.h"
#include <chrono>
#include <thread>
#include <random>
#include "algononnaive.h"

#ifdef TRACY_ENABLE
void* operator new(std::size_t count) {
    auto ptr = malloc(count);TracyAlloc(ptr, count);
    return ptr;
}
void operator delete(void* ptr) noexcept {
    TracyFree(ptr);
    free(ptr);
}
#endif
// TODO: Reference additional headers your program requires here.
