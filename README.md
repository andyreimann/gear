GEAR 2.5 - Game Engine
====
GEAR 2.5 is a Game Engine written in C++ and using OpenGL.
It extensively takes usage of new C++11 features and uses at least OpenGL 3.0 as the lowest version. Currently it is only compileable on Windows, but further OS may follow in future as well.

Current features:
 * generic extendable entity component system in a multithreaded environment
 * generic event callback system
 * mesh formats: fbx (animated), md5 (animated)
 * forward rendering
 * deferred rendering
 * highly configurable multipass rendering
 * shading languages: GLSL, CG
 * generic ubershader format with shader permutation generation using macros
 * configurable post processing pipeline
 * physics support (bullet physics or newton game dynamics)
 * cascaded shadow mapping for directional lights
 * point light shadow mapping
 * 2D Texturing support
 * Render to Texture (with multiple render targets)
 * Z-Sorted rendering for transparent materials
 * generic extendable resource importer with multithreaded caching possibility