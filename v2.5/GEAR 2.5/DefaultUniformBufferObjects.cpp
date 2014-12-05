#pragma once

#include "DefaultUniformBufferObjects.h"
#include <G2Core/ShaderViews.h>

using namespace G2;

// TODO we HAVE to get rid of the ShaderLanguage parameter at some point :)

DefaultUniformBufferObjects::DefaultUniformBufferObjects() :
material("GLSL", sizeof(G2Core::ShaderView::Material), nullptr, G2Core::BufferUsage::STREAM_DRAW, G2Core::UniformBufferBindingPoint::UBO_MATERIAL),
lights("GLSL", sizeof(G2Core::ShaderView::Light) * 8, nullptr, G2Core::BufferUsage::DYNAMIC_DRAW, G2Core::UniformBufferBindingPoint::UBO_LIGHTS),
matrices("GLSL", sizeof(G2Core::ShaderView::Matrices), nullptr, G2Core::BufferUsage::STREAM_DRAW, G2Core::UniformBufferBindingPoint::UBO_MATRICES),
postProcessingInfo("GLSL", sizeof(G2Core::ShaderView::PostProcessingInfo), nullptr, G2Core::BufferUsage::DYNAMIC_DRAW, G2Core::UniformBufferBindingPoint::UBO_POST_PROCESSING_INFO)
{}