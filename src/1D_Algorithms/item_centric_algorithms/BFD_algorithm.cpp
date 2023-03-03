//
// Created by weldeab solomon on 03/03/2023.
//

#include "../../include/BFD_algorithm.h"



BFD_algorithm::BFD_algorithm(std::string algo_name, const Instance &instance):
        ItemCentricAlgo(algo_name, instance)

{
    m_is_BFD_type = true;

}

void BFD_algorithm::sortBins()
{
    stable_sort(m_bins_activated.begin(), m_bins_activated.end(), bin_Comparator_Rem_Cap_Increasing);
}
