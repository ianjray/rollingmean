#include "fcmp.hpp"
#include "rollingmean.hpp"

#include <cassert>
#include <cstdio>

int main()
{
    {
        RollingMean r(3);
        assert(!r.ready());
        assert(fcmp(r.sum(), 0));
        assert(fcmp(r.mean(), 0));

        // { 4,         }  { 4,      }
        // { 4, 2,      }  { 2, 4,   }
        // { 4, 2, 3,   }  { 2, 3, 4 }
        // { 4, 2, 3, 1 }  { 1, 2, 3 }

        double v;
        v = r.add(4);
        assert(!r.ready());
        assert(fcmp(v, 4));
        assert(fcmp(r.sum(), 0));
        assert(fcmp(r.mean(), 0));
        v = r.add(2);
        assert(!r.ready());
        assert(fcmp(v, 2));
        assert(fcmp(r.sum(), 0));
        assert(fcmp(r.mean(), 0));
        v = r.add(3);
        assert(r.ready());
        assert(fcmp(v, r.mean()));
        assert(fcmp(r.sum(), 3));
        assert(fcmp(r.mean(), 3));
        v = r.add(1);
        assert(r.ready());
        assert(fcmp(v, r.mean()));
        assert(fcmp(r.sum(), 2));
        assert(fcmp(r.mean(), 2));

        r.clear();
        assert(!r.ready());
        assert(fcmp(r.sum(), 0));
        assert(fcmp(r.mean(), 0));

        r.add(4);
        r.add(2);
        r.add(3);
        r.add(1);
        assert(fcmp(r.sum(), 2));
        assert(fcmp(r.mean(), 2));
    }

    {
        RollingMean r(8);

        // { 9, 8, 5,               }  { 5, 8, 9,               }  sum  8 mean  8
        // { 9, 8, 5, 6,            }  { 5, 6, 8, 9,            }  sum 14 mean  7
        // { 9, 8, 5, 6, 7,         }  { 5, 6, 7, 8, 9,         }  sum 21 mean  7
        // { 9, 8, 5, 6, 7, 4       }  { 4, 5, 6, 7, 8, 9,      }  sum 26 mean  6.5
        // { 9, 8, 5, 6, 7, 4, 2,   }  { 2, 4, 5, 6, 7, 8, 9,   }  sum 30 mean  6
        // { 9, 8, 5, 6, 7, 4, 2, 1 }  { 1, 2, 4, 5, 6, 7, 8, 9 }  sum 32 mean  5.333
        // { 8, 5, 6, 7, 4, 2, 1, 3 }  { 1, 2, 3, 4, 5, 6, 7, 8 }  sum 27 mean  4.5
        // { 5, 6, 7, 4, 2, 1, 3, 4 }  { 1, 2, 3, 4, 4, 5, 6, 7 }  sum 24 mean  4
        // { 6, 7, 4, 2, 1, 3, 4, 6 }  { 1, 2, 3, 4, 4, 6, 6, 7 }  sum 25 mean  4.166
        // { 7, 4, 2, 1, 3, 4, 6, 5 }  { 1, 2, 3, 4, 4, 5, 6, 7 }  sum 24 mean  4
        // { 4, 2, 1, 3, 4, 6, 5, 2 }  { 1, 2, 2, 3, 4, 4, 5, 6 }  sum 20 mean  3.333

        double v;
        v = r.add(9);
        assert(fcmp(v, 9));
        assert(fcmp(r.sum(), 0));
        assert(fcmp(r.mean(), 0));
        v = r.add(8);
        assert(fcmp(v, 8));
        assert(fcmp(r.sum(), 0));
        assert(fcmp(r.mean(), 0));
        v = r.add(5);
        assert(fcmp(v, r.mean()));
        assert(fcmp(r.sum(), 8));
        assert(fcmp(r.mean(), 8));
        v = r.add(6);
        assert(fcmp(v, r.mean()));
        assert(fcmp(r.sum(), 14));
        assert(fcmp(r.mean(), 7));
        v = r.add(7);
        assert(fcmp(v, r.mean()));
        assert(fcmp(r.sum(), 21));
        assert(fcmp(r.mean(), 7));
        v = r.add(4);
        assert(fcmp(v, r.mean()));
        assert(fcmp(r.sum(), 26));
        assert(fcmp(r.mean(), 6.5));
        v = r.add(2);
        assert(fcmp(v, r.mean()));
        assert(fcmp(r.sum(), 30));
        assert(fcmp(r.mean(), 6));
        v = r.add(1);
        assert(fcmp(v, r.mean()));
        assert(fcmp(r.sum(), 32));
        assert(fcmp(r.mean(), 5.333333));
        v = r.add(3);
        assert(fcmp(v, r.mean()));
        assert(fcmp(r.sum(), 27));
        assert(fcmp(r.mean(), 4.5));
        v = r.add(4);
        assert(fcmp(v, r.mean()));
        assert(fcmp(r.sum(), 24));
        assert(fcmp(r.mean(), 4));
        v = r.add(6);
        assert(fcmp(v, r.mean()));
        assert(fcmp(r.sum(), 25));
        assert(fcmp(r.mean(), 4.166667));
        v = r.add(5);
        assert(fcmp(v, r.mean()));
        assert(fcmp(r.sum(), 24));
        assert(fcmp(r.mean(), 4));
        v = r.add(2);
        assert(fcmp(v, r.mean()));
        assert(fcmp(r.sum(), 20));
        assert(fcmp(r.mean(), 3.333333));
    }

    printf("PASS %s\n", __FILE__);
}
