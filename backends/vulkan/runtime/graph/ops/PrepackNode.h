/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#ifdef USE_VULKAN_API

#include <ATen/native/vulkan/api/Context.h>
#include <ATen/native/vulkan/api/Tensor.h>
#include <ATen/native/vulkan/api/Types.h>

#include <executorch/backends/vulkan/runtime/graph/containers/Value.h>

namespace at {
namespace native {
namespace vulkan {

class ComputeGraph;

/*
 * Represents a single prepacking op in a ML model. In graph mode, ops will be
 * implemented in a derived class that implements encode, which will implement
 * encoding of shaders transferring necessary data (such as weights and biases)
 * to the GPU.
 */
class PrepackNode final {
  friend class ComputeGraph;

 public:
  PrepackNode(
      const api::ShaderInfo& shader,
      const api::utils::uvec3& global_workgroup_size,
      const api::utils::uvec3& local_workgroup_size,
      const ValueRef tref,
      const ValueRef packed,
      api::UniformParamsBuffer&& params)
      : shader_(shader),
        global_workgroup_size_(global_workgroup_size),
        local_workgroup_size_(local_workgroup_size),
        tref_(tref),
        packed_(packed),
        params_(std::move(params)) {}

  ~PrepackNode() = default;

  void encode(ComputeGraph* graph);

 protected:
  const api::ShaderInfo shader_;
  const api::utils::uvec3 global_workgroup_size_;
  const api::utils::uvec3 local_workgroup_size_;
  const ValueRef tref_;
  const ValueRef packed_;
  // TODO(T180906086): pass multiple buffers and index with ValueRef.
  // TODO(T180906457): allow re-computing param buffers.
  api::UniformParamsBuffer params_;
};

} // namespace vulkan
} // namespace native
} // namespace at

#endif /* USE_VULKAN_API */
