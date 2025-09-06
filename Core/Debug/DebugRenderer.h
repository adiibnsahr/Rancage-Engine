// Core/Debug/DebugRenderer.h
#pragma once
#include "Core/Math/Vector3.h"
#include <vector>

/**
 * @file DebugRenderer.h
 * @brief Declares the DebugRenderer class for drawing debug geometry like lines and AABBs.
 */

 /**
  * @class DebugRenderer
  * @brief Provides functionality to draw simple debug primitives such as lines and bounding boxes in 3D space.
  *        Useful for visualizing logic, physics, or other runtime information during development.
  */
class DebugRenderer {
public:
    /**
     * @brief Initializes internal resources required by the debug renderer.
     *        Should be called once during application startup or context setup.
     */
    void Initialize();

    /**
     * @brief Releases internal resources used by the debug renderer.
     *        Should be called once during application shutdown.
     */
    void Shutdown();

    /**
     * @brief Draws a colored line in 3D space from start to end.
     * @param start The start point of the line.
     * @param end The end point of the line.
     * @param color The RGB color of the line (each component in range [0,1]).
     */
    void DrawLine(const Vector3& start, const Vector3& end, const Vector3& color);

    /**
     * @brief Draws an axis-aligned bounding box (AABB) defined by its min and max corners.
     * @param min The minimum (corner) point of the AABB.
     * @param max The maximum (corner) point of the AABB.
     * @param color The RGB color of the bounding box lines.
     */
    void DrawAABB(const Vector3& min, const Vector3& max, const Vector3& color);

    /**
     * @brief Prepares the debug renderer for a new frame.
     *        Clears previously submitted debug primitives.
     */
    void BeginFrame();

    /**
     * @brief Finalizes and submits the debug primitives collected during the frame
     *        to the rendering pipeline.
     */
    void EndFrame();

private:
    /**
     * @struct LineVertex
     * @brief Represents a single vertex of a debug line, including position and color.
     */
    struct LineVertex {
        Vector3 pos;   ///< Position of the vertex in 3D space
        Vector3 color; ///< RGB color of the vertex
    };

    /**
     * @brief Container for collected line vertices to be rendered this frame.
     */
    std::vector<LineVertex> lines_;
};
