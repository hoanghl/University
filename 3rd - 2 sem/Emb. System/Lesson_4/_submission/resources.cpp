#include    "resources.hpp"

#include    <iostream>
#include    <stdlib.h>     /* srand, rand */
#include    <time.h>       /* time */
#include    "oled/Edison_OLED.h"
#include    "gpio/gpio.h"

using namespace std;
// **************************** Data structures *************************
Node::Node(struct Point *point, Node *pNext) {
    this->_point = point;
    this->_pNext = pNext;
}

Node::~Node() {
    delete this->_point;
}



Queue::Queue(struct Point *head, struct Point *tail) {
    this->_tail = new Node(tail, NULL );
    this->_head = new Node(head, this->_tail);
    this->_size = 2;
}

Queue::~Queue() {
    Node *p = this->_tail;
    while (p != NULL) {
        Node *pTemp = p;
        p = p->_pNext;
        pTemp->~Node();
        delete pTemp;
    }
}

void    Queue::addHead(struct Point *point) {
    Node *p = new Node(point, this->_head);
    this->_head = p;
    this->_size++;
}

void    Queue::removeTail() {
    Node *p = this->getHead();
    for ( ; p->_pNext != this->_tail; p = p->_pNext);
        delete this->_tail;
    this->_tail = p;
    this->_tail->_pNext = NULL;
}

Node*   Queue::getTail() {
    return this->_tail;
}
Node*   Queue::getHead() {
    return this->_head;
}
unsigned char  Queue::getSize() {
    return this->_size;
}
void    Queue::setSize(unsigned char n) {
    this->_size = n;
}

// ****************************** Entities in game *******************************
// ************** SNAKE
Snake::Snake() {
    this->_length = LENGTH_BEGIN;
    this->_last_direction = LEFT;
    struct Point *head = new Point(); struct Point *tail = new Point();
    head->y = tail->y = 30;
    head->x = 10; tail->x = head->x + this->_length;
    
    this->_turn_points = new Queue(head, tail);
}

Snake::~Snake() {
    this->_turn_points->~Queue();
}

bool    Snake::isHitWall() {
    Node *head = this->_turn_points->getHead();
    //check reaching BOUND_UP or BOUND_DOWN
    if (head->_point->y <= BOUND_UP || head->_point->y >= BOUND_DOWN)
        return true;

    //check reaching BOUND_LEFT or BOUND_RIGHT
    if (head->_point->x <= BOUND_LEFT || head->_point->x >= BOUND_RIGHT)
        return true;

    return false;
}

struct Point*   Snake::getHead() {
    return this->_turn_points->getHead()->_point;
}
bool    Snake::isHitApple(struct Point *apple) {
    Node *head = this->_turn_points->getHead();

    if (head->_point->x == apple->x && head->_point->y == apple->y)
        return true;
    return false;
}


void    Snake::drawSnake() {
    Node* p_prev = this->_turn_points->getHead();
    Node* p      = p_prev->_pNext;

    for (; p != NULL; p_prev = p, p = p->_pNext)
        drawLine(p_prev->_point->x, p_prev->_point->y, p->_point->x, p->_point->y);
}


unsigned char   max(unsigned char a, unsigned char b) {
    return (a > b ? a : b);
}


unsigned char   min(unsigned char a, unsigned char b) {
    return (a < b ? a : b);
}

bool    Snake::isHitBody(struct Point *point) {
    Node* p_prev = this->_turn_points->getHead()->_pNext;
    Node* p      = p_prev->_pNext;

    cout << " --- isHitBody checking: ---\n";
    cout << "Coordinate of point: " << (int)point->x << " - " << (int)point->y << "\n";
    for (Node *pT = this->_turn_points->getHead(); pT != NULL; pT = pT->_pNext)
        cout << "Coordinate: " << (int)pT->_point->x << " - " << (int)pT->_point->y << "\n";
    

    for (; p != NULL; p_prev = p, p = p->_pNext) {
        if (p->_point->x == point->x && p_prev->_point->x == point->x)
            if (min(p->_point->y, p_prev->_point->y) <= point->y &&
                max(p->_point->y, p_prev->_point->y) >= point->y){
                cout << "block 1\n";
                return true;
            }

        if (p->_point->y == point->y && p_prev->_point->y == point->y)
            if (min(p->_point->x, p_prev->_point->x) <= point->x &&
                max(p->_point->x, p_prev->_point->x) >= point->x){
                cout << "block 1\n";
                return true;
            }
    }
    return false;
}

void    Snake::move_head(Direction analog, Direction lastDir){
    Point *head = this->getHead();
    switch (analog)
    {
        case LEFT:{            
            this->_turn_points->addHead(new Point(head->x - 1, head->y));
            break;
        }
        case RIGHT: {
            this->_turn_points->addHead(new Point(head->x + 1, head->y));
            break;
        }
        case UP: {
            this->_turn_points->addHead(new Point(head->x, head->y - 1));
            break;
        }
        case DOWN: {
            this->_turn_points->addHead(new Point(head->x , head->y + 1));
            break;
        }
        case NON: {
            switch (lastDir) {
                case LEFT:{            
                    this->getHead()->x--;
                    break;
                }
                case RIGHT: {
                    this->getHead()->x++;
                    break;
                }
                case UP: {
                    this->getHead()->y--;
                    break;
                }
                case DOWN: {
                    this->getHead()->y++;
                    break;
                }
                default:
                    break;
            }
            break;
        }
    }
}

int    distance(Node *p1, Node *p2) {
    return (p1->_point->y == p2->_point->y ? abs(p1->_point->x - p2->_point->x) : abs(p1->_point->y - p2->_point->y));
}
void    Snake::move(Direction direction) {
    // recalculate head postition of snake
    this->move_head(direction, this->_last_direction);
    if (direction != NON)
        this->_last_direction = direction;
    

    // recalculate other points and tail of snake
    int length = this->_length + 1;
    Node *p_prev = this->_turn_points->getHead();
    Node *p      = p_prev->_pNext;


    for ( ; p != NULL; p_prev = p, p = p->_pNext) {

        length -= distance(p, p_prev);
        cout << "distance = " << distance(p, p_prev) << " and length = " << length << "\n";
        if (length == 0) {
            if (p == this->_turn_points->getTail()) {
                if (p->_point->x == p_prev->_point->x)
                    p->_point->y = (p->_point->y > p_prev->_point->y) ? (p->_point->y - 1) : (p->_point->y + 1);
                else
                    p->_point->x = (p->_point->x > p_prev->_point->x) ? (p->_point->x - 1) : (p->_point->x + 1);
                return;

            }
            else
            {
                while (p != this->_turn_points->getTail())
                    this->_turn_points->removeTail();
                cout << " *** Remove tail just happened ***\n";
                return;
            }
        }
        if (length < 0) {
            while (p != this->_turn_points->getTail())
                this->_turn_points->removeTail();

            if (p->_point->x == p_prev->_point->x)
                p->_point->y = (p->_point->y < p_prev->_point->y) ? (p->_point->y - length) : (p->_point->y + length);
            else
                p->_point->x = (p->_point->x < p_prev->_point->x) ? (p->_point->x - length) : (p->_point->x + length);

            return;
        }
            
    }
}


Queue*      Snake::get_queue() {
    return this->_turn_points;
}
 // ************* APPLE
Apple::Apple() {
    this->_position = new Point(40, 40);
}

void            Apple::genNew() {
    this->_position->y = rand() % (BOUND_DOWN - BOUND_UP) + BOUND_UP;
    this->_position->x = rand() % (BOUND_RIGHT - BOUND_LEFT) + BOUND_LEFT;
}

struct Point*   Apple::get_position() {
    return this->_position;
}

void            Apple::drawApple() {
    oled.pixel(this->_position->x, this->_position->y);
}

