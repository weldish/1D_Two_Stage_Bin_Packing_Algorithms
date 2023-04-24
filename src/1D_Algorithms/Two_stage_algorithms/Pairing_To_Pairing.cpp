//
// Created by weldeab solomon on 12/03/2023.
//

#include "../../include/Pairing_To_Pairing.h"


#include <cmath>
#include <iostream>

PairingToPairing::PairingToPairing(std::string algo_name, const Instance &instance):
        BaseAlgorithm(algo_name, instance)

{}

int PairingToPairing::solveInstWith2SA(float b_factor, int LB, int UB)
{
    int best_solution = UB; // keeping track the best solution

    int breakpoint = 0; // first breakpoint
    int k; // number of iterations
    int answer;
    bool found_sol = false;

    k = ceil(1/b_factor); // this specifies the number of breakpoints or iterations for a given b_factor
    int track = -1 ;
    for (int i= 0; i<k; i++)
    {
        breakpoint = breakpoint + b_factor * LB;

        answer = solveInstWithMBP(breakpoint, LB, UB);

        if (answer == -1)
        {
            std::cout << "coudnt find answer" << std::endl;
        }
        if(answer >=0)
        {
            found_sol = true;
        }

        if (answer > 0 and answer < best_solution)
        {
            //update best solution
            best_solution = answer;
            track = i;
        }
        if (best_solution == LB)
        {
            // if the current best solution is the same as LB, return the current best solution since it is optimal.
            return best_solution;
        }
    }

    if(!found_sol)
    {
        return -1;
    }

    std::cout << "best_k_value: " << std::to_string(track) << std::endl;
    return best_solution;

}

int PairingToPairing::solveInstWithMBP(int breakpoint, int LB, int UB)
{

    // First, try to find a solution with UB
    if (!trySolveTwoStage(breakpoint, UB))
    {
        // If no solution is found, there is no need to continue the search
        return -1;
    }

    // A solution with UB was found, store the current solution
    m_best_solution_bins.clear();
    updateBestSolutionBins(getActivatedBinsCopy());
    int m_bins;

    // Then iteratively try to improve on the solution
    while (LB < UB)
    {
        m_bins = std::floor((float)(LB + UB) / 2.0);

        if (trySolveTwoStage(breakpoint, m_bins))
        {
            // Better solution found, update UB
            UB = m_bins;
            updateBestSolutionBins(getActivatedBinsCopy());
        }
        else
        {
            // m_bins is too small, update LB
            LB = m_bins + 1; // +1 to keep a potential feasible solution with LB
        }
    }

    setSolution(m_best_solution_bins);
    return UB;


}



void PairingToPairing::updateBestSolutionBins(const std::vector<Bin*> &activated_bins)
{
    for (Bin* bin : m_best_solution_bins)
    {

        if (bin != nullptr)
        {
            delete bin;
        }
    }
    m_best_solution_bins.clear();
    m_best_solution_bins = activated_bins;
}




bool PairingToPairing::trySolveTwoStage(int breakpoint, int m_bins)
{

    clearSolution(); // deletes all bins that were used from a previous attempt to solve the instance

    // For all items in the list
    auto curr_item_it = m_items.begin();
    auto end_items_it = m_items.end();

    return solve_First_Pairing(breakpoint, m_bins, curr_item_it, end_items_it);
}



// solve by calling the itemCentric algorithm
bool PairingToPairing::solve_First_Pairing(int breakpoint, int m_bins, std::vector<Item*>::iterator start_it, std::vector<Item*>::iterator end_it)
{

    if (breakpoint < 0)
    {
        std::string s("Trying to solve instance with negative number of bins");
        throw std::runtime_error(s);
    }


    // creat a breakpoint number of bins
    for (int i = 0; i < breakpoint; ++i)
    {
        createNewBin();
    }

    int nb_items = m_items.size();

    m_bin_item_scores.clear();
    //unsigned long nb_items = m_items.size();
    m_bin_item_scores.resize(breakpoint, std::vector<float>(nb_items, 0.0));


    for (Bin* bin : m_bins_activated)
    {
        updateScores(bin, m_items.begin(), m_items.end(), m_score_first);
    }


    auto first_item_it = start_it;
    auto end_items_it = end_it;
    auto start_bin_it = m_bins_activated.begin();
    auto end_bins_it = m_bins_activated.end();


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

                    // Scores were computed previously
                    float score = m_bin_item_scores[(*curr_bin_it)->getBinId()] [(*curr_item_it)->getItemId()];


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
            //--remaining_items;

            // Put this item at beginning of the list and advance the first iterator
            std::iter_swap(max_score_item_it, first_item_it);
            first_item_it++;


            // Update score of remaining items for that bin
            updateScores(*max_score_bin_it, first_item_it, end_items_it, m_score_first);


        }
        else
        {
            // There is no feasible item-bin pair, the solution is infeasible
            // Update the iterator on first remaining item before exitting
            auto first_remaining_item = first_item_it;

            // this method pack the rest of items by adding one bin at a time.
            return solve_Second_Pairing(breakpoint, m_bins, first_remaining_item, end_it, m_score_second);


        }

    }


    return true;


}



// solve by calling the pairing algorithm
bool PairingToPairing::solve_Second_Pairing(int breakpoint, int m_bins, std::vector<Item*>::iterator last_it, std::vector<Item*>::iterator end_it, SCORE score)
{
    m_bin_item_scores.clear();

    int nb_items = m_items.size();
    //unsigned long nb_items = m_items.size();
    m_bin_item_scores.resize(m_bins, std::vector<float>(nb_items, 0.0));

    // m_bins always bigger than breakpoint by atleast one
    //create m_bins - breakpoint
    //std::cout << "second pairing was run" << std::endl;
    int extra_bins = m_bins-breakpoint;
    if (extra_bins < 0)
    {

        std::string s("trying to solve the isntance without adding new bins");
        throw std::runtime_error(s);
    }
    // add bins to the existing ones
    for (int i = 0; i < extra_bins; ++i)
    {
        createNewBin();
    }

    for (Bin* bin : m_bins_activated)
    {
        updateScores(bin, m_items.begin(), m_items.end(), m_score_second);
    }


    // settting up the algorithm finsihes here


    auto first_item_it = last_it; // first item is the last unallocated item from stage 1

    auto end_items_it = end_it;
    // we only need to scan the additional bins since the former ones are packed/cant take in more items.(optimisation)
    auto start_bin_it = m_bins_activated.begin();
    if (extra_bins >= 0)
    {
        start_bin_it = m_bins_activated.begin() + m_bins_activated.size() - extra_bins - 2;
    }

    auto end_bins_it = m_bins_activated.end();



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
                    // Scores were computed previously
                    float score = m_bin_item_scores[(*curr_bin_it)->getBinId()] [(*curr_item_it)->getItemId()];
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
            //--remaining_items;

            // Put this item at beginning of the list and advance the first iterator
            std::iter_swap(max_score_item_it, first_item_it);
            first_item_it++;


            // Update score of remaining items for that bin
            updateScores(*max_score_bin_it, first_item_it, end_items_it, m_score_second);


        }
        else
        {
            // There is no feasible item-bin pair, the solution is infeasible
            // Update the iterator on first remaining item before exiting

            return false;
        }

    }

    return true;
}



// updating scores for that bin only by going over the remaining items and computing scores for each item-bin pair

void PairingToPairing::updateScores(Bin* bin, std::vector<Item*>::iterator first_item, std::vector<Item*>::iterator end_it, SCORE score)
{
    std::vector<float>& item_scores = m_bin_item_scores[bin->getBinId()];
    for (auto item_it = first_item; item_it != end_it; item_it++)
    {
        //m_bin_item_scores[bin->getId()][(*item_it)->getId()];
        item_scores[(*item_it)->getItemId()] = computeItemBinScore(*item_it, bin, score);
    }
}



float PairingToPairing::computeItemBinScore(Item *item, Bin *bin, SCORE m_score)
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




unsigned long PairingToPairing::solveInstWithIC(int hint_nb_bins)
{
    std::string s = "With itemCentricpairing-type algorithm please call solveForMultiBreakPointsinstead.";
    throw std::runtime_error(s);
}

bool PairingToPairing::tryToSolve(int m_bins)
{
    std::string s = "With itemCentricpairing-type algorithm call solveForMultiBreakPoints instead";
    throw std::runtime_error(s);
}



/* ================================================ */
/* ================================================ */




Pairing_P_To_P::Pairing_P_To_P(std::string algo_name, const Instance &instance):
        PairingToPairing(algo_name, instance)
{
    m_score_first = SCORE::PRODUCT;
    m_score_second = SCORE::PRODUCT;
}

/* ================================================ */
/* ================================================ */
Pairing_P_To_S::Pairing_P_To_S(std::string algo_name, const Instance &instance):
        PairingToPairing(algo_name, instance)
{
    m_score_first = SCORE::PRODUCT;
    m_score_second = SCORE::SLACK;
}


/* ================================================ */
/* ================================================ */

Pairing_P_To_T::Pairing_P_To_T(std::string algo_name, const Instance &instance):
        PairingToPairing(algo_name, instance)
{
    m_score_first = SCORE::PRODUCT;
    m_score_second = SCORE::TIGHT_FILL;
}

/* ================================================ */
/* ================================================ */

Pairing_S_To_P::Pairing_S_To_P(std::string algo_name, const Instance &instance):
        PairingToPairing(algo_name, instance)
{
    m_score_first = SCORE::SLACK;
    m_score_second = SCORE::PRODUCT;
}

/* ================================================ */
/* ================================================ */

Pairing_S_To_S::Pairing_S_To_S(std::string algo_name, const Instance &instance):
        PairingToPairing(algo_name, instance)
{
    m_score_first = SCORE::SLACK;
    m_score_second = SCORE::SLACK;
}

/* ================================================ */
/* ================================================ */

Pairing_S_To_T::Pairing_S_To_T(std::string algo_name, const Instance &instance):
        PairingToPairing(algo_name, instance)
{
    m_score_first = SCORE::SLACK;
    m_score_second = SCORE::TIGHT_FILL;
}

/* ================================================ */
/* ================================================ */
Pairing_T_To_P::Pairing_T_To_P(std::string algo_name, const Instance &instance):
        PairingToPairing(algo_name, instance)
{
    m_score_first = SCORE::TIGHT_FILL;
    m_score_second = SCORE::PRODUCT;
}

/* ================================================ */
/* ================================================ */

Pairing_T_To_S::Pairing_T_To_S(std::string algo_name, const Instance &instance):
        PairingToPairing(algo_name, instance)
{
    m_score_first = SCORE::TIGHT_FILL;
    m_score_second = SCORE::SLACK;
}

/* ================================================ */
/* ================================================ */

Pairing_T_To_T::Pairing_T_To_T(std::string algo_name, const Instance &instance):
        PairingToPairing(algo_name, instance)
{
    m_score_first = SCORE::TIGHT_FILL;
    m_score_second = SCORE::TIGHT_FILL;
}

/* ================================================ */
/* ================================================ */



