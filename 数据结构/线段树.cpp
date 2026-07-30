const int INF = numeric_limits<int>::max() / 2;
/*
线段树使用方法：

1. 建树：
vector<Node> arr(n + 1);
for (int i = 1; i <= n; i++)
{
    int x;
    cin >> x;
    arr[i] = Node(x);
}
SegmentTree tree(arr);

2. 单点修改：
tree.change(pos, Node(val));

3. 单点查询：
Node res = tree.get(pos);

4. 区间查询：
Node res = tree.query(l, r);

5. 区间修改：
tree.rangeChange(l, r, Tag(val));

注意：
1. 数组必须是1-based。
2. 线段树放在solve()里面，多组测试不会累计内存。
3. Node()必须表示空节点。
4. Tag表示一次区间修改。
5. 如果题目没有区间修改，可以不调用rangeChange。
*/

// ==================================================
// ===== 这里写一个节点需要保存的信息 ================
// ==================================================

struct Node
{
    int sum;
    int mn;
    int mx;
    int len;

    // 空节点，用于区间查询时合并
    Node()
    {
        sum = 0;
        mn = INF;
        mx = -INF;
        len = 0;
    }

    // 根据一个数组元素创建叶子节点
    Node(int val)
    {
        sum = val;
        mn = val;
        mx = val;
        len = 1;
    }
};

// ==================================================
// ===== 这里写区间修改需要保存的懒标记 ==============
// ==================================================

struct Tag
{
    int add;

    // 空懒标记
    Tag()
    {
        add = 0;
    }

    // 根据一次修改创建懒标记
    Tag(int val)
    {
        add = val;
    }
};

// ==================================================
// ===== 这里写父节点和左右儿子的关系 ================
// ==================================================

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

// ==================================================
// ===== 这里写一次区间修改如何影响节点信息 ==========
// ==================================================

void applyTag(Node &node, const Tag &tag)
{
    node.sum += tag.add * node.len;
    node.mn += tag.add;
    node.mx += tag.add;
}

// ==================================================
// ===== 这里写两个懒标记如何叠加 ====================
// ==================================================

void mergeTag(Tag &oldTag, const Tag &newTag)
{
    oldTag.add += newTag.add;
}

// ==================================================
// ===== 下面是线段树主体，一般不需要修改 ============
// ==================================================

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

    // 根据1-based节点数组建树
    SegmentTree(const vector<Node> &arr)
    {
        init(arr);
    }

    // 用左右儿子更新父节点
    void pushUp(int id)
    {
        tr[id] = mergeNode(tr[id * 2], tr[id * 2 + 1]);
    }

    // 给当前节点添加修改
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

    // 把懒标记传给左右儿子
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

    // 递归建树
    void build(int id, int l, int r, const vector<Node> &arr)
    {
        if (l == r)
        {
            tr[id] = arr[l];
            return;
        }

        int mid = (l + r) / 2;

        build(id * 2, l, mid, arr);
        build(id * 2 + 1, mid + 1, r, arr);

        pushUp(id);
    }

    // 初始化线段树
    void init(const vector<Node> &arr)
    {
        n = arr.size() - 1;

        tr.assign(4 * n + 5, Node());
        lazy.assign(4 * n + 5, Tag());
        haveLazy.assign(4 * n + 5, 0);

        build(1, 1, n, arr);
    }

    // 单点修改
    void change(
        int id,
        int l,
        int r,
        int pos,
        const Node &val)
    {
        if (l == r)
        {
            tr[id] = val;
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
    void change(int pos, const Node &val)
    {
        change(1, 1, n, pos, val);
    }

    // 区间修改
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

    // 对区间[l,r]执行tag修改
    void rangeChange(int l, int r, const Tag &tag)
    {
        rangeChange(1, 1, n, l, r, tag);
    }

    // 区间查询
    Node query(
        int id,
        int l,
        int r,
        int ql,
        int qr)
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

    // 查询位置pos
    Node get(int pos)
    {
        return query(pos, pos);
    }

    // 查询整个数组
    Node all()
    {
        return tr[1];
    }
};
