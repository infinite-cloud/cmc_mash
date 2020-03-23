#ifndef BVH_H
#define BVH_H

#include <algorithm>
#include <deque>
#include <functional>
#include <memory>
#include <vector>
#include <limits>

#include <glm/vec3.hpp>

#include "ray.h"

class BoundingBox
{
    glm::vec3 _min;
    glm::vec3 _max;

public:
    BoundingBox() : _min(glm::vec3(0)), _max(glm::vec3(0)) {}
    BoundingBox(const glm::vec3 &min, const glm::vec3 &max) :
        _min(min), _max(max) {}

    const glm::vec3 &min() const { return _min; }
    const glm::vec3 &max() const { return _max; }

    glm::vec3 size() const { return _max - _min; }
    glm::vec3 center() const { return 0.5f * (_max + _min); }

    std::array<glm::vec3, 8> corners() const
    {
        return std::array<glm::vec3, 8>
        {
            _min,
            glm::vec3(_max.x, _min.y, _min.z),
            glm::vec3(_min.x, _max.y, _min.z),
            glm::vec3(_max.x, _max.y, _min.z),
            glm::vec3(_min.x, _min.y, _max.z),
            glm::vec3(_max.x, _min.y, _max.z),
            glm::vec3(_min.x, _max.y, _max.z),
            _max
        };
    }

    bool intersects(const Ray &r) const
    {
        float t_min = (_min.x - r.origin().x) * (1.0d / r.direction()).x;
        float t_max = (_max.x - r.origin().x) * (1.0d / r.direction()).x;

        if (t_min > t_max)
        {
            std::swap(t_min, t_max);
        }

        float t_1 = (_min.y - r.origin().y) * (1.0d / r.direction()).y;
        float t_2 = (_max.y - r.origin().y) * (1.0d / r.direction()).y;

        if (t_1 > t_2)
        {
            std::swap(t_1, t_2);
        }

        if (t_1 > t_max || t_2 < t_min)
        {
            return false;
        }

        t_min = std::max(t_min, t_1);


        t_1 = (_min.z - r.origin().z) * (1.0d / r.direction()).z;
        t_2 = (_max.z - r.origin().z) * (1.0d / r.direction()).z;

        if (t_1 > t_2)
        {
            std::swap(t_1, t_2);
        }

        if (t_1 > t_max || t_2 < t_min)
        {
            return false;
        }

        return t_max > 0;
    }

    static BoundingBox combine(const BoundingBox &a, const BoundingBox &b)
    {
        return BoundingBox
        (
            glm::vec3
            (
                std::min(a.min().x, b.min().x),
                std::min(a.min().y, b.min().y),
                std::min(a.min().z, b.min().z)
            ),
            glm::vec3
            (
                std::max(a.max().x, b.max().x),
                std::max(a.max().y, b.max().y),
                std::max(a.max().z, b.max().z)
            )
        );
    }
};

inline uint64_t morton(uint32_t x, uint32_t y, uint32_t z)
{
    auto split_by_3 =
        [](uint32_t a)
        {
            uint64_t val = a >> 11;

            val = (val | val << 32) & 0x1f00000000ffff;
            val = (val | val << 16) & 0x1f0000ff0000ff;
            val = (val | val << 8) & 0x100f00f00f00f00f;
            val = (val | val << 4) & 0x10c30c30c30c30c3;
            val = (val | val << 2) & 0x1249249249249249;

            return val;
        };

    return split_by_3(x) | (split_by_3(y) << 1) | (split_by_3(z) << 2);
}

inline uint64_t morton(const glm::vec3& pos)
{
    return morton
    (
        static_cast<uint32_t>(pos.x * std::numeric_limits<uint32_t>::max()),
        static_cast<uint32_t>(pos.y * std::numeric_limits<uint32_t>::max()),
        static_cast<uint32_t>(pos.z * std::numeric_limits<uint32_t>::max())
    );
}

inline uint64_t morton(const BoundingBox &b, const BoundingBox &total)
{
    auto v = (b.center() - total.min()) / (total.max() - total.min());

    v.x = (total.min().x == total.max().x) ? 0 : v.x;
    v.y = (total.min().y == total.max().y) ? 0 : v.y;
    v.z = (total.min().z == total.max().z) ? 0 : v.z;

    return morton(v);
}

template <class T>
class BVH
{
public:
    struct Node
    {
        BoundingBox box;

        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        T *object = nullptr;
        
        template <typename TFn>
        void search(const Ray &r, const TFn &fn) const
        {
            if (box.intersects(r))
            {
                if (object)
                {
                    fn(*object);
                }

                if (left)
                {
                    left->search(r, fn);
                }

                if (right)
                {
                    right->search(r, fn);
                }
            }
        }
    };

private:
    std::unique_ptr<Node> _root;

public:
    BVH() : _root(nullptr) {}
    BVH(std::unique_ptr<Node> root) : _root(std::move(root)) {}

    const std::unique_ptr<Node> &root() const { return _root; }

    template <typename TFn>
    void search(const Ray &r, const TFn &fn) const
    {
        if (_root)
        {
            _root->search(r, fn);
        }
    }

    template <typename TAABBFn>
    static BVH<T> construct(const std::vector<T *> &objects,
            size_t delta, const TAABBFn &aabb)
    {
        if (objects.empty())
        {
            return BVH<T>();
        }

        std::vector<std::tuple<T *, BoundingBox, uint64_t>> objects_sorted;

        auto box = BoundingBox
        (
            glm::vec3(std::numeric_limits<float>::infinity()),
            glm::vec3(-std::numeric_limits<float>::infinity())
        );

        for (T *o : objects)
        {
            auto o_box = aabb(*o);

            objects_sorted.emplace_back(o, o_box, 0);
            box = BoundingBox::combine(box, o_box);
        }

        for (auto &o : objects_sorted)
        {
            std::get<2>(o) = morton(std::get<1>(o), box);
        }

        std::sort(objects_sorted.begin(), objects_sorted.end(),
            [](const auto &t1, const auto &t2)
            {
                return std::get<2>(t1) < std::get<2>(t2);
            }
        );

        return BVH<T>
        (
            construct_build_tree
            (
                objects_sorted,
                0,
                objects_sorted.size(),
                delta,
                62
            )
        );
    }

private:
    static std::unique_ptr<Node> construct_build_tree(
        const std::vector<std::tuple<T *, BoundingBox, uint64_t>> &objects,
        size_t start, size_t end, size_t delta, int bit)
    {
        if (start == end)
        {
            return nullptr;
        }

        if ((end - start) <= delta || bit < 0)
        {
            return construct_combine_primitives(objects, start, end);
        }
        else
        {
            size_t part = construct_make_partition(objects, start, end, bit);

            if (part == start || part == end)
            {
                return construct_build_tree(objects, start, end,
                    delta, bit - 1);
            }
            else
            {
                auto n = std::make_unique<Node>();

                n->left = construct_build_tree(objects, start, part,
                    delta, bit - 1);
                n->right = construct_build_tree(objects, part, end,
                    delta, bit - 1);

                n->box = BoundingBox::combine(n->left->box, n->right->box);

                return n;
            }
        }
    }

    static size_t construct_make_partition(
        const std::vector<std::tuple<T*, BoundingBox, uint64_t>>& objects,
        size_t start, size_t end, int bit)
    {
        uint64_t bitmask = 1 << bit;

        while (start < end)
        {
            size_t mid = start + (end - start) / 2;

            if ((std::get<2>(objects[start]) & bitmask) == 0)
            {
                start = mid + 1;
            }
            else
            {
                end = mid;
            }
        }

        return start;
    }

    static std::unique_ptr<Node> construct_combine_primitives(
        const std::vector<std::tuple<T *, BoundingBox, uint64_t>> &objects,
        size_t start, size_t end)
    {
        std::vector<std::unique_ptr<Node>> clusters;

        for (size_t i = start; i < end; ++i)
        {
            const auto &o = objects[i];
            auto new_cluster = std::make_unique<Node>();

            new_cluster->box = std::get<1>(o);
            new_cluster->object = std::get<0>(o);

            clusters.push_back(std::move(new_cluster));
        }

        while (clusters.size() > 1)
        {
            float best = std::numeric_limits<float>::infinity();
            size_t i_best = 0;
            size_t j_best = 0;

            for (size_t i = 0; i < clusters.size(); ++i)
            {
                for (size_t j = i + 1; j < clusters.size(); j++) {
                    float dist = glm::distance(clusters[i]->box.center(),
                        clusters[j]->box.center());

                    if (dist < best)
                    {
                        best = dist;
                        i_best = i;
                        j_best = j;
                    }
                }
            }

            auto new_cluster = std::make_unique<Node>();

            new_cluster->box = BoundingBox::combine(clusters[i_best]->box,
                clusters[j_best]->box);
            new_cluster->left = std::move(clusters[i_best]);
            new_cluster->right = std::move(clusters[j_best]);

            clusters.push_back(std::move(new_cluster));

            clusters.erase(clusters.begin() + j_best);
            clusters.erase(clusters.begin() + i_best);
        }

        return std::move(clusters[0]);
    }
};

#endif // BVH_H
