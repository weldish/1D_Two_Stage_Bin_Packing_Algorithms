//
// Created by weldeab solomon on 12/03/2023.
//

#ifndef INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_ITEM_CENTRIC_TO_PAIRING_H
#define INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_ITEM_CENTRIC_TO_PAIRING_H




#include "base_algorithm.h"
#include "pairing_algo_variants.h"

// for two stage algorithms that construct a solution by applying an item-centric algorithm followed by Multi-bin pairing algorithm
class ItemCentricPairingAlgo : public BaseAlgorithm
{
public:
    ItemCentricPairingAlgo(std::string algo_name, const Instance &instance);

    virtual unsigned long solveInstWithIC(int hint_nb_bins = 0);
    virtual bool tryToSolve(int m_bins);


    // This method generates a list of breakpoints and calls the trySolveTwoStage algorithm  for every breakpoint.
    // it keeps track of the best solution
    // it returns the number of bins in the best solution
    virtual int solveInstWith2SA(float b_factor, int LB, int UB);

    // returns the number of bins used
    // search for a feasible solution between the LB and UP via binary search.
    virtual int solveInstWithMBP(int breakpoint, int LB, int UB);
protected:
    // given a breakpoint and m number of bins, this method tries to solve the instance
    // the paramater m_bins is changed with every call to trySolveTwoStage
    // m_bins is within the range  [LB, UB]
    virtual bool trySolveTwoStage(int breakpoint, int m_bins);

    // uses an item-centric algorithm to pack the items untill the threshold/breakpoint is reached.
    bool solveItemCentric(int breakpoint,int m_bins, std::vector<Item*>::iterator start_it, std::vector<Item*>::iterator end_it);
    //  uses an MB-pairing algorithm to pack the rest of items
    bool solvePairing(int breakpoint, int m_bins, std::vector<Item*>::iterator last_it, std::vector<Item*>::iterator end_it, SCORE score);
    // updated the item-bin scores
    void updateScores(Bin* bin, std::vector<Item*>::iterator first_item, std::vector<Item*>::iterator end_it);
    // computes the score for a given item-bin pair.
    float computeItemBinScore(Item* item, Bin* bin);

    void updateBestSolutionBins(const std::vector<Bin*>& activated_bins);

    void sortBins();
    void sortItems();



protected:
    bool m_is_FFD_type;
    bool m_is_BFD_type;
    bool m_is_WFD_type;
    SCORE m_score;

    std::vector<Bin*> m_best_solution_bins;
    std::vector<std::vector<float>> m_bin_item_scores;



};


/* ================================================ */
/* ================================================ */


class FFD_Pairing_PRODUCT : public ItemCentricPairingAlgo
{
public:
    FFD_Pairing_PRODUCT(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class FFD_Pairing_SLACK : public ItemCentricPairingAlgo
{
public:
    FFD_Pairing_SLACK(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class FFD_Pairing_TIGHT_FILL : public ItemCentricPairingAlgo
{
public:
    FFD_Pairing_TIGHT_FILL (std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */



class BFD_Pairing_PRODUCT : public ItemCentricPairingAlgo
{
public:
    BFD_Pairing_PRODUCT(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class BFD_Pairing_SLACK : public ItemCentricPairingAlgo
{
public:
    BFD_Pairing_SLACK(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class BFD_Pairing_TIGHT_FILL : public ItemCentricPairingAlgo
{
public:
    BFD_Pairing_TIGHT_FILL (std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */


class WFD_Pairing_PRODUCT : public ItemCentricPairingAlgo
{
public:
    WFD_Pairing_PRODUCT(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class WFD_Pairing_SLACK : public ItemCentricPairingAlgo
{
public:
    WFD_Pairing_SLACK(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class WFD_Pairing_TIGHT_FILL : public ItemCentricPairingAlgo
{
public:
    WFD_Pairing_TIGHT_FILL (std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */




#endif //INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_ITEM_CENTRIC_TO_PAIRING_H
