//
// Created by weldeab solomon on 03/03/2023.
//

#ifndef INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_WFD_ALGORITHM_H
#define INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_WFD_ALGORITHM_H


#include "itemCentricAlgo.h"

class WFD_algorithm : public ItemCentricAlgo
{
public:
    WFD_algorithm(std::string algo_name, const Instance &instance);

protected:
    void sortBins();

};

#endif //INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_WFD_ALGORITHM_H
