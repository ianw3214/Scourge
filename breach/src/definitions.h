#pragma once

// ======================================
// TODO: Maybe these layers should live in the engine?
//  - idk, need to think about it more
//  - perhaps instead of all layers sharing a z buffer, this should utilize
//       framebuffers instead that then get combined when rendering the
//       final screen
enum class RenderLayer : int {
    // Hacky solution
    BACKGROUND = -5,
    DEFAULT = 0,
    VFX_TOP = 1,
    UI = 5
};