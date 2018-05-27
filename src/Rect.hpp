#ifndef Rect_hpp
#define Rect_hpp

#include "SDL.hpp"
#include <vector>
#include <utility>

namespace SDL
{
    using namespace std::rel_ops;

    struct Size
    {
        int w, h;
    };

    using Point = SDL_Point;
    using Rect = SDL_Rect;
    using Points = std::vector<Point>;
    using Rects = std::vector<Rect>;
    using Line = std::pair<Point, Point>;
    using Lines = std::vector<Line>;
    using Sizes = std::vector<Size>;

    inline bool operator!(Rect const &A)
    {
        return SDL_RectEmpty(&A);
    }

    inline bool operator<(Rect const &A, Rect const &B)
    {
        return A.w < B.w and A.h < B.h and A.x > B.x and A.y > B.y;
    }

    inline bool operator==(Rect const &A, Rect const &B)
    {
        return SDL_RectEquals(&A, &B);
    }

    inline bool operator&&(Rect const &A, Rect const &B)
    {
        return SDL_HasIntersection(&A, &B);
    }

    inline bool operator&&(Rect const &A, Point const &B)
    {
        return SDL_PointInRect(&B, &A);
    }

    inline bool operator&&(Rect const &A, Line B)
    {
        return SDL_IntersectRectAndLine(&A, &B.first.x, &B.first.y, &B.second.x, &B.second.y);
    }

    inline Line operator&(Rect const &A, Line B)
    {
        if (not SDL_IntersectRectAndLine(&A, &B.first.x, &B.first.y, &B.second.x, &B.second.y))
        {
            B.first.x = B.first.y = B.second.x = B.second.y = 0;
        }
        return B;
    }

    inline Rect operator&(Rect const &A, Rect const &B)
    {
        Rect C;
        if (not SDL_IntersectRect(&A, &B, &C))
        {
            C.x = C.y = C.w = C.h = 0;
        }
        return C;
    }

    inline Rect operator|(Rect const &A, Rect const &B)
    {
        Rect C;
        SDL_UnionRect(&A, &B, &C);
        return C;
    }
}

#endif // file
