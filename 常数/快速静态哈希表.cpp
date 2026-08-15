// 快速静态哈希表
// 只支持整数类型的 key
// 必须定义成全局变量

using i32 = int32_t;
using u64 = uint64_t;

template<
    class Key,
    class Value,
    size_t MAX_SIZE = 2000005,
    size_t HASH_SIZE = 4000037
>
struct FastMap
{
    Key keys[MAX_SIZE];
    Value values[MAX_SIZE];

    i32 nxt[MAX_SIZE];

    i32 head[HASH_SIZE];
    i32 vis[HASH_SIZE];

    i32 top;
    i32 free_head;
    i32 cnt;
    i32 timer;

    u64 seed;

    FastMap()
    {
        top = 0;
        free_head = 0;
        cnt = 0;
        timer = 1;

        seed = chrono::steady_clock::now()
                   .time_since_epoch()
                   .count();
    }

    u64 getRandom(u64 x)
    {
        x += seed + 0x9e3779b97f4a7c15ULL;

        x = (x ^ (x >> 30)) *
            0xbf58476d1ce4e5b9ULL;

        x = (x ^ (x >> 27)) *
            0x94d049bb133111ebULL;

        x ^= x >> 31;

        return x;
    }

    i32 getHash(const Key &x)
    {
        return getRandom((u64)x) % HASH_SIZE;
    }

    void initBucket(i32 pos)
    {
        if (vis[pos] != timer)
        {
            vis[pos] = timer;
            head[pos] = 0;
        }
    }

    i32 findId(const Key &x)
    {
        i32 pos = getHash(x);

        if (vis[pos] != timer)
        {
            return 0;
        }

        for (i32 id = head[pos];
             id != 0;
             id = nxt[id])
        {
            if (keys[id] == x)
            {
                return id;
            }
        }

        return 0;
    }

    // 判断 key 是否存在
    bool count(const Key &x)
    {
        return findId(x) != 0;
    }

    // 像 map 一样使用 X[key]
    Value &operator[](const Key &x)
    {
        i32 id = findId(x);

        if (id != 0)
        {
            return values[id];
        }

        i32 pos = getHash(x);

        initBucket(pos);

        if (free_head != 0)
        {
            id = free_head;
            free_head = nxt[free_head];
        }
        else
        {
            top++;
            id = top;

            assert(top < (i32)MAX_SIZE);
        }

        keys[id] = x;
        values[id] = Value();

        nxt[id] = head[pos];
        head[pos] = id;

        cnt++;

        return values[id];
    }

    // 删除 key
    bool erase(const Key &x)
    {
        i32 pos = getHash(x);

        if (vis[pos] != timer)
        {
            return false;
        }

        i32 last_id = 0;
        i32 id = head[pos];

        while (id != 0 && keys[id] != x)
        {
            last_id = id;
            id = nxt[id];
        }

        if (id == 0)
        {
            return false;
        }

        if (last_id == 0)
        {
            head[pos] = nxt[id];
        }
        else
        {
            nxt[last_id] = nxt[id];
        }

        // 回收被删除的节点
        nxt[id] = free_head;
        free_head = id;

        cnt--;

        return true;
    }

    // 清空，时间复杂度 O(1)
    void clear()
    {
        timer++;

        if (timer >= 2000000000)
        {
            memset(vis, 0, sizeof(vis));
            timer = 1;
        }

        top = 0;
        free_head = 0;
        cnt = 0;
    }

    int size()
    {
        return cnt;
    }

    bool empty()
    {
        return cnt == 0;
    }
};
