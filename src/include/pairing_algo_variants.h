//
// Created by weldeab solomon on 06/03/2023.
//

#ifndef INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_PAIRING_ALGO_VARIANTS_H
#define INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_PAIRING_ALGO_VARIANTS_H

#include "base_algorithm.h"
#include "itemCentricAlgo.h"


using namespace onedpacking;

enum class SCORE { // Item-bin scores
    PRODUCT
    ,SLACK
    ,TIGHT_FILL
};

class PairingAlgorithm: public BaseAlgorithm
{
public:

    PairingAlgorithm(std::string algo_name, const Instance &instance );
    virtual unsigned long solveInstance(int hint_nb_bins = 0);

    virtual int solveInstanceMultiBin(int LB, int UB);

protected:
    virtual bool tryToSolve(int m_bins);
    void createNewBins(int m_bins);
    float computeItemBinScore(Item* item, Bin* bin);
    bool packItems(std::vector<Bin*>::iterator start_bin_it);
    void updateScores(Bin* bin, std::vector<Item*>::iterator first_item, std::vector<Item*>::iterator end_it);
    void updateBestSolutionBins(const std::vector<Bin*>& activated_bins);




protected:
    SCORE m_score;
    std::vector<std::vector<float>> m_bin_item_scores;
    std::vector<Item*>::iterator m_first_remaining_item;
    std::vector<Bin*> m_best_solution_bins;
};





#endif //INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_PAIRING_ALGO_VARIANTS_H
