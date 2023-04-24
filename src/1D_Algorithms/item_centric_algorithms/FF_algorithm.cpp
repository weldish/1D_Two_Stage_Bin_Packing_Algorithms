//
// Created by weldeab solomon on 13/02/2023.
//

#include "../../include/FF_algorithm.h"

FF_algorithm::FF_algorithm(std::string algo_name, const Instance &instance):
        ItemCentricAlgo(algo_name, instance)

{
    m_is_FF_type = true;

}

