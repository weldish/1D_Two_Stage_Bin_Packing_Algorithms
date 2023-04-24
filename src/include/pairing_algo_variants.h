//
// Created by weldeab solomon on 06/03/2023.
//

#ifndef INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_PAIRING_ALGO_VARIANTS_H
#define INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_PAIRING_ALGO_VARIANTS_H

#include "base_algorithm.h"
#include "itemCentricAlgo.h"


using namespace onedpacking;

// Item-bin scores
enum class SCORE {
    PRODUCT
    ,SLACK
    ,TIGHT_FILL
};

// this is a base class for Multi-bin algorithms

class PairingAlgorithm: public BaseAlgorithm
{
public:

    PairingAlgorithm(std::string algo_name, const Instance &instance );
    virtual unsigned long solveInstWithIC(int hint_nb_bins = 0); // non used for Multi-bin paring algorithms

    // solves an instance with a Multi-Bin Pairing algorithm
    virtual int solveInstWithMBP(int LB, int UB);

protected:
    // solves an instances with a given m bins.
    virtual bool tryToSolve(int m_bins);
    // creates a set of new m bins
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


/* ============================================================== */
/* ============================================================== */


class S_Pairing_PRODUCT : public PairingAlgorithm
{
public:
    S_Pairing_PRODUCT(std::string algo_name, const Instance &instance);
};

/* ============================================================== */
/* ============================================================== */


class S_Pairing_SLACK : public PairingAlgorithm
{
public:
    S_Pairing_SLACK(std::string algo_name, const Instance &instance);
};

/* ============================================================== */
/* ============================================================== */




class S_Pairing_TIGHT_FILL : public PairingAlgorithm
{
public:
    S_Pairing_TIGHT_FILL(std::string algo_name, const Instance &instance);
};



/* ============================================================== */
/* ============================================================== */




#endif //INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_PAIRING_ALGO_VARIANTS_H

