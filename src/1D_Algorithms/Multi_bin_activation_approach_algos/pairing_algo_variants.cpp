//
// Created by weldeab solomon on 06/03/2023.
//

#include "../../include/pairing_algo_variants.h"


#include <cmath>

PairingAlgorithm::PairingAlgorithm(std::string algo_name, const Instance &instance):
        BaseAlgorithm(algo_name, instance)
{}


void PairingAlgorithm::createNewBins(int m_bins)
{
    m_bins_activated.reserve(m_bins_activated.size() + m_bins);
    for (int i = 0; i < m_bins; ++i)
    {
        createNewBin();
    }
}



float PairingAlgorithm::computeItemBinScore(Item *item, Bin *bin)
{
    float score_value = 0.0;

    switch(m_score)
    {
        case SCORE::PRODUCT:
        {
            // score = item_size * residual bin capacity
            score_value = item->getItemSize() * bin->getRemBinCapacity();
        }
            break;

        case SCORE::SLACK:
        {
            // score = -(bin_residual_capacity - item_size)
            score_value = -(bin->getRemBinCapacity() - item->getItemSize());
        }
            break;
        case SCORE::TIGHT_FILL:
        {
            // score = item_size / residual bin capacity
            score_value = (item->getItemSize() / (float)bin->getRemBinCapacity());
        }
            break;
    }
    return score_value;
}

// updating scores for that bin only by going over the remaining items and computing scores for each item-bin pair
void PairingAlgorithm::updateScores(Bin* bin, std::vector<Item*>::iterator first_item, std::vector<Item*>::iterator end_it)
{

    std::vector<float>& item_scores = m_bin_item_scores[bin->getBinId()];
    for (auto item_it = first_item; item_it != end_it; item_it++)
    {
        //m_bin_item_scores[bin->getId()][(*item_it)->getId()];
        item_scores[(*item_it)->getItemId()] = computeItemBinScore(*item_it, bin);
    }
}


// we this method, we are setting up the envioroment and call the packitemstobins method
bool PairingAlgorithm::tryToSolve(int m_bins)
{
    if (m_bins < 0)
    {
        std::string s("Trying to solve instance with negative number of bins");
        throw std::runtime_error(s);
    }
    clearSolution(); // this makes m_bins_activated size 0
    createNewBins(m_bins);  // now m_bins_activated is size m_bins
    unsigned long nb_items = m_items.size();

    m_bin_item_scores.clear(); // cleaing the vector of vectors for item-bin scores
    m_bin_item_scores.resize(m_bins, std::vector<float>(nb_items, 0.0));
    // updating the item-bin scores for all bins
    for (Bin* bin : m_bins_activated)
    {
        updateScores(bin, m_items.begin(), m_items.end());
    }

    m_first_remaining_item = m_items.begin();
    return packItems(m_bins_activated.begin());
}


bool PairingAlgorithm::packItems(std::vector<Bin*>::iterator start_bin_it)
{
    auto first_item_it = m_first_remaining_item; // TODO probably don't need this new variable
    auto end_items_it = m_items.end();
    auto end_bins_it = m_bins_activated.end();
    int remaining_items = end_items_it - first_item_it;


    while(first_item_it != end_items_it) // While there are items to pack
    {
        // this is all to compute maximum score for each pair of item-bin
        auto max_score_item_it = end_items_it;
        auto max_score_bin_it = end_bins_it;
        float max_score_val = std::numeric_limits<double>::lowest(); // -infinity

        // For each remaining item
        for(auto curr_item_it = first_item_it; curr_item_it != end_items_it; ++curr_item_it)
        {
            // For each bin
            for(auto curr_bin_it = start_bin_it; curr_bin_it != end_bins_it; ++curr_bin_it)
            {
                // when retrieving the highest score this doesItemFitToBin method will indirectly invalidate computed scores of item-bin pairs where the item doesnt fit into the bin: we are only considering scores that  are of feasible item-bin pair.
                if ((*curr_bin_it)->doesItemFitToBin((*curr_item_it)->getItemSize()))
                {

                    float score;
//                    if (!store_scores)
//                    {
//                        // Need to recompute all scores
//                        score = computeItemBinScore((*curr_item_it), (*curr_bin_it));
//                    }
                    //else
                    // {
                    // Scores were computed previously
                    score = m_bin_item_scores[(*curr_bin_it)->getBinId()] [(*curr_item_it)->getItemId()];
                    //}

                    if (score > max_score_val)
                    {
                        max_score_val = score;
                        max_score_item_it = curr_item_it;
                        max_score_bin_it = curr_bin_it;
                    }
                }
            }
        }

        if (max_score_item_it != end_items_it)
        {
            // There is a feasible item-bin pair
            addItemToBin(*max_score_item_it, *max_score_bin_it);
            --remaining_items;

            // Put this item at beginning of the list and advance the first iterator
            std::iter_swap(max_score_item_it, first_item_it);
            first_item_it++;


            // Update score of remaining items for that bin
            updateScores(*max_score_bin_it, first_item_it, end_items_it);


        }
        else
        {
            // There is no feasible item-bin pair, the solution is infeasible
            // Update the iterator on first remaining item before exitting
            m_first_remaining_item = first_item_it;
            return false;
        }



    }

    m_first_remaining_item = end_items_it;
    // since the instance has been solved
    setNbOfBinsActuallyUsed(m_bins_activated.begin(), m_bins_activated.end());
    return true;

}

unsigned long PairingAlgorithm::solveInstance(int hint_nb_bins)
{
    std::string s = "With Pairing-type algorithm please call 'tryToSolve'  instead.";
    throw std::runtime_error(s);
}


