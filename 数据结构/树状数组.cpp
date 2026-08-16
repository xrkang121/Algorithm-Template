/*
树状数组统一板子

支持功能：

1. 单点加法
   a[x] += val
   tree.pointAdd(x, val);

2. 单点赋值
   a[x] = val
   tree.pointSet(x, val);

3. 单点查询
   查询 a[x]
   int value = tree.get(x);

4. 区间加法
   a[l...r] 全部加 val
   tree.rangeAdd(l, r, val);

5. 前缀和查询
   查询 a[1] + ... + a[x]
   int sum = tree.prefixSum(x);

6. 区间和查询
   查询 a[l] + ... + a[r]
   int sum = tree.rangeSum(l, r);

注意：

1. 本板子下标从 1 开始。
2. 本板子的区间修改是“区间加法”，不支持区间覆盖赋值。
3. pointSet 可以实现单个位置的覆盖赋值。
4. 如果需要区间覆盖赋值，通常使用带懒标记的线段树。
5. 所有操作的时间复杂度都是 O(log n)。
*/

#include <bits/stdc++.h>
using namespace std;

#define int long long

struct Fenwick
{
    int n;

    // tree1 维护差分，tree2 用于把差分还原成前缀和
    vector<int> tree1;
    vector<int> tree2;

    Fenwick(int _n = 0)
    {
        init(_n);
    }

    // 初始化长度为 n 的全 0 数组
    void init(int _n)
    {
        n = _n;

        tree1.assign(n + 2, 0);
        tree2.assign(n + 2, 0);
    }

    // 树状数组内部单点修改
    void add(vector<int> &tree, int x, int val)
    {
        while (x <= n)
        {
            tree[x] += val;
            x += x & -x;
        }
    }

    // 树状数组内部前缀查询
    int query(vector<int> &tree, int x)
    {
        int res = 0;

        while (x > 0)
        {
            res += tree[x];
            x -= x & -x;
        }

        return res;
    }

    // a[l...r] 全部加 val
    void rangeAdd(int l, int r, int val)
    {
        if (l > r)
        {
            return;
        }

        add(tree1, l, val);
        add(tree1, r + 1, -val);

        add(tree2, l, val * (l - 1));
        add(tree2, r + 1, -val * r);
    }

    // 查询 a[1] + a[2] + ... + a[x]
    int prefixSum(int x)
    {
        if (x <= 0)
        {
            return 0;
        }

        return query(tree1, x) * x - query(tree2, x);
    }

    // 查询 a[l] + a[l+1] + ... + a[r]
    int rangeSum(int l, int r)
    {
        if (l > r)
        {
            return 0;
        }

        return prefixSum(r) - prefixSum(l - 1);
    }

    // 查询单点 a[x]
    int get(int x)
    {
        return rangeSum(x, x);
    }

    // a[x] += val
    void pointAdd(int x, int val)
    {
        rangeAdd(x, x, val);
    }

    // a[x] = val
    void pointSet(int x, int val)
    {
        int old = get(x);
        rangeAdd(x, x, val - old);
    }

    // 使用一个 1-based 数组建立树状数组
    // arr[0] 不使用，有效数据为 arr[1...n]
    void build(const vector<int> &arr)
    {
        init((int)arr.size() - 1);

        for (int i = 1; i <= n; i++)
        {
            rangeAdd(i, i, arr[i]);
        }
    }
};

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    int n = 5;

    // 建立长度为 n、初始全部为 0 的数组
    Fenwick tree(n);

    // 相当于 a[2] += 5
    tree.pointAdd(2, 5);

    // 相当于 a[3] = 10
    tree.pointSet(3, 10);

    // 相当于 a[2...4] 全部加 3
    tree.rangeAdd(2, 4, 3);

    // 查询 a[3]
    cout << tree.get(3) << '\n';

    // 查询 a[1] + ... + a[4]
    cout << tree.prefixSum(4) << '\n';

    // 查询 a[2] + ... + a[4]
    cout << tree.rangeSum(2, 4) << '\n';

    /*
    如果已经有初始数组：

    int n;
    cin >> n;

    vector<int> arr(n + 1);

    for (int i = 1; i <= n; i++)
    {
        cin >> arr[i];
    }

    Fenwick tree;
    tree.build(arr);
    */

    return 0;
}
