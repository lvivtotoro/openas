#ifndef OAS_AUTM
#define OAS_AUTM

#include"vivs.h"

#include<type_traits>
#include<unordered_map>
#include"sparsepp/spp.h"
#include"pos.h"

namespace as {

template<typename CellValue>
class OASEX dautm {
    static_assert(std::is_scalar<CellValue>::value, "Template parameter CellValue may only be a scalar.");

    public:
        struct OASEX ruleset {
            virtual void handle(dautm<CellValue>&) = 0;
        };
        struct OASEX rulesets {
            class neighbor;
            class lambda;
        };
    private:
        void* rulesUserdata = NULL;
        ruleset* rules = NULL;

        std::unordered_map<pos, CellValue> oldCells;
        std::unordered_map<pos, CellValue> cells;
    public:
        dautm(ruleset* rules) : rules(rules) {}

        void step(int n = 1) {
            for(int i = 0; i < n; i++) {
                oldCells.clear();
                oldCells.swap(cells);

                rules->handle(*this);
            }
        }

        void set(pos::coord_type x, pos::coord_type y, CellValue val) {
            cells.emplace(pos{x, y}, val);
        }

        CellValue get(const pos& p) {
            auto a = cells.find(p);
            if(a == cells.end()) {
                return 0;
            } else {
                return a->second;
            }
        }

        void getInto(CellValue* arr, int x, int y, int w, int h) {
            for(int i = 0; i < w; i++) {
                for(int j = 0; j < h; j++) {
                    auto a = cells.find(pos{x + i, y + j});
                    if(a == cells.end()) {
                        arr[i * w + j] = 0;
                    } else {
                        arr[i * w + j] = a->second;
                    }
                }
            }
        }
};

template<typename CellValue>
struct OASEX dautm<CellValue>::rulesets::lambda : dautm<CellValue>::ruleset {

    typedef void(*lambda_type)(dautm<CellValue>&);

    lambda_type func;

    lambda(lambda_type func) : func(func) {
    }

    virtual void handle(dautm<CellValue>& sim) override {
        this->func(sim);
    }

};

}

#endif // OAS_AUTM
