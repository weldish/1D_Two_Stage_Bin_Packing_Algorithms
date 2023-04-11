//
// Created by weldeab solomon on 11/04/2023.
//

#ifndef INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_UTILS_H
#define INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_UTILS_H


#include "base_algorithm.h"

#include "itemCentricAlgo.h"
//#include "FF_algorithm.hpp"
#include "FFD_algorithm.h"
#include "BFD_algorithm.h"
#include "WFD_algorithm.h"
//#include "WFDm_algorithm.hpp"
#include "pairing_algo_variants.h"
#include "lower_bound.h"
#include "Item_Centric_To_Pairing.h"
#include "Pairing_To_Item_Centric.h"
#include "Pairing_To_Pairing.h"

//#include "item_centric_algorithms.hpp"
//#include "algos_MultiBin.hpp"
//#include "lower_bounds.hpp"

#include <string>

using namespace onedpacking;

//std::vector<std::string> splitString(const std::string& str);
//bool isRatioWeight(const std::string& str);

// only for item-centric algoritms such as FFD, BFD, WFD
BaseAlgorithm* createItemCentricAlgo(std::string algo_name, const Instance &instance);
// only for SINGLE stage mutlibin algoritms such as WFDm and pairing
BaseAlgorithm* createSingleMultiBinAlgo(std::string algo_name, const Instance &instance);

// only for two stage mutlibin algoritms
BaseAlgorithm* createTwoStageMultiBinAlgo(std::string algo_name, const Instance &instance);



#endif //INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_UTILS_H
