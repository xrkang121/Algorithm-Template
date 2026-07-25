/*
合并堆（左偏树）

支持操作：
1. push(x)       插入一个元素
2. top()         返回优先级最高的元素
3. pop()         删除优先级最高的元素
4. empty()       判断堆是否为空
5. size()        返回元素个数
6. merge(other)  把另一个堆合并进当前堆

Compare 的写法与 priority_queue 相同：
cmp(x, y) == true 表示 x 的优先级比 y 低。

默认 less<T>：
x < y 时，x 优先级低，因此是大根堆。
*/

template <class T, int MAXN, class Compare = less<T>>
struct MergeHeap
{
    struct Node
    {
        int ls;     // 左儿子
        int rs;     // 右儿子
        int dis;    // 左偏树的距离
        int siz;    // 当前子树的元素个数
        T val;      // 当前节点保存的值
    };

    /*
    所有相同类型的 MergeHeap 共用同一个节点池。

    这样两个堆合并时，只需要合并两个根节点，
    不需要逐个取出再插入。
    */
    static inline Node tr[MAXN];

    // 当前一共创建了多少个节点
    static inline int cnt = 0;

    // 优先级比较函数
    static inline Compare cmp;

    // 当前堆的根节点，root == 0 表示空堆
    int root = 0;

    /*
    创建一个新节点，返回新节点编号。

    注意：
    MAXN 必须大于所有堆总共执行 push 的次数。
    被 pop 的节点不会重新利用。
    */
    static int newNode(T val)
    {
        cnt++;

        tr[cnt].ls = 0;
        tr[cnt].rs = 0;
        tr[cnt].dis = 1;
        tr[cnt].siz = 1;
        tr[cnt].val = val;

        return cnt;
    }

    /*
    合并以 x、y 为根的两棵左偏树，
    返回合并后的根节点。

    这是整个合并堆最核心的函数。
    一般不需要在外面直接调用。
    */
    static int mergeNode(int x, int y)
    {
        // 有一个为空，直接返回另一个
        if (x == 0 || y == 0)
        {
            return x + y;
        }

        /*
        cmp(tr[x].val, tr[y].val) == true
        表示 x 的优先级比 y 低。

        所以交换 x、y，保证 x 始终是优先级更高的节点。
        */
        if (cmp(tr[x].val, tr[y].val))
        {
            swap(x, y);
        }

        // 把 y 合并进 x 的右子树
        tr[x].rs = mergeNode(tr[x].rs, y);

        /*
        左偏树要求：
        左儿子的 dis 不能小于右儿子的 dis。

        如果不满足，就交换左右儿子。
        */
        if (tr[tr[x].ls].dis < tr[tr[x].rs].dis)
        {
            swap(tr[x].ls, tr[x].rs);
        }

        // 根据右儿子更新距离
        tr[x].dis = tr[tr[x].rs].dis + 1;

        // 更新当前子树大小
        tr[x].siz = tr[tr[x].ls].siz
                  + tr[tr[x].rs].siz + 1;

        return x;
    }

    // 判断堆是否为空
    bool empty()
    {
        return root == 0;
    }

    // 返回当前堆的元素个数
    int size()
    {
        return tr[root].siz;
    }

    /*
    返回优先级最高的元素。

    使用前必须保证堆不为空：
    if (!q.empty())
    {
        cout << q.top();
    }
    */
    T top()
    {
        return tr[root].val;
    }

    // 插入一个新元素
    void push(T val)
    {
        int now = newNode(val);
        root = mergeNode(root, now);
    }

    /*
    删除优先级最高的元素。

    原来的根被删除后，
    把它的左子树和右子树合并起来作为新根。
    */
    void pop()
    {
        // 空堆不能删除
        if (empty())
        {
            return;
        }

        root = mergeNode(tr[root].ls, tr[root].rs);
    }

    /*
    把 other 合并到当前堆。

    合并之后：
    1. 当前堆拥有两个堆的所有元素
    2. other 变成空堆
    */
    void merge(MergeHeap &other)
    {
        root = mergeNode(root, other.root);
        other.root = 0;
    }
};
