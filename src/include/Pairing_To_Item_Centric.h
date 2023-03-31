//
// Created by weldeab solomon on 12/03/2023.
//

#ifndef INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_PAIRING_TO_ITEM_CENTRIC_H
#define INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_PAIRING_TO_ITEM_CENTRIC_H




#include "base_algorithm.h"
#include "pairing_algo_variants.h"

class PairingToItemCentric : public BaseAlgorithm
{
public:
    PairingToItemCentric(std::string algo_name, const Instance &instance);

    virtual unsigned long solveInstance(int hint_nb_bins = 0);
    virtual bool tryToSolve(int m_bins);


    // 1. this method creates a list of breakpoints and calls the solveInstnaceMultiBin method for every breakpoint.
    // it keeps track of the best solution and number of iterations that led to that best solution
    // it returns the number of bins in the best solution
    //the value of breakpoints lies within [1, LB)
    // the value of breakpoints should round down and be an integer
    virtual int solveForMultiBreakPoints(float b_factor, int LB, int UB);


protected:
    // 3. given a limit bins and m number of bins, try solving the instance
    // the paramater m_bins is changed incrementaly.
    // starting value for m_bins = breakpoint + 1
    // m_bins is within the range  [LB, UB]
    // breakpoint lies within [1, LB)
    virtual int trySolveTwoStage(int breakpoint);

    // 4. uses an item-centric algorithm to pack the first subset of items
    void solveItemCentric(std::vector<Item*>::iterator last_it, std::vector<Item*>::iterator end_it);
    // 5. uses an pairing algorithm to pack the rest subset of items
    int solvePairing(int breakpoint,std::vector<Item*>::iterator start_it, std::vector<Item*>::iterator end_it);

    void updateScores(Bin* bin, std::vector<Item*>::iterator first_item, std::vector<Item*>::iterator end_it);

    float computeItemBinScore(Item* item, Bin* bin);



    void sortBins();
    void sortItems();



protected:
    bool m_is_FFD_type;
    bool m_is_BFD_type;
    bool m_is_WFD_type;
    SCORE m_score;

    std::vector<std::vector<float>> m_bin_item_scores;

};


/* ================================================ */
/* ================================================ */


class Pairing_PRODUCT_To_FFD : public PairingToItemCentric
{
public:
    Pairing_PRODUCT_To_FFD(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_SLACK_To_FFD : public PairingToItemCentric
{
public:
    Pairing_SLACK_To_FFD(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_TIGHT_FILL_To_FFD : public PairingToItemCentric
{
public:
    Pairing_TIGHT_FILL_To_FFD(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */



class Pairing_PRODUCT_To_BFD : public PairingToItemCentric
{
public:
    Pairing_PRODUCT_To_BFD(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_SLACK_To_BFD : public PairingToItemCentric
{
public:
    Pairing_SLACK_To_BFD(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_TIGHT_FILL_To_BFD : public PairingToItemCentric
{
public:
    Pairing_TIGHT_FILL_To_BFD (std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */


class Pairing_PRODUCT_To_WFD : public PairingToItemCentric
{
public:
    Pairing_PRODUCT_To_WFD(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_SLACK_To_WFD : public PairingToItemCentric
{
public:
    Pairing_SLACK_To_WFD(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_TIGHT_FILL_To_WFD : public PairingToItemCentric
{
public:
    Pairing_TIGHT_FILL_To_WFD (std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */





#endif //INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_PAIRING_TO_ITEM_CENTRIC_H
