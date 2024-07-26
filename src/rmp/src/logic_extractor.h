// Copyright 2024 Google LLC
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#pragma once

#include <unordered_set>
#include <vector>

#include "abc_library_factory.h"
#include "db_sta/dbSta.hh"
#include "logic_cut.h"
#include "sta/Graph.hh"
#include "sta/NetworkClass.hh"
#include "sta/SearchPred.hh"

namespace rmp {

class SearchPredNonReg2AbcSupport : public sta::SearchPredNonReg2
{
 public:
  // supported_liberty_cells is a set of cells that are supported by ABC.
  SearchPredNonReg2AbcSupport(sta::dbSta* open_sta,
                              AbcLibrary* abc_library,
                              sta::Graph* graph)
      : sta::SearchPredNonReg2(open_sta),
        abc_library_(abc_library),
        graph_(graph)
  {
  }
  bool searchThru(sta::Edge* edge) override;

 private:
  AbcLibrary* abc_library_;
  sta::Graph* graph_;
};

class LogicExtractorFactory
{
 public:
  LogicExtractorFactory(sta::dbSta* open_sta) : open_sta_(open_sta) {}
  LogicExtractorFactory& AppendEndpoint(sta::Vertex* vertex);
  LogicCut BuildLogicCut(AbcLibrary& abc_network);

 private:
  // Process vertificies from BFS STA output to find the primary inputs.
  std::vector<sta::Pin*> GetPrimaryInputs(
      std::vector<sta::Vertex*>& cut_vertices);
  std::vector<sta::Pin*> GetPrimaryOutputs(
      std::vector<sta::Vertex*>& cut_vertices);
  std::vector<sta::Vertex*> GetCutVertices(AbcLibrary& abc_network);
  std::unordered_set<sta::Instance*> GetCutInstances(
      std::vector<sta::Vertex*>& cut_vertices);
  std::vector<sta::Pin*> FilterUndrivenOutputs(
      std::vector<sta::Pin*>& primary_outputs,
      std::unordered_set<sta::Instance*>& cut_instances);

  std::vector<sta::Vertex*> endpoints_;
  sta::dbSta* open_sta_;
};
}  // namespace rmp
