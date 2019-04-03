#ifndef RESOURCES_HPP_
#define RESOURCES_HPP_

#include    "Draw.hpp"

#define WIN_SCORE       30      // score to win
#define TEXT_H          15      // score is displayed in the area is 10 pixels high and LCDWIDTH pixels wide
#define LENGTH_BEGIN    25

// boundaries of playing area
#define BOUND_LEFT  0
#define BOUND_UP    TEXT_H
#define BOUND_RIGHT LCDWIDTH
#define BOUND_DOWN  LCDHEIGHT


// **************************** General resources *************************

struct Point {
    unsigned char  x, y;

    Point() {
        this->x = this->y = 0;
    }
    Point(unsigned char x_, unsigned char y_) {
        this->x = x_; this->y = y_;
    }
};



// ***************************** Data structures **************************

class Node {
public:
    struct Point    *_point;
    Node            *_pNext;

    Node(struct Point *point, Node *pNext);
   ~Node(); 

};


class Queue {
private:
    Node                *_head, *_tail;
    unsigned char  _size;


public:
    Queue(struct Point *head, struct Point *tail);
   ~Queue();

    Node*           getTail();
    Node*           getHead();
    void            removeTail();
    void            addHead(struct Point *point);
    unsigned char   getSize();
    void            setSize(unsigned char n);
    
};


// ***************************** Entities in game ***************************
class Snake {
private:
    unsigned int     _length;
    Queue           *_turn_points;
    Direction        _last_direction;

public:
    Snake();
   ~Snake();
 
    struct Point*   getHead();
    bool            isHitWall();                        // check if the snake's head hits the wall
    bool            isHitBody(struct Point *point);     // check if the given point is belong to the body of the snake: 
                                                        // used to check head hits body and apple generated is on snake's body
    bool            isHitApple(struct Point *apple);
    void            move(Direction direction);
    void            drawSnake();

    Queue*          get_queue(); 

private:
    void            move_head(Direction analog, Direction lastDir);
};

class Apple {
private:
    struct Point    *_position;

public:
                    Apple();
    void            genNew();
    struct Point*   get_position();
    void            drawApple();
};

#endif
