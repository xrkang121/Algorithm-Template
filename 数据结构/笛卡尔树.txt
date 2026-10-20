笛卡尔树（最大 / 最小通用）—— 单调栈 O(n)

【什么时候想到笛卡尔树】
1. 题目围绕“一个区间内的最大值 / 最小值（或其位置）”展开，尤其需要按最值位置把区间分成左右两段。
2. 每个区间都能选择一个最值位置作“根”，左右区间递归处理；想把这种区间分治变为一棵树，再做树形 DP / 计数。
3. 需要大量查询区间最大值 / 最小值的位置：笛卡尔树建好后，区间 [l,r] 的最值位置对应 LCA(l,r)（LCA 还需另行预处理）。
4. 要统计每个元素作为某些子数组最大 / 最小值的贡献；或者题目涉及最近更大 / 更小元素形成的区间结构。

【核心性质】
1. 节点编号就是原数组下标 i，节点权值是 a[i]。
2. 中序遍历的节点顺序恒为 1,2,...,n。
3. 最大笛卡尔树：父节点权值 >= 儿子权值；最小笛卡尔树：父节点权值 <= 儿子权值。
4. 每个节点最多一个父亲、一个左儿子、一个右儿子；root 对应全局最值位置。
5. 任意节点 u 的子树对应原数组的一个连续区间 [L[u], R[u]]；u 是该区间按规定选出的最值位置。
6. 不同区间的最值位置关系，可以借这棵树转化成祖先关系；常与树形 DP、LCA、区间计数结合。
7. 当数组有相同值时，必须明确“最左最值”还是“最右最值”，比较符号决定优先级。

【四种构造：只修改 while 比较符号】
最大值 + 相等取最左：a[stk.back()] <  a[i]
最大值 + 相等取最右：a[stk.back()] <= a[i]
最小值 + 相等取最左：a[stk.back()] >  a[i]
最小值 + 相等取最右：a[stk.back()] >= a[i]

相等取最左时：
- 最大树：左儿子值 < 父节点值，右儿子值 <= 父节点值。
- 最小树：左儿子值 > 父节点值，右儿子值 >= 父节点值。
相等取最右时：
- 最大树：左儿子值 <= 父节点值，右儿子值 < 父节点值。
- 最小树：左儿子值 >= 父节点值，右儿子值 > 父节点值。

【完整可复制板子：默认最大值 + 相等取最左】
#include <bits/stdc++.h>
using namespace std;

#define int long long

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    int n;
    cin >> n;

    vector<int> arr(n + 1);
    for (int i = 1; i <= n; i++)
    {
        cin >> arr[i];
    }

    vector<int> l_son(n + 1, 0);
    vector<int> r_son(n + 1, 0);
    vector<int> fa(n + 1, 0);
    vector<int> stk;

    for (int i = 1; i <= n; i++)
    {
        int last = 0;

        while (!stk.empty() && arr[stk.back()] < arr[i])
        {
            last = stk.back();
            stk.pop_back();
        }

        if (!stk.empty())
        {
            r_son[stk.back()] = i;
            fa[i] = stk.back();
        }

        if (last != 0)
        {
            l_son[i] = last;
            fa[last] = i;
        }

        stk.push_back(i);
    }

    int root = stk[0];

    return 0;
}

【赛场要改什么】
1. 选“最大”还是“最小”；相等时取“最左”还是“最右”，只需换 while 条件。
2. 输入 arr[i]；后续算法直接使用 root、l_son[i]、r_son[i]、fa[i]。
3. l_son[i] / r_son[i] / fa[i] 为 0 表示不存在。
4. 若需要树形 DP，按儿子在前、父亲在后的顺序处理；深树优先用非递归遍历。

【如需树形 DP：非递归得到自底向上的处理顺序】
在上述代码求出 root 后，按需插入：

vector<int> order;
vector<int> stk2;
stk2.push_back(root);

while (!stk2.empty())
{
    int u = stk2.back();
    stk2.pop_back();
    order.push_back(u);

    if (l_son[u] != 0)
    {
        stk2.push_back(l_son[u]);
    }

    if (r_son[u] != 0)
    {
        stk2.push_back(r_son[u]);
    }
}

for (int i = (int)order.size() - 1; i >= 0; i--)
{
    int u = order[i];
}

【常用性质：子数组贡献】
设以 u 为根的子树是下标区间 [L[u], R[u]]，且使用固定的相等值优先级：
- u 作为子数组 [l,r] 的“被选中最值位置”的子数组数量为
  (u - L[u] + 1) * (R[u] - u + 1)。
- 若只需要最值本身而不关心是哪一个相同元素，则不同的相等值优先级都可以，计数时保持统一即可。

【构造过程记忆】
每个 i 入栈一次，最多出栈一次。
- 弹出的都是在新节点 i 出现后不能继续待在主链上的点；最后一个弹出的 last 变成 i 的左儿子。
- 若栈非空，剩余栈顶变成 i 的父亲，i 是它的右儿子。
- 最后栈底 stk[0] 是根节点。

【复杂度】
构造 O(n) 时间、O(n) 空间。
附加 LCA / DP 的复杂度取决于你采用的具体算法。
