///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2021, The Regents of the University of California
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>
#include <unordered_map>

namespace utl {
class Logger;
}

namespace odb {
class dbBlock;
class dbInst;
}  // namespace odb

namespace mpl2 {
class Metrics;
class Cluster;

struct SizeThresholds
{
  int max_macro = 0;
  int min_macro = 0;
  int max_std_cell = 0;
  int min_std_cell = 0;
};

struct PhysicalHierarchyMaps
{
  std::map<int, Cluster*> id_to_cluster;
  std::unordered_map<odb::dbInst*, int> inst_to_cluster_id;
};

struct PhysicalHierarchy
{
  Cluster* root;
  PhysicalHierarchyMaps maps;

  SizeThresholds base_thresholds;
  float coarsening_ratio = 0;
  int max_level = 0;  // Max level might be reset when clustering.
};

class ClusteringEngine
{
 public:
  ClusteringEngine(odb::dbBlock* block, utl::Logger* logger);

  void setDesignMetrics(Metrics* design_metrics);
  void setTargetStructure(PhysicalHierarchy* tree);

  void buildPhysicalHierarchy();

 private:
  void initTree();
  void setDefaultThresholds();

  odb::dbBlock* block_;
  utl::Logger* logger_;
  Metrics* design_metrics_;

  PhysicalHierarchy* tree_;

  // Criado na engine
  int id_;
  SizeThresholds level_thresholds_;
};

}  // namespace mpl2