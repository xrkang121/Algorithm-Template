#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

#define int long long

/*
    OrderSet<T> st;

    要求：
    T 必须支持 operator<

    操作：

    st.insert(x);
        插入一个 x，允许重复

    st.erase(x);
        删除一个 x
        成功返回 true，不存在返回 false

    st.kth(k);
        返回第 k 小的元素
        k 从 0 开始
        kth(0) 是最小值
        使用前保证 0 <= k < st.size()

    st.rank(x);
        返回严格小于 x 的元素数量
        也就是 x 从 0 开始的排名

    st.lessEqualCount(x);
        返回小于等于 x 的元素数量

    st.count(x);
        返回 x 出现的次数

    st.exist(x);
        判断 x 是否存在

    st.size();
        返回元素数量

    st.empty();
        判断是否为空

    st.clear();
        清空所有元素
*/

template<class T>
struct OrderSet
{
    typedef pair<T, long long> Key;

    typedef tree<
        Key,
        null_type,
        less<Key>,
        rb_tree_tag,
        tree_order_statistics_node_update
    > Tree;

    Tree tr;
    long long id = 0;

    void insert(const T &val)
    {
        id++;

        tr.insert({val, id});
    }

    bool erase(const T &val)
    {
        auto it = tr.lower_bound({val, LLONG_MIN});

        if (it == tr.end() || val < it->first || it->first < val)
        {
            return false;
        }

        tr.erase(it);

        return true;
    }

    T kth(int k)
    {
        return tr.find_by_order(k)->first;
    }

    int rank(const T &val)
    {
        return tr.order_of_key({val, LLONG_MIN});
    }

    int lessEqualCount(const T &val)
    {
        return tr.order_of_key({val, LLONG_MAX});
    }

    int count(const T &val)
    {
        return lessEqualCount(val) - rank(val);
    }

    bool exist(const T &val)
    {
        return count(val) > 0;
    }

    int size()
    {
        return tr.size();
    }

    bool empty()
    {
        return tr.empty();
    }

    void clear()
    {
        tr.clear();
        id = 0;
    }
};

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    OrderSet<int> st;

    return 0;
}
