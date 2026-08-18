/*
随机哈希板子（比赛可直接复制）

适用场景：

1. 给每个整数编号 x 分配一个几乎不会重复的随机值。
2. 判断两个集合或多重集合是否相同。
3. 把复杂状态、树、图、区间中的元素压缩成哈希值。
4. 防止 unordered_map / unordered_set 被特殊数据卡成 O(n^2)。

核心特点：

1. 使用 splitmix64，比 rand() 随机得多。
2. 使用两份独立的 uint64_t，碰撞概率极低。
3. 相同的 x 在同一次程序运行中一定得到相同哈希值。
4. getHash(x) 不需要 unordered_map，可以直接计算。

最重要的注意事项：

1. 哈希永远存在理论碰撞，只能让碰撞概率极低，不能做到数学上绝对正确。
2. 必须使用 uint64_t 或 unsigned long long。
   不要把哈希值改成 long long，因为有符号整数溢出属于未定义行为。
3. uint64_t 的加减乘法会自动对 2^64 取模，这是正常行为。
4. 多重集合哈希要使用加法：同一个元素出现几次，就加几次。
5. 不要用异或维护一般多重集合，因为 x ^ x = 0，出现两次会被消掉。
6. getHash(x) 只适用于：相同 x 必须得到相同哈希值的情况。
7. 如果 x 是字符串、结构体等类型，可以先离散化成整数，再调用 getHash(id)。
8. 不要使用 srand(time(0)) + rand()，随机范围小，而且低位质量较差。
*/

#include <bits/stdc++.h>
using namespace std;

#define int long long

using u64 = uint64_t;

// 64 位高质量混合函数
u64 splitmix64(u64 x)
{
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

// 每次程序运行时使用不同种子
mt19937_64 rnd(
    chrono::steady_clock::now().time_since_epoch().count());

u64 seed1 = rnd();
u64 seed2 = rnd();

// 双 64 位哈希值
typedef struct Hash
{
    u64 first;
    u64 second;

    Hash(u64 _first = 0, u64 _second = 0)
    {
        first = _first;
        second = _second;
    }

    bool operator==(const Hash &other) const
    {
        return first == other.first &&
               second == other.second;
    }

    bool operator!=(const Hash &other) const
    {
        return !(*this == other);
    }

    Hash operator+(const Hash &other) const
    {
        return Hash(first + other.first,
                    second + other.second);
    }

    Hash operator-(const Hash &other) const
    {
        return Hash(first - other.first,
                    second - other.second);
    }

    // 两个哈希值分别相乘
    Hash operator*(const Hash &other) const
    {
        return Hash(first * other.first,
                    second * other.second);
    }

    Hash &operator+=(const Hash &other)
    {
        first += other.first;
        second += other.second;
        return *this;
    }

    Hash &operator-=(const Hash &other)
    {
        first -= other.first;
        second -= other.second;
        return *this;
    }
} Hash;

// 相同的整数 x 一定得到相同哈希值
Hash getHash(int x)
{
    u64 now = (u64)x;

    return Hash(
        splitmix64(now + seed1),
        splitmix64(now + seed2));
}

// 生成一个全新的双随机值
// 注意：每调用一次，结果一般都不同
Hash getRandomHash()
{
    return Hash(rnd(), rnd());
}

// 防 unordered_map / unordered_set 被特殊数据卡哈希
struct SafeHash
{
    static const u64 FIXED_RANDOM;

    size_t operator()(int x) const
    {
        return splitmix64((u64)x + FIXED_RANDOM);
    }

    size_t operator()(const pair<int, int> &x) const
    {
        u64 left = splitmix64((u64)x.first + FIXED_RANDOM);
        u64 right = splitmix64((u64)x.second + FIXED_RANDOM + 1);

        return splitmix64(left ^ right);
    }
};

const u64 SafeHash::FIXED_RANDOM =
    chrono::steady_clock::now().time_since_epoch().count();

/*
============================================================
用法一：判断两个多重集合是否相同
============================================================

多重集合 A 和 B 中，每出现一个 x，就加上 getHash(x)。

Hash hash_a;
Hash hash_b;

for (int x : A)
{
    hash_a += getHash(x);
}

for (int x : B)
{
    hash_b += getHash(x);
}

if (hash_a == hash_b)
{
    // 两个多重集合几乎一定相同
}
else
{
    // 两个多重集合一定不同
}

说明：

如果 A = {1, 1, 3}，那么：

hash_a = getHash(1) + getHash(1) + getHash(3)

所以出现次数也会被记录。
*/

/*
============================================================
用法二：插入和删除元素
============================================================

Hash now;

// 插入 x
now += getHash(x);

// 删除 x
now -= getHash(x);

加法和减法必须成对使用。
只有集合中确实存在 x 时，才能执行删除。
*/

/*
============================================================
用法三：区间多重集合哈希
============================================================

vector<Hash> pre(n + 1);

for (int i = 1; i <= n; i++)
{
    pre[i] = pre[i - 1] + getHash(arr[i]);
}

// arr[l...r] 的多重集合哈希
Hash range_hash = pre[r] - pre[l - 1];

区间顺序不会影响结果：

{1, 2, 3} 和 {3, 1, 2} 的多重集合哈希相同。

如果需要区分顺序，就不能直接用这个加法哈希，
应该使用字符串多项式哈希、Z 函数、后缀数组等方法。
*/

/*
============================================================
用法四：给不同类别设置随机权值，并让权值总和为 0
============================================================

这个就是“每个编号在 k 种类别中出现次数是否相同”类题目的用法。

vector<Hash> weight(k);
Hash sum;

for (int i = 0; i < k - 1; i++)
{
    weight[i] = getRandomHash();
    sum += weight[i];
}

// uint64_t 的 0 - sum 会自动对 2^64 取模
weight[k - 1] = Hash() - sum;

// 此时一定有：
// weight[0] + weight[1] + ... + weight[k - 1] = Hash(0, 0)

位置 i 的贡献：

Hash value = getHash(id[i]) * weight[type[i]];

区间内所有 value 相加。
如果结果等于 Hash(0, 0)，则区间几乎一定满足均衡条件。

特别地，当 k = 1 时：

weight[0] = Hash(0, 0)

任意区间都应该被判定为均衡，这是正确的。
*/

/*
============================================================
用法五：防卡 unordered_map / unordered_set
============================================================

unordered_map<int, int, SafeHash> mp;
unordered_set<int, SafeHash> st;

unordered_map<pair<int, int>, int, SafeHash> pair_mp;

注意：

SafeHash 只是保护 unordered_map 自己的桶分布，
它和前面的“集合随机哈希 Hash”是两种不同用途。
*/

/*
============================================================
什么时候不能使用这个板子
============================================================

1. 题目要求百分之百确定、完全不能接受随机算法时。
2. 需要判断字符串或数组的排列顺序时。
3. 需要从哈希值还原原集合时。
4. 对手可以根据你已经输出的哈希结果，自适应构造后续输入时。

比赛中普通的离线输入、集合判等、树哈希、状态压缩等场景，
双 64 位随机哈希通常已经足够安全。
*/

/*
最小测试 main：

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    Hash a = getHash(10) + getHash(20) + getHash(10);
    Hash b = getHash(20) + getHash(10) + getHash(10);
    Hash c = getHash(10) + getHash(20);

    cout << (a == b) << "\n"; // 1
    cout << (a == c) << "\n"; // 0

    return 0;
}
*/
