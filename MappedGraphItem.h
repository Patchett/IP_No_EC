#include "common.h"

class MappedGraphItem {
public:

    MappedGraphItem(int ari, int aci, float w) :
        adj_row_index(ari),
        adj_col_index(aci),
        weight(w) {}

    MappedGraphItem() :
        adj_row_index(0),
        adj_col_index(0),
        weight(0) {}

    ~MappedGraphItem() {}
    int adj_row_index;
    int adj_col_index;
    float weight;
};