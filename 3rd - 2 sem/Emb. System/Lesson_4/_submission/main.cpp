#include    "resources.hpp"

#include    <stdlib.h>
#include    <time.h>
#include    <unistd.h>

using namespace std;

enum STATE {WIN, LOSE, PLAYING};
STATE state = PLAYING;

// entities in game
Snake   *snake = new Snake();
Apple   *apple = new Apple();

// general variables
int     score = 0;
edOLED  oled;
gpio    BUTTON_UP   (47, INPUT);
gpio    BUTTON_DOWN (44, INPUT);
gpio    BUTTON_LEFT(165, INPUT);
gpio    BUTTON_RIGHT(45, INPUT);


void    run();
void    draw();
 
int main(int argc, char * argv[]) {
    srand(time(NULL));
    setupOLED();
    draw();
    
    while(true){
        draw();
        usleep(1000/60);
        
        run();

        if (state == WIN) {
            prize("You won !");
            usleep(600000);
            return 0;
        } else if (state == LOSE)
        {
            prize("Loooser");
            usleep(600000);
            return 0;
        }        
    }
    return 0;
}

/*
 * in run():
 *      - read button and calculate snake position
 *      - check hit
 */
void    run() {
    cout << " ---- new run: " << "\n";
    snake->move(readButton());
    // cout << "Head: " << (int)q->getHead()->_point->x << "  -  " << (int)q->getHead()->_point->y << "\n";
    // cout << "Tail: " << (int)q->getTail()->_point->x << "  -  " << (int)q->getTail()->_point->y << "\n";
    // cout << "Apple: "<< (int)apple->get_position()->x <<"  -  " << (int)apple->get_position()->y << "\n";


    //check if hit
    if (snake->isHitWall()) {
        state = LOSE;
        return;
    }
    if (snake->isHitBody(snake->getHead())) {
        cout << "Yes, hit body \n";
        state = LOSE;
        return;
    }
    if (snake->isHitApple(apple->get_position())) {
        score += 5;

        if (score == WIN_SCORE){
            state = WIN;
            return;
        }

        do {
            apple->genNew();
        } while (snake->isHitBody(apple->get_position()));
    }

    cout << " ---- end run\n";
}

void    draw() {
    oled.clear(PAGE);

    printScore(score);
    oled.lineH(BOUND_LEFT, BOUND_UP, LCDWIDTH);
    
    
    snake->drawSnake();
    apple->drawApple();
    
    oled.display();
}


void        setupOLED() {
	oled.begin();
	oled.clear(PAGE);
	oled.display();
	oled.setFontType(0);
}

Direction   readButton() {
    if (BUTTON_DOWN.pinRead() == PRESSED)
        return DOWN;
    if (BUTTON_LEFT.pinRead() == PRESSED)
        return LEFT;
    if (BUTTON_RIGHT.pinRead() == PRESSED)
        return RIGHT;
    if (BUTTON_UP.pinRead() == PRESSED)
        return UP;
    return NON;
}

void        drawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
    oled.line(x1, y1, x2, y2);
}

void        printScore(int score) {
    oled.setCursor(5, 5);
	oled.print("Score: "); oled.print(score);
}

void        prize(const char * msg) {
    oled.clear(PAGE);
    oled.display();

    oled.setCursor(10, 25);
    oled.print(msg);
    oled.display();
}
