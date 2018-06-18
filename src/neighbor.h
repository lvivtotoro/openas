#ifndef NEIGHBOR_H_INCLUDED
#define NEIGHBOR_H_INCLUDED

#include"autm.h"
#include<vector>

namespace as {

template<typename CellValue>
struct OASEX dautm<CellValue>::rulesets::neighbor : dautm<CellValue>::ruleset {

    struct gol;
    struct lambda;

    // Phewwweeeeee, I can't explain this in text, basically it takes advantage of the fact that
    // all neighbor-based CAs have more than 2 possible states.
    virtual void handle(dautm<CellValue>& sim) override final {
        std::vector<as::pos> testNext;

        for(auto& cell : sim.oldCells) {
            as::pos p(cell.first);
            CellValue v(cell.second);

            CellValue neighbors[3][3];
            for(int i = -1; i <= 1; i++) {
                for(int j = -1; j <= 1; j++) {
                    as::pos np{p.x + i, p.y + j};
                    auto n = sim.oldCells.find(np);
                    if(n == sim.oldCells.end()) {
                        testNext.push_back(np);
                        neighbors[i + 1][j + 1] = 0;
                    } else {
                        neighbors[i + 1][j + 1] = n->second;
                    }
                }
            }

            sim.cells.emplace(pos(cell.first), this->select(sim, neighbors));
        }
        for(as::pos& p : testNext) {
            CellValue neighbors[3][3];
            for(int i = -1; i <= 1; i++) {
                for(int j = -1; j <= 1; j++) {
                    as::pos np{p.x + i, p.y + j};
                    auto n = sim.oldCells.find(np);
                    if(n == sim.oldCells.end()) {
                        neighbors[i + 1][j + 1] = 0;
                    } else {
                        neighbors[i + 1][j + 1] = n->second;
                    }
                }
            }
            sim.cells.emplace(p, this->select(sim, neighbors));
        }
    };

    virtual CellValue select(dautm<CellValue>& sim, CellValue neighbors[3][3]) = 0;

};

template<typename CellValue>
struct OASEX dautm<CellValue>::rulesets::neighbor::gol : dautm<CellValue>::rulesets::neighbor {

    virtual CellValue select(dautm<CellValue>& sim, CellValue neighbors[3][3]) {
        CellValue curVal = neighbors[1][1];

        CellValue neighborVals = 0;
        neighborVals += neighbors[0][0];
        neighborVals += neighbors[0][1];
        neighborVals += neighbors[0][2];
        neighborVals += neighbors[1][0];
        neighborVals += neighbors[1][2];
        neighborVals += neighbors[2][0];
        neighborVals += neighbors[2][1];
        neighborVals += neighbors[2][2];

        if(curVal) {
            if(neighborVals < 2) return 0;
            else if(neighborVals > 3) return 0;
            else return 1;
        } else {
            if(neighborVals == 3) return 1;
            else return 0;
        }
    }

};

template<typename CellValue>
struct OASEX dautm<CellValue>::rulesets::neighbor::lambda : dautm<CellValue>::rulesets::neighbor {

    typedef CellValue(*lambda_type)(dautm<CellValue>&, CellValue[3][3]);

    lambda_type func;

    lambda(lambda_type func) : func(func) {
    }

    virtual CellValue select(dautm<CellValue>& sim, CellValue neighbors[3][3]) {
        return this->func(sim, neighbors);
    }

};

}

#endif // NEIGHBOR_H_INCLUDED
