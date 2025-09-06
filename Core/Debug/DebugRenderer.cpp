// Core/Debug/DebugRenderer.cpp
#include "DebugRenderer.h"

void DebugRenderer::Initialize() {
    // Initialize GPU buffers, shaders if needed (implementation depends on GraphicsAPI)
    lines_.reserve(1024); // reserve for performance
}

void DebugRenderer::Shutdown() {
    // Release GPU resources
    lines_.clear();
}

void DebugRenderer::DrawLine(const Vector3& start, const Vector3& end, const Vector3& color) {
    lines_.push_back({ start, color });
    lines_.push_back({ end, color });
}

void DebugRenderer::DrawAABB(const Vector3& min, const Vector3& max, const Vector3& color) {
    // 8 corners of the bounding box
    Vector3 corners[8] = {
        {min.x, min.y, min.z},
        {max.x, min.y, min.z},
        {max.x, max.y, min.z},
        {min.x, max.y, min.z},
        {min.x, min.y, max.z},
        {max.x, min.y, max.z},
        {max.x, max.y, max.z},
        {min.x, max.y, max.z}
    };

    // 12 edges of a box defined by pairs of indices into corners array
    int edges[12][2] = {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };

    for (int i = 0; i < 12; ++i) {
        DrawLine(corners[edges[i][0]], corners[edges[i][1]], color);
    }
}

void DebugRenderer::BeginFrame() {
    lines_.clear();
}

void DebugRenderer::EndFrame() {
    // Submit lines_ to GPU pipeline for drawing
    // This depends on actual Graphics API (e.g., DirectX12)
    // Conceptually:
    // 1. Map vertex buffer, upload lines_
    // 2. Bind debug pipeline (simple color shader)
    // 3. Issue Draw call with lines_.size() vertices
}
