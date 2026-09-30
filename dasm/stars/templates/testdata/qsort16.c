#include <assert.h>
#include <string.h>

typedef struct { int16_t x, y; } Point;

// ComparePoints reproduces ICompLong's comparison of packed POINT16 values.
static int ComparePoints(const void *a, const void *b)
{
    return (int16_t)(((const Point *)a)->x - ((const Point *)b)->x);
}

// ComparePointers compares records through native-width array elements.
static int ComparePointers(const void *a, const void *b)
{
    return ComparePoints(*(Point *const *)a, *(Point *const *)b);
}

// CompareBytes compares the key in an odd-sized three-byte record.
static int CompareBytes(const void *a, const void *b)
{
    return *(const unsigned char *)a - *(const unsigned char *)b;
}

// main checks tie order traced from 0024:0866 and exhaustive small arrays.
int main(void)
{
    Point equal[] = {{1106, 100}, {1106, 200}, {1106, 300}};
    const Point equalWant[] = {{1106, 100}, {1106, 200}, {1106, 300}};
    qsort16(equal, 3, sizeof(*equal), ComparePoints);
    assert(memcmp(equal, equalWant, sizeof(equal)) == 0);

    Point points[] = {{3, 'A'}, {1, 'B'}, {2, 'C'}, {1, 'D'}, {3, 'E'}, {2, 'F'}};
    const Point want[] = {{1, 'D'}, {1, 'B'}, {2, 'C'}, {2, 'F'}, {3, 'E'}, {3, 'A'}};
    Point *pointers[] = {points, points + 1, points + 2, points + 3, points + 4, points + 5};
    qsort16(pointers, 6, sizeof(*pointers), ComparePointers);
    for (size_t i = 0; i < 6; ++i)
        assert(pointers[i]->x == want[i].x && pointers[i]->y == want[i].y);
    qsort16(points, 6, sizeof(*points), ComparePoints);
    assert(memcmp(points, want, sizeof(points)) == 0);

    unsigned char odd[][3] = {{3, 'A', 0xa5}, {1, 'B', 0xa5}, {2, 'C', 0xa5},
                             {1, 'D', 0xa5}, {3, 'E', 0xa5}, {2, 'F', 0xa5}};
    qsort16(odd, 6, sizeof(*odd), CompareBytes);
    for (size_t i = 0; i < 6; ++i)
        assert(odd[i][0] == want[i].x && odd[i][1] == want[i].y && odd[i][2] == 0xa5);

    for (unsigned pattern = 0; pattern < 65536; ++pattern) {
        Point values[8];
        for (size_t i = 0; i < 8; ++i) {
            values[i].x = ((pattern >> (2 * i)) & 3) - 2;
            values[i].y = i;
        }
        qsort16(values, 8, sizeof(*values), ComparePoints);
        unsigned seen = 0;
        for (size_t i = 0; i < 8; ++i) {
            assert(values[i].y >= 0 && values[i].y < 8);
            assert(values[i].x == (int)((pattern >> (2 * values[i].y)) & 3) - 2);
            seen |= 1u << values[i].y;
            if (i != 0)
                assert(values[i - 1].x <= values[i].x);
        }
        assert(seen == 255);
    }
    return 0;
}
