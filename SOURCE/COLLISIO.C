#ifndef _M_COLL_
#define _M_COLL_

#define pointInsideRect(x, y, rx, ry, rw, rh) \
    ((x) >= (rx) && (x) <= ((rx) + (rw)) && (y) >= (ry) && (y) <= ((ry) + (rh)))

#define rectanglesCollide(x1, y1, w1, h1, x2, y2, w2, h2) \
    (pointInsideRect((x1), (y1), (x2), (y2), (w2), (h2)) || \
     pointInsideRect((x1) + (w1), (y1), (x2), (y2), (w2), (h2)) || \
     pointInsideRect((x1), (y1) + (h1), (x2), (y2), (w2), (h2)) || \
     pointInsideRect((x1) + (w1), (y1) + (h1), (x2), (y2), (w2), (h2)))

#define boxesCollide(x1, y1, w1, h1, x2, y2, w2, h2) \
    (rectanglesCollide((x1), (y1), (w1), (h1), (x2), (y2), (w2), (h2)) || \
     rectanglesCollide((x2), (y2), (w2), (h2), (x1), (y1), (w1), (h1)))

#endif  // _M_COLL_
