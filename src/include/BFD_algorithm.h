//
// Created by weldeab solomon on 03/03/2023.
//

#ifndef INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_BFD_ALGORITHM_H
#define INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_BFD_ALGORITHM_H


#include "itemCentricAlgo.h"
// BFD algorithm
class BFD_algorithm : public ItemCentricAlgo
{
public:
    BFD_algorithm(std::string algo_name, const Instance &instance);

protected:
    // sorting bins in increasing order of their residual capacity.
    void sortBins();

};


#endif //INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_BFD_ALGORITHM_H
