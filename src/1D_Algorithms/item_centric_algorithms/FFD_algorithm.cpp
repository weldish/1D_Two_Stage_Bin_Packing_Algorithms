//
// Created by weldeab solomon on 01/03/2023.
//

#include "../../include/FFD_algorithm.h"

FFD_algorithm::FFD_algorithm(std::string algo_name, const Instance &instance):
        ItemCentricAlgo(algo_name, instance)

{
    m_is_FFD_type = true;

}
