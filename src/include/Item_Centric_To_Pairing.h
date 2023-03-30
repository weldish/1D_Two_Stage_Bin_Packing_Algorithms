//
// Created by weldeab solomon on 12/03/2023.
//

#ifndef INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_ITEM_CENTRIC_TO_PAIRING_H
#define INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_ITEM_CENTRIC_TO_PAIRING_H




#include "base_algorithm.h"
#include "pairing_algo_variants.h"

class ItemCentricPairingAlgo : public BaseAlgorithm
{
public:
    ItemCentricPairingAlgo(std::string algo_name, const Instance &instance);

    virtual unsigned long solveInstance(int hint_nb_bins = 0);
    virtual bool tryToSolve(int m_bins);


    // 1. this method creates a list of breakpoints and calls the solveInstnaceMultiBin method for every breakpoint.
    // it keeps track of the best solution and number of iterations that led to that best solution
    // it returns the number of bins in the best solution
    //the value of breakpoints lies within [1, LB)
    // the value of breakpoints should round down and be an integer
    virtual int solveForMultiBreakPoints(float b_factor, int LB, int UB);

    // 2. uses binary search
    // returns the number of bins used
    // starts with the UB which is computed by FF algorithm
    //breakpoint lies within [1, LB)
    virtual int solveInstanceMultiBin(int breakpoint, int LB, int UB);
protected:
    // 3. given a limit bins and m number of bins, try solving the instance
    // the paramater m_bins is changed incrementaly.
    // starting value for m_bins = breakpoint + 1
    // m_bins is within the range  [LB, UB]
    // breakpoint lies within [1, LB)
    virtual bool trySolveTwoStage(int breakpoint, int m_bins);

    // 4. uses an item-centric algorithm to pack the first subset of items
    bool solveItemCentric(int breakpoint,int m_bins, std::vector<Item*>::iterator start_it, std::vector<Item*>::iterator end_it);
    // 5. uses an pairing algorithm to pack the rest subset of items
    bool solvePairing(int breakpoint, int m_bins, std::vector<Item*>::iterator last_it, std::vector<Item*>::iterator end_it, SCORE score);

    void updateScores(Bin* bin, std::vector<Item*>::iterator first_item, std::vector<Item*>::iterator end_it);

    float computeItemBinScore(Item* item, Bin* bin);

    void updateBestSolutionBins(const std::vector<Bin*>& activated_bins);

    void sortBins();
    void sortItems();

    // 1. clear the solution()
    // 2. clear the item-bin score vector
    // 3. resize the item-bin score vector to m bins
    // 4. depending on the type, call either FFD or BFD or WFD
    // 5. call pairing()


protected:
    bool m_is_FFD_type;
    bool m_is_BFD_type;
    bool m_is_WFD_type;
    SCORE m_score;
    int bin_increment;
    int limit_increment;

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
