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

template <class T, class Compare = less<T>>
struct MergeHeap
{
    struct Node
    {
        int ls;     // 左儿子编号
        int rs;     // 右儿子编号
        int dis;    // 左偏树距离
        int siz;    // 子树元素数量
        T val;      // 保存的数据
    };

    /*
    所有相同类型的合并堆共用一个动态节点池。

    tr[0] 表示空节点。
    vector 会随着插入自动扩大。
    */
    static inline vector<Node> tr = vector<Node>(1);

    // 比较函数
    static inline Compare cmp;

    // 当前堆的根节点，0表示空堆
    int root = 0;

    // 创建新节点，返回节点编号
    static int newNode(const T &val)
    {
        tr.push_back({
            0,      // 左儿子
            0,      // 右儿子
            1,      // 初始距离
            1,      // 初始大小
            val     // 保存的值
        });

        return tr.size() - 1;
    }

    // 合并两个根节点
    static int mergeNode(int x, int y)
    {
        // 一个堆为空，返回另一个
        if (x == 0 || y == 0)
        {
            return x + y;
        }

        /*
        如果x的优先级比y低，就交换。
        保证x是优先级更高的节点。
        */
        if (cmp(tr[x].val, tr[y].val))
        {
            swap(x, y);
        }

        // 把y合并进x的右子树
        tr[x].rs = mergeNode(tr[x].rs, y);

        // 保证左儿子的距离不小于右儿子
        if (tr[tr[x].ls].dis < tr[tr[x].rs].dis)
        {
            swap(tr[x].ls, tr[x].rs);
        }

        // 更新距离和大小
        tr[x].dis = tr[tr[x].rs].dis + 1;

        tr[x].siz = tr[tr[x].ls].siz
                  + tr[tr[x].rs].siz + 1;

        return x;
    }

    // 判断是否为空
    bool empty() const
    {
        return root == 0;
    }

    // 返回元素个数
    int size() const
    {
        return tr[root].siz;
    }

    // 返回优先级最高的元素，使用前要保证非空
    const T &top() const
    {
        return tr[root].val;
    }

    // 插入新元素
    void push(const T &val)
    {
        root = mergeNode(root, newNode(val));
    }

    // 删除优先级最高的元素
    void pop()
    {
        if (empty())
        {
            return;
        }

        root = mergeNode(tr[root].ls, tr[root].rs);
    }

    /*
    把other合并进当前堆。

    合并后：
    当前堆包含所有元素；
    other变成空堆。
    */
    void merge(MergeHeap &other)
    {
        root = mergeNode(root, other.root);
        other.root = 0;
    }

    /*
    清空当前堆。

    这里只让当前堆变为空堆，
    不会释放整个公共节点池。
    */
    void clear()
    {
        root = 0;
    }

    /*
    如果大概知道元素数量，可以提前申请空间，
    减少vector扩容次数，但不是必须调用。
    */
    static void reserve(int n)
    {
        tr.reserve(n + 1);
    }

    /*
    清空整个公共节点池。

    只能在所有同类型合并堆都不再使用时调用，
    例如每组测试数据结束，并且之前的堆全部作废。
    */
    static void resetPool()
    {
        tr.clear();
        tr.resize(1);
    }
};
