enum EdgeType { INPUT, ARTIFICIAL };
struct EdgeData {
    int idx;
    EdgeType type;

    EdgeData(): idx(0), type(INPUT) {}

    EdgeData(int _idx): idx(_idx), type(INPUT) {}

    EdgeData(int _idx, EdgeType _type): idx(_idx), type(_type) {}

    void set_idx(int _idx) {
        idx = _idx;
    }
};
