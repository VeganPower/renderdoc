/******************************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2017 Baldur Karlsson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#pragma once

namespace VKPipe
{
DOCUMENT("The contents of a single binding element within a descriptor set, possibly in an array.");
struct BindingElement
{
  DOCUMENT("The :class:`ResourceId` of the current view object, if one is in use.");
  ResourceId view;    // bufferview, imageview, attachmentview
  DOCUMENT("The :class:`ResourceId` of the current underlying buffer or image object.");
  ResourceId res;    // buffer, image, attachment
  DOCUMENT("The :class:`ResourceId` of the current sampler object.");
  ResourceId sampler;
  DOCUMENT("``True`` if this is an immutable sampler binding.");
  bool immutableSampler = false;

  DOCUMENT("The :class:`ResourceFormat` that the view uses.");
  ResourceFormat viewfmt;
  DOCUMENT("Four :class:`TextureSwizzle` elements indicating the swizzle applied to this texture.");
  TextureSwizzle swizzle[4] = {TextureSwizzle::Red, TextureSwizzle::Green, TextureSwizzle::Blue,
                               TextureSwizzle::Alpha};
  DOCUMENT("For textures - the first mip level used in the view.");
  uint32_t baseMip = 0;
  DOCUMENT("For 3D textures and texture arrays - the first slice used in the view.");
  uint32_t baseLayer = 0;
  DOCUMENT("For textures - the number of mip levels in the view.");
  uint32_t numMip = 0;
  DOCUMENT("For 3D textures and texture arrays - the number of array slices in the view.");
  uint32_t numLayer = 0;

  DOCUMENT("For buffers - the byte offset where the buffer view starts in the underlying buffer.");
  uint64_t offset = 0;
  DOCUMENT("For buffers - how many bytes are in this buffer view.");
  uint64_t size = 0;

  DOCUMENT("For samplers - the :class:`TextureFilter` describing the filtering mode.");
  TextureFilter Filter;
  DOCUMENT("For samplers - the :class:`AddressMode` in the U direction.");
  AddressMode AddressU = AddressMode::Wrap;
  DOCUMENT("For samplers - the :class:`AddressMode` in the V direction.");
  AddressMode AddressV = AddressMode::Wrap;
  DOCUMENT("For samplers - the :class:`AddressMode` in the W direction.");
  AddressMode AddressW = AddressMode::Wrap;
  DOCUMENT("For samplers - a bias to apply to the calculated mip level before sampling.");
  float mipBias = 0.0f;
  DOCUMENT("For samplers - the maximum anisotropic filtering level to use.");
  float maxAniso = 0.0f;
  DOCUMENT("For samplers - the :class:`CompareFunc` for comparison samplers.");
  CompareFunc comparison = CompareFunc::AlwaysTrue;
  DOCUMENT("For samplers - the minimum mip level that can be used.");
  float minlod = 0.0f;
  DOCUMENT("For samplers - the maximum mip level that can be used.");
  float maxlod = 0.0f;
  DOCUMENT("For samplers - the RGBA border color.");
  float BorderColor[4];
  DOCUMENT("For samplers - ``True`` if unnormalized co-ordinates are used in this sampler.");
  bool unnormalized = false;

  DOCUMENT(R"(For samplers - check if the border color is used in this Vulkan sampler.

:return: ``True`` if the border color is used, ``False`` otherwise.
:rtype: bool
)");
  bool UseBorder() const
  {
    return AddressU == AddressMode::ClampBorder || AddressV == AddressMode::ClampBorder ||
           AddressW == AddressMode::ClampBorder;
  }
};

DOCUMENT("The contents of a single binding within a descriptor set, either arrayed or not.");
struct DescriptorBinding
{
  DOCUMENT(R"(How many descriptors are in this binding array.
If this binding is empty/non-existant this value will be ``0``.
)");
  uint32_t descriptorCount = 0;
  DOCUMENT("The :class:`BindType` of this binding.");
  BindType type = BindType::Unknown;
  DOCUMENT("The :class:`ShaderStageMask` where this binding is visible.");
  ShaderStageMask stageFlags = ShaderStageMask::Unknown;

  DOCUMENT(R"(A list of :class:`VK_BindingElement` with the binding elements.
If :data:`descriptorCount` is 1 then this isn't an array, and this list has only one element.
)");
  rdcarray<BindingElement> binds;
};

DOCUMENT("The contents of a descriptor set.");
struct DescriptorSet
{
  DOCUMENT("The :class:`ResourceId` of the descriptor set layout that matches this set.");
  ResourceId layout;
  DOCUMENT("The :class:`ResourceId` of the descriptor set object.");
  ResourceId descset;

  DOCUMENT(R"(A list of :class:`VK_DescriptorBinding` with the bindings within this set.
This list is indexed by the binding, so it may be sparse (some entries do not contain any elements).
)");
  rdcarray<DescriptorBinding> bindings;
};

DOCUMENT("Describes the object and descriptor set bindings of a Vulkan pipeline object.");
struct Pipeline
{
  DOCUMENT("The :class:`ResourceId` of the pipeline object.");
  ResourceId obj;
  DOCUMENT("The flags used to create the pipeline object.");
  uint32_t flags = 0;

  DOCUMENT("A list of :class:`VK_DescriptorSet` with the bound descriptor sets.");
  rdcarray<DescriptorSet> DescSets;
};

DOCUMENT("Describes the Vulkan index buffer binding.")
struct IB
{
  DOCUMENT("The :class:`ResourceId` of the index buffer.");
  ResourceId buf;

  DOCUMENT("The byte offset from the start of the buffer to the beginning of the index data.");
  uint64_t offs = 0;
};

DOCUMENT("Describes the vulkan input assembly configuration.");
struct InputAssembly
{
  DOCUMENT("``True`` if primitive restart is enabled for strip primitives.");
  bool primitiveRestartEnable = false;

  DOCUMENT("The :class:`VK_IB` with the index buffer binding.");
  IB ibuffer;
};

DOCUMENT("Describes the configuration of a single vertex attribute.");
struct VertexAttribute
{
  DOCUMENT("The location in the shader that is bound to this attribute.");
  uint32_t location = 0;
  DOCUMENT("The vertex binding where data will be sourced from.");
  uint32_t binding = 0;
  DOCUMENT("The :class:`ResourceFormat` describing how each input element will be interpreted.");
  ResourceFormat format;
  DOCUMENT(
      "The byte offset from the start of each vertex data in the :data:`binding` to this "
      "attribute.");
  uint32_t byteoffset = 0;
};

DOCUMENT("Describes a vertex binding.");
struct VertexBinding
{
  DOCUMENT("The vertex binding where data will be sourced from.");
  uint32_t vbufferBinding = 0;
  DOCUMENT("The byte stride between the start of one set of vertex data and the next.");
  uint32_t bytestride = 0;
  DOCUMENT("``True`` if the vertex data is instance-rate.");
  bool perInstance = false;
};

DOCUMENT("Describes a single Vulkan vertex buffer binding.")
struct VB
{
  DOCUMENT("The :class:`ResourceId` of the buffer bound to this slot.");
  ResourceId buffer;
  DOCUMENT("The byte offset from the start of the buffer to the beginning of the vertex data.");
  uint64_t offset = 0;
};

DOCUMENT("Describes the fixed-function vertex input fetch setup.");
struct VertexInput
{
  DOCUMENT("A list of :class:`VK_VertexAttribute` with the vertex attributes.");
  rdcarray<VertexAttribute> attrs;
  DOCUMENT("A list of :class:`VK_VertexBinding` with the vertex bindings.");
  rdcarray<VertexBinding> binds;
  DOCUMENT("A list of :class:`VK_VB` with the vertex buffers.");
  rdcarray<VB> vbuffers;
};

DOCUMENT("The provided value for a specialization constant.");
struct SpecInfo
{
  DOCUMENT("The specialization ID");
  uint32_t specID = 0;
  DOCUMENT("A ``bytes`` with the contents of the constant.");
  bytebuf data;
};

DOCUMENT("Describes a Vulkan shader stage.");
struct Shader
{
  DOCUMENT("The :class:`ResourceId` of the shader module object.");
  ResourceId Object;
  DOCUMENT("The name of the entry point in the shader module that is used.");
  rdcstr entryPoint;

  DOCUMENT("A :class:`ShaderReflection` describing the reflection data for this shader.");
  ShaderReflection *ShaderDetails = NULL;
  DOCUMENT(R"(A :class:`ShaderBindpointMapping` to match :data:`ShaderDetails` with the bindpoint
mapping data.
)");
  ShaderBindpointMapping BindpointMapping;

  DOCUMENT("A :class:`VK_ShaderStage` identifying which stage this shader is bound to.");
  ShaderStage stage = ShaderStage::Vertex;

  DOCUMENT("A list of :class:`VK_SpecInfo` with the provided specialization constants.");
  rdcarray<SpecInfo> specialization;
};

DOCUMENT("Describes the state of the fixed-function tessellator.");
struct Tessellation
{
  DOCUMENT("The number of control points in each input patch.");
  uint32_t numControlPoints = 0;
};

DOCUMENT("Describes a single Vulkan viewport.");
struct Viewport
{
  DOCUMENT("The X co-ordinate of the viewport.");
  float x = 0.0f;
  DOCUMENT("The Y co-ordinate of the viewport.");
  float y = 0.0f;
  DOCUMENT("The width of the viewport.");
  float width = 0.0f;
  DOCUMENT("The height of the viewport.");
  float height = 0.0f;
  DOCUMENT("The minimum depth of the viewport.");
  float minDepth = 0.0f;
  DOCUMENT("The maximum depth of the viewport.");
  float maxDepth = 0.0f;
};

DOCUMENT("Describes a single Vulkan scissor region.");
struct Scissor
{
  DOCUMENT("The X co-ordinate of the scissor region.");
  int32_t x = 0;
  DOCUMENT("The Y co-ordinate of the scissor region.");
  int32_t y = 0;
  DOCUMENT("The width of the scissor region.");
  int32_t width = 0;
  DOCUMENT("The height of the scissor region.");
  int32_t height = 0;
};

DOCUMENT("Describes a combined viewport and scissor region.");
struct ViewportScissor
{
  DOCUMENT("The :class:`VK_Viewport`.");
  Viewport vp;
  DOCUMENT("The :class:`VK_Scissor`.");
  Scissor scissor;
};

DOCUMENT("Describes the view state in the pipeline.");
struct ViewState
{
  DOCUMENT("A list of :class:`VK_ViewportScissor`.");
  rdcarray<ViewportScissor> viewportScissors;
};

DOCUMENT("Describes the raster state in the pipeline.");
struct Raster
{
  DOCUMENT(R"(``True`` if pixels outside of the near and far depth planes should be clamped and
to ``0.0`` to ``1.0`` and not clipped.
)");
  bool depthClampEnable = false;
  DOCUMENT("``True`` if primitives should be discarded during rasterization.");
  bool rasterizerDiscardEnable = false;
  DOCUMENT(R"(``True`` if counter-clockwise polygons are front-facing.
``False`` if clockwise polygons are front-facing.
)");
  bool FrontCCW = false;
  DOCUMENT("The polygon fill mode.");
  FillMode fillMode = FillMode::Solid;
  DOCUMENT("The polygon culling mode.");
  CullMode cullMode = CullMode::NoCull;

  DOCUMENT("The fixed depth bias value to apply to z-values.");
  float depthBias = 0.0f;
  DOCUMENT(R"(The clamp value for calculated depth bias from :data:`depthBias` and
:data:`slopeScaledDepthBias`
)");
  float depthBiasClamp = 0.0f;
  DOCUMENT("The slope-scaled depth bias value to apply to z-values.");
  float slopeScaledDepthBias = 0.0f;
  DOCUMENT("The fixed line width in pixels.");
  float lineWidth = 0.0f;
};

DOCUMENT("Describes the multisampling state in the pipeline.");
struct MultiSample
{
  DOCUMENT("How many samples to use when rasterizing.");
  uint32_t rasterSamples = 0;
  DOCUMENT("``True`` if rendering should happen at sample-rate frequency.");
  bool sampleShadingEnable = false;
  DOCUMENT("The minimum sample shading rate.");
  float minSampleShading = 0.0f;
  DOCUMENT("A mask that generated samples should be masked with using bitwise ``AND``.");
  uint32_t sampleMask = 0;
};

DOCUMENT("Describes the details of a Vulkan blend operation.");
struct BlendEquation
{
  DOCUMENT("The :class:`BlendMultiplier` for the source blend value.");
  BlendMultiplier Source = BlendMultiplier::One;
  DOCUMENT("The :class:`BlendMultiplier` for the destination blend value.");
  BlendMultiplier Destination = BlendMultiplier::One;
  DOCUMENT("The :class:`BlendOp` to use in the blend calculation.");
  BlendOp Operation = BlendOp::Add;
};

DOCUMENT("Describes the blend configuration for a given Vulkan attachment.");
struct Blend
{
  DOCUMENT("``True`` if blending is enabled for this attachment.");
  bool blendEnable = false;

  DOCUMENT("A :class:`VK_BlendEquation` describing the blending for colour values.");
  BlendEquation blend;
  DOCUMENT("A :class:`VK_BlendEquation` describing the blending for alpha values.");
  BlendEquation alphaBlend;

  DOCUMENT("The mask for writes to the attachment.");
  uint8_t writeMask = 0;
};

DOCUMENT("Describes the pipeline blending state.");
struct ColorBlend
{
  DOCUMENT("``True`` if alpha-to-coverage should be used when blending to an MSAA target.");
  bool alphaToCoverageEnable = false;
  DOCUMENT("``True`` if alpha-to-one should be used when blending to an MSAA target.");
  bool alphaToOneEnable = false;
  DOCUMENT("``True`` if the logic operation in :data:`logic` should be used.");
  bool logicOpEnable = false;
  DOCUMENT(
      "The :class:`LogicOp` to use for logic operations, if :data:`logicOpEnable` is ``True``.");
  LogicOp logic = LogicOp::NoOp;

  DOCUMENT("The list of :class:`VK_Blend` with the blending configuration per-attachment.");
  rdcarray<Blend> attachments;

  DOCUMENT("The constant blend factor to use in blend equations.");
  float blendConst[4] = {1.0f, 1.0f, 1.0f, 1.0f};
};

DOCUMENT("Describes the details of a Vulkan stencil operation.");
struct StencilFace
{
  DOCUMENT("The :class:`StencilOp` to apply if the stencil-test fails.");
  StencilOp FailOp = StencilOp::Keep;
  DOCUMENT("The :class:`StencilOp` to apply if the depth-test fails.");
  StencilOp DepthFailOp = StencilOp::Keep;
  DOCUMENT("The :class:`StencilOp` to apply if the stencil-test passes.");
  StencilOp PassOp = StencilOp::Keep;
  DOCUMENT("The :class:`CompareFunc` to use for testing stencil values.");
  CompareFunc Func = CompareFunc::AlwaysTrue;
  DOCUMENT("The current stencil reference value.");
  uint32_t ref = 0;
  DOCUMENT("The mask for testing stencil values.");
  uint32_t compareMask = 0;
  DOCUMENT("The mask for writing stencil values.");
  uint32_t writeMask = 0;
};

DOCUMENT("Describes the pipeline depth-stencil state.");
struct DepthStencil
{
  DOCUMENT("``True`` if depth testing should be performed.");
  bool depthTestEnable = false;
  DOCUMENT("``True`` if depth values should be written to the depth target.");
  bool depthWriteEnable = false;
  DOCUMENT("``True`` if depth bounds tests should be applied.");
  bool depthBoundsEnable = false;
  DOCUMENT("The :class:`CompareFunc` to use for testing depth values.");
  CompareFunc depthCompareOp = CompareFunc::AlwaysTrue;

  DOCUMENT("``True`` if stencil operations should be performed.");
  bool stencilTestEnable = false;

  DOCUMENT("A :class:`VK_StencilFace` describing what happens for front-facing polygons.");
  StencilFace front;
  DOCUMENT("A :class:`VK_StencilFace` describing what happens for back-facing polygons.");
  StencilFace back;

  DOCUMENT("The near plane bounding value.");
  float minDepthBounds = 0.0f;
  DOCUMENT("The far plane bounding value.");
  float maxDepthBounds = 0.0f;
};

DOCUMENT("Describes the setup of a renderpass and subpasses.");
struct RenderPass
{
  DOCUMENT("The :class:`ResourceId` of the render pass.");
  ResourceId obj;

  // VKTODOMED renderpass and subpass information here

  DOCUMENT("A list of indices into the framebuffer attachments for input attachments.");
  rdcarray<uint32_t> inputAttachments;
  DOCUMENT("A list of indices into the framebuffer attachments for color attachments.");
  rdcarray<uint32_t> colorAttachments;
  DOCUMENT("A list of indices into the framebuffer attachments for resolve attachments.");
  rdcarray<uint32_t> resolveAttachments;
  DOCUMENT(R"(An index into the framebuffer attachments for the depth-stencil attachment.

If there is no depth-stencil attachment, this index is ``-1``.
)");
  int32_t depthstencilAttachment = -1;
};

DOCUMENT("Describes a single attachment in a framebuffer object.");
struct Attachment
{
  DOCUMENT("The :class:`ResourceId` of the image view itself.");
  ResourceId view;
  DOCUMENT("The :class:`ResourceId` of the underlying image that the view refers to.");
  ResourceId img;

  DOCUMENT("The :class:`ResourceFormat` that the view uses.");
  ResourceFormat viewfmt;
  DOCUMENT("Four :class:`TextureSwizzle` elements indicating the swizzle applied to this texture.");
  TextureSwizzle swizzle[4] = {TextureSwizzle::Red, TextureSwizzle::Green, TextureSwizzle::Blue,
                               TextureSwizzle::Alpha};
  DOCUMENT("The first mip level used in the attachment.");
  uint32_t baseMip = 0;
  DOCUMENT("For 3D textures and texture arrays, the first slice used in the attachment.");
  uint32_t baseLayer = 0;
  DOCUMENT("The number of mip levels in the attachment.");
  uint32_t numMip = 1;
  DOCUMENT("For 3D textures and texture arrays, the number of array slices in the attachment.");
  uint32_t numLayer = 1;
};

DOCUMENT("Describes a framebuffer object and its attachments.");
struct Framebuffer
{
  DOCUMENT("The :class:`ResourceId` of the framebuffer object.");
  ResourceId obj;

  DOCUMENT("A list of :class:`VK_Attachment` with the attachments of this framebuffer.");
  rdcarray<Attachment> attachments;

  DOCUMENT("The width of this framebuffer in pixels.");
  uint32_t width = 0;
  DOCUMENT("The height of this framebuffer in pixels.");
  uint32_t height = 0;
  DOCUMENT("The number of layers in this framebuffer.");
  uint32_t layers = 0;
};

DOCUMENT("Describes the render area for a render pass instance.");
struct RenderArea
{
  DOCUMENT("The X co-ordinate of the render area.");
  int32_t x = 0;
  DOCUMENT("The Y co-ordinate of the render area.");
  int32_t y = 0;
  DOCUMENT("The width of the render area.");
  int32_t width = 0;
  DOCUMENT("The height of the render area.");
  int32_t height = 0;
};

DOCUMENT("Describes the current pass instance at the current time.");
struct CurrentPass
{
  DOCUMENT("The :class:`VK_RenderPass` that is currently active.");
  RenderPass renderpass;
  DOCUMENT("The :class:`VK_Framebuffer` that is currently being used.");
  Framebuffer framebuffer;
  DOCUMENT("The :class:`VK_RenderArea` that is currently being rendered to.");
  RenderArea renderArea;
};

DOCUMENT("Contains the layout of a range of subresources in an image.");
struct ImageLayout
{
  DOCUMENT("The first mip level used in the range.");
  uint32_t baseMip = 0;
  DOCUMENT("For 3D textures and texture arrays, the first slice used in the range.");
  uint32_t baseLayer = 0;
  DOCUMENT("The number of mip levels in the range.");
  uint32_t numMip = 1;
  DOCUMENT("For 3D textures and texture arrays, the number of array slices in the range.");
  uint32_t numLayer = 1;
  DOCUMENT("The name of the current image state.");
  rdcstr name;
};

DOCUMENT("Contains the current layout of all subresources in the image.");
struct ImageData
{
  DOCUMENT("The :class:`ResourceId` of the image.");
  ResourceId image;

  DOCUMENT(
      "A list of :class:`VK_ImageLayout` with the set of subresources that make up the image.");
  rdcarray<ImageLayout> layouts;
};

DOCUMENT("The full current Vulkan pipeline state.");
struct State
{
  DOCUMENT("A :class:`VK_Pipeline` with the currently bound compute pipeline, if any.");
  Pipeline compute;
  DOCUMENT("A :class:`VK_Pipeline` with the currently bound graphics pipeline, if any.");
  Pipeline graphics;

  DOCUMENT("A :class:`VK_InputAssembly` describing the input assembly stage.");
  InputAssembly IA;
  DOCUMENT("A :class:`VK_VertexInput` describing the vertex input stage.");
  VertexInput VI;

  DOCUMENT("A :class:`VK_Shader` describing the vertex shader stage.");
  Shader m_VS;
  DOCUMENT("A :class:`VK_Shader` describing the tessellation control shader stage.");
  Shader m_TCS;
  DOCUMENT("A :class:`VK_Shader` describing the tessellation evaluation shader stage.");
  Shader m_TES;
  DOCUMENT("A :class:`VK_Shader` describing the geometry shader stage.");
  Shader m_GS;
  DOCUMENT("A :class:`VK_Shader` describing the fragment shader stage.");
  Shader m_FS;
  DOCUMENT("A :class:`VK_Shader` describing the compute shader stage.");
  Shader m_CS;

  DOCUMENT("A :class:`VK_Tessellation` describing the tessellation stage.");
  Tessellation Tess;

  DOCUMENT("A :class:`VK_ViewState` describing the viewport setup.");
  ViewState VP;
  DOCUMENT("A :class:`VK_Raster` describing rasterization.");
  Raster RS;

  DOCUMENT("A :class:`VK_MultiSample` describing the multisample setup.");
  MultiSample MSAA;
  DOCUMENT("A :class:`VK_ColorBlend` describing color blending.");
  ColorBlend CB;
  DOCUMENT("A :class:`VK_DepthStencil` describing the depth-stencil stage.");
  DepthStencil DS;

  DOCUMENT("A :class:`VK_CurrentPass` describing the current renderpass, subpass and framebuffer.");
  CurrentPass Pass;

  DOCUMENT("A list of :class:`VK_ImageData` entries, one for each image.");
  rdcarray<ImageData> images;
};

};    // namespace VKPipe

DECLARE_REFLECTION_STRUCT(VKPipe::BindingElement);
DECLARE_REFLECTION_STRUCT(VKPipe::DescriptorBinding);
DECLARE_REFLECTION_STRUCT(VKPipe::DescriptorSet);
DECLARE_REFLECTION_STRUCT(VKPipe::Pipeline);
DECLARE_REFLECTION_STRUCT(VKPipe::IB);
DECLARE_REFLECTION_STRUCT(VKPipe::InputAssembly);
DECLARE_REFLECTION_STRUCT(VKPipe::VertexAttribute);
DECLARE_REFLECTION_STRUCT(VKPipe::VertexBinding);
DECLARE_REFLECTION_STRUCT(VKPipe::VB);
DECLARE_REFLECTION_STRUCT(VKPipe::VertexInput);
DECLARE_REFLECTION_STRUCT(VKPipe::SpecInfo);
DECLARE_REFLECTION_STRUCT(VKPipe::Shader);
DECLARE_REFLECTION_STRUCT(VKPipe::Tessellation);
DECLARE_REFLECTION_STRUCT(VKPipe::Viewport);
DECLARE_REFLECTION_STRUCT(VKPipe::Scissor);
DECLARE_REFLECTION_STRUCT(VKPipe::ViewportScissor);
DECLARE_REFLECTION_STRUCT(VKPipe::ViewState);
DECLARE_REFLECTION_STRUCT(VKPipe::Raster);
DECLARE_REFLECTION_STRUCT(VKPipe::MultiSample);
DECLARE_REFLECTION_STRUCT(VKPipe::BlendEquation);
DECLARE_REFLECTION_STRUCT(VKPipe::Blend);
DECLARE_REFLECTION_STRUCT(VKPipe::ColorBlend);
DECLARE_REFLECTION_STRUCT(VKPipe::StencilFace);
DECLARE_REFLECTION_STRUCT(VKPipe::DepthStencil);
DECLARE_REFLECTION_STRUCT(VKPipe::RenderPass);
DECLARE_REFLECTION_STRUCT(VKPipe::Attachment);
DECLARE_REFLECTION_STRUCT(VKPipe::Framebuffer);
DECLARE_REFLECTION_STRUCT(VKPipe::RenderArea);
DECLARE_REFLECTION_STRUCT(VKPipe::CurrentPass);
DECLARE_REFLECTION_STRUCT(VKPipe::ImageLayout);
DECLARE_REFLECTION_STRUCT(VKPipe::ImageData);
DECLARE_REFLECTION_STRUCT(VKPipe::State);
