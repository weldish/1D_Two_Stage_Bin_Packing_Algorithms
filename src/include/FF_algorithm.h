//
// Created by weldeab solomon on 13/02/2023.
//

#ifndef INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_FF_ALGORITHM_H
#define INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_FF_ALGORITHM_H


#include "ItemCentricAlgo.h"
// we use this algorithm when solving the upper bound
class FF_algorithm : public ItemCentricAlgo
{
public:
    FF_algorithm(std::string algo_name, const Instance &instance);

};

#endif //INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_FF_ALGORITHM_H
