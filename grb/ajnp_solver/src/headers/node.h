struct NodeData {
    int idx;
    bool to_be_removed;
    bool is_maximum;
    bool is_root;

    NodeData(): idx(0), to_be_removed(false), is_maximum(false), is_root(false) {}

    NodeData(int _idx): idx(_idx), to_be_removed(false), is_maximum(false), is_root(false) {}

    NodeData(int _idx, bool _is_root, bool _to_be_removed):
		idx(_idx), to_be_removed(_to_be_removed), is_maximum(false), is_root(_is_root) {}

    NodeData(int _idx, bool _is_root, bool _to_be_removed, bool _is_maximum):
		idx(_idx), to_be_removed(_to_be_removed), is_maximum(_is_maximum), is_root(_is_root) {}

    void set_idx(int _idx) {
        idx = _idx;
    }
};
