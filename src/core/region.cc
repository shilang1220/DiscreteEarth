// Copyright 2005 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS-IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// Author: ericv@google.com (Eric Veach)

#include "core/region.h"

#include <vector>

#include "core/gt_cap.h"
#include "core/gt_cell.h"
#include "core/gt_cell_id.h"

void Region::GetCellUnionBound(std::vector<GTCellId> *cell_ids) const {
  return GetCapBound().GetCellUnionBound(cell_ids);
}
