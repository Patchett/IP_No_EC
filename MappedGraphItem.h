#include "common.h"

class MappedGraphItem {
public:

    MappedGraphItem(int ari, int aci, float w) :
        adj_row_index(ari),
        adj_col_index(aci),
        weight(w) {}

    int adj_row_index;
    int adj_col_index;
    int weight;
};