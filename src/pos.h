#ifndef POS_H_INCLUDED
#define POS_H_INCLUDED

namespace as {

struct OASEX pos {
    typedef long coord_type;

    coord_type x, y;

    inline bool operator==(const pos& other) const {
        return x == other.x && y == other.y;
    }
};

}

namespace std {

template <>
struct OASEX hash<as::pos> {
    std::size_t operator()(const as::pos& p) const {
        using std::hash;
        std::size_t ret = 17;
        ret = ret * 23 + p.x;
        ret = ret * 23 + p.y;
        return ret;
    }
};

}

#endif // POS_H_INCLUDED
