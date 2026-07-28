/*
合并堆（左偏树）

支持操作：
1. push(x)       插入一个元素
2. top()         返回优先级最高的元素
3. pop()         删除优先级最高的元素
4. empty()       判断堆是否为空
5. size()        返回元素个数
6. merge(other)  把另一个堆合并进当前堆
7. clear()       清空当前堆

Compare 的写法与 priority_queue 相同：
cmp(x, y) == true 表示 x 的优先级比 y 低。

默认 less<T>：
x < y 时，x 优先级低，因此是大根堆。

大根堆：
MergeHeap<int> Q;

小根堆：
MergeHeap<int, greater<int>> Q;
*/

template <class T, class Compare = less<T>>
struct MergeHeap
{
    struct Node
    {
        int ls;  // 左儿子编号
        int rs;  // 右儿子编号
        int dis; // 左偏树距离
        int siz; // 子树元素数量
        T val;   // 保存的数据
    };

    /*
    所有相同类型的合并堆共用一个动态节点池。

    tr[0]表示空节点。
    vector会随着插入自动扩大。
    */
    static inline vector<Node> tr = vector<Node>(1);

    /*
    保存已经删除的节点编号。

    后面创建新节点时，
    优先使用这些被删除的节点，
    避免反复pop、push导致内存不断增加。
    */
    static inline vector<int> freeNode;

    // 比较函数
    static inline Compare cmp;

    // 当前堆的根节点，0表示空堆
    int root = 0;

    // 创建新节点，返回节点编号
    static int newNode(const T &val)
    {
        /*
        如果存在已经删除的节点，
        直接重新使用这个节点。
        */
        if (!freeNode.empty())
        {
            int id = freeNode.back();
            freeNode.pop_back();

            tr[id] = {
                0,   // 左儿子
                0,   // 右儿子
                1,   // 初始距离
                1,   // 初始大小
                val  // 保存的值
            };

            return id;
        }

        // 没有空闲节点时，才创建新节点
        tr.push_back({
            0,   // 左儿子
            0,   // 右儿子
            1,   // 初始距离
            1,   // 初始大小
            val  // 保存的值
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

        // 更新距离
        tr[x].dis = tr[tr[x].rs].dis + 1;

        // 更新子树大小
        tr[x].siz = tr[tr[x].ls].siz
                  + tr[tr[x].rs].siz + 1;

        return x;
    }

    // 判断堆是否为空
    bool empty() const
    {
        return root == 0;
    }

    // 返回元素数量
    int size() const
    {
        return tr[root].siz;
    }

    // 返回优先级最高的元素，使用前要保证堆非空
    const T &top() const
    {
        return tr[root].val;
    }

    // 插入新元素
    void push(const T &val)
    {
        int now = newNode(val);

        root = mergeNode(root, now);
    }

    // 删除优先级最高的元素
    void pop()
    {
        if (empty())
        {
            return;
        }

        // 保存要删除的根节点
        int oldRoot = root;

        // 保存左右子树
        int leftRoot = tr[oldRoot].ls;
        int rightRoot = tr[oldRoot].rs;

        // 左右子树合并，成为新的根
        root = mergeNode(leftRoot, rightRoot);

        /*
        原来的根节点已经不属于当前堆。

        把它放进空闲节点池，
        后面push时可以直接重新使用。
        */
        freeNode.push_back(oldRoot);
    }

    /*
    把other合并进当前堆。

    合并后：
    当前堆包含两个堆的所有元素；
    other变成空堆。
    */
    void merge(MergeHeap &other)
    {
        root = mergeNode(root, other.root);

        other.root = 0;
    }

    /*
    清空当前堆。

    当前堆中的所有节点都会放入空闲节点池，
    之后可以被重新使用。

    复杂度为O(size())。
    */
    void clear()
    {
        if (empty())
        {
            return;
        }

        vector<int> stk;
        stk.push_back(root);

        while (!stk.empty())
        {
            int x = stk.back();
            stk.pop_back();

            if (tr[x].ls != 0)
            {
                stk.push_back(tr[x].ls);
            }

            if (tr[x].rs != 0)
            {
                stk.push_back(tr[x].rs);
            }

            freeNode.push_back(x);
        }

        root = 0;
    }

    /*
    如果大概知道最多同时存在多少个元素，
    可以提前申请空间，减少vector扩容次数。

    不是必须调用。
    */
    static void reserve(int n)
    {
        tr.reserve(n + 1);
        freeNode.reserve(n + 1);
    }

    /*
    返回节点池曾经创建过的节点数量。

    主要用于调试内存，不需要可以删除。
    */
    static int poolSize()
    {
        return tr.size() - 1;
    }

    /*
    返回当前空闲节点数量。

    主要用于调试内存，不需要可以删除。
    */
    static int freeSize()
    {
        return freeNode.size();
    }

    /*
    清空并释放整个公共节点池。

    调用以后，所有相同类型的合并堆都会失效。

    只能在确定之前的堆全部不再使用时调用，
    一般在每组测试数据全部处理结束后调用。
    */
    static void resetPool()
    {
        vector<Node> newTree(1);
        tr.swap(newTree);

        vector<int> newFreeNode;
        freeNode.swap(newFreeNode);
    }
};




MergeHeap<Node, greater<Node>>::resetPool();
