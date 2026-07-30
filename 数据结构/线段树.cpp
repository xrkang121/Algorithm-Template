/*
需要根据题目修改的部分：

1. Node：一个区间需要维护什么信息
2. Tag：区间修改需要什么懒标记
3. mergeNode：父节点如何由左右儿子得到
4. applyTag：修改如何作用到一个节点
5. mergeTag：两个懒标记如何叠加
*/

struct Node
{
    int sum;
    int mn;
    int mx;
    int len;

    // 空节点，用于区间查询合并
    Node()
    {
        sum = 0;
        mn = INF;
        mx = -INF;
        len = 0;
    }

    // 根据一个数组元素建立叶子节点
    Node(int val)
    {
        sum = val;
        mn = val;
        mx = val;
        len = 1;
    }
};

struct Tag
{
    int add;

    // 空懒标记
    Tag()
    {
        add = 0;
    }

    // 构造区间加法标记
    Tag(int val)
    {
        add = val;
    }
};

// 父节点由左右儿子合并得到
Node mergeNode(const Node &left, const Node &right)
{
    if (left.len == 0)
    {
        return right;
    }

    if (right.len == 0)
    {
        return left;
    }

    Node res;

    res.sum = left.sum + right.sum;
    res.mn = min(left.mn, right.mn);
    res.mx = max(left.mx, right.mx);
    res.len = left.len + right.len;

    return res;
}

// 把一个区间修改作用到节点上
void applyTag(Node &node, const Tag &tag)
{
    node.sum += tag.add * node.len;
    node.mn += tag.add;
    node.mx += tag.add;
}

// 把新的懒标记叠加到原来的懒标记上
void mergeTag(Tag &oldTag, const Tag &newTag)
{
    oldTag.add += newTag.add;
}

struct SegmentTree
{
    int n;

    vector<Node> tr;
    vector<Tag> lazy;
    vector<int> haveLazy;

    SegmentTree()
    {
        n = 0;
    }

    // arr必须是1-based数组
    SegmentTree(const vector<int> &arr)
    {
        init(arr);
    }

    void pushUp(int id)
    {
        // 这就是一个节点和左右儿子的数据关系
        tr[id] = mergeNode(tr[id * 2], tr[id * 2 + 1]);
    }

    void apply(int id, const Tag &tag)
    {
        applyTag(tr[id], tag);

        if (haveLazy[id] == 0)
        {
            lazy[id] = tag;
            haveLazy[id] = 1;
        }
        else
        {
            mergeTag(lazy[id], tag);
        }
    }

    void pushDown(int id)
    {
        if (haveLazy[id] == 0)
        {
            return;
        }

        apply(id * 2, lazy[id]);
        apply(id * 2 + 1, lazy[id]);

        lazy[id] = Tag();
        haveLazy[id] = 0;
    }

    void build(int id, int l, int r, const vector<int> &arr)
    {
        if (l == r)
        {
            tr[id] = Node(arr[l]);
            return;
        }

        int mid = (l + r) / 2;

        build(id * 2, l, mid, arr);
        build(id * 2 + 1, mid + 1, r, arr);

        pushUp(id);
    }

    void init(const vector<int> &arr)
    {
        n = arr.size() - 1;

        tr.assign(4 * n + 5, Node());
        lazy.assign(4 * n + 5, Tag());
        haveLazy.assign(4 * n + 5, 0);

        build(1, 1, n, arr);
    }

    void change(int id, int l, int r, int pos, int val)
    {
        if (l == r)
        {
            tr[id] = Node(val);
            lazy[id] = Tag();
            haveLazy[id] = 0;
            return;
        }

        pushDown(id);

        int mid = (l + r) / 2;

        if (pos <= mid)
        {
            change(id * 2, l, mid, pos, val);
        }
        else
        {
            change(id * 2 + 1, mid + 1, r, pos, val);
        }

        pushUp(id);
    }

    // 把位置pos直接修改成val
    void change(int pos, int val)
    {
        change(1, 1, n, pos, val);
    }

    void rangeChange(
        int id,
        int l,
        int r,
        int ql,
        int qr,
        const Tag &tag)
    {
        if (ql <= l && r <= qr)
        {
            apply(id, tag);
            return;
        }

        pushDown(id);

        int mid = (l + r) / 2;

        if (ql <= mid)
        {
            rangeChange(id * 2, l, mid, ql, qr, tag);
        }

        if (qr > mid)
        {
            rangeChange(id * 2 + 1, mid + 1, r, ql, qr, tag);
        }

        pushUp(id);
    }

    // 给区间[l,r]进行一次修改
    void rangeChange(int l, int r, const Tag &tag)
    {
        rangeChange(1, 1, n, l, r, tag);
    }

    Node query(int id, int l, int r, int ql, int qr)
    {
        if (ql <= l && r <= qr)
        {
            return tr[id];
        }

        pushDown(id);

        int mid = (l + r) / 2;

        if (qr <= mid)
        {
            return query(id * 2, l, mid, ql, qr);
        }

        if (ql > mid)
        {
            return query(id * 2 + 1, mid + 1, r, ql, qr);
        }

        Node left = query(id * 2, l, mid, ql, qr);
        Node right = query(id * 2 + 1, mid + 1, r, ql, qr);

        return mergeNode(left, right);
    }

    // 查询区间[l,r]
    Node query(int l, int r)
    {
        return query(1, 1, n, l, r);
    }

    // 查询单点pos
    Node get(int pos)
    {
        return query(pos, pos);
    }
};
