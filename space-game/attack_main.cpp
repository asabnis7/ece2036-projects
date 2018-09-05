// main.cpp -----------------------------------------------

#include "mbed.h"
#include "uLCD_4DGL.h"
#include "PinDetect.h"
#include "Speaker.h"
#include "attack.h"

#include <cstdlib>
#include <ctime>

// set up global objects ----------------------------------
PinDetect pbLeft(p15);  // move left
PinDetect pbFire(p16); // fire
PinDetect pbRight(p17); // move right

uLCD_4DGL LCD(p28, p27, p29); // tx, rx, reset

Speaker sfx(p21); // set up speaker

Timer t; // game timer

Spaceship s1;
Bullet b1;
// --------------------------------------------------------


// pushbutton interrupt functions -------------------------
void pbLeft_hit_callback(void)
{
    s1.moveLeft();
}

void pbRight_hit_callback(void)
{
    s1.moveRight();
}

void pbFire_hit_callback(void)
{
    b1.setBullet(true);
}
// --------------------------------------------------------


// main function ------------------------------------------
int main()
{
    // seed random number generator
    std::srand(static_cast<unsigned int>(time(0)));

    // LCD setup
    LCD.display_control(LANDSCAPE_R);
    LCD.cls();
    LCD.baudrate(3000000);
    LCD.background_color(BLACK);
    
    // intro message
    LCD.printf("Invaders inbound!\nDefeat them and\n protect Earth!");
    for (int i = 0; i < 3; i++)
    {
        sfx.PlayNote(200+(i*50), 0.05, 0.1);
        sfx.PlayNote(250+(i*40), 0.05, 0.1);
    }
    wait(2.0);

    // set pushbuttons to active low
    pbLeft.mode(PullUp);
    pbRight.mode(PullUp);
    pbFire.mode(PullUp);

    // set pushbutton interrupts
    pbLeft.attach_deasserted(&pbLeft_hit_callback);
    pbRight.attach_deasserted(&pbRight_hit_callback);
    pbFire.attach_deasserted(&pbFire_hit_callback);
    
    pbLeft.setSampleFrequency();
    pbRight.setSampleFrequency();
    pbFire.setSampleFrequency();

    // delay for initial pullup to take effect
    wait(.01);

    LCD.cls();
    
    ScreenAliens *aliens[6];

    aliens[0] = new AlienBob(1);
    aliens[1] = new AlienDan(2);
    aliens[2] = new AlienJim(3);
    aliens[3] = new AlienDan(4);
    aliens[4] = new AlienJim(5);
    aliens[5] = new AlienAlice(6);

    for (int i = 0; i < 6; i++)
    {
        aliens[i]->draw(LCD);
    }

    s1.draw(LCD);
    
    t.start(); // start timing

    bool gameOver = false;
    
    int alienNum = 6;
    bool isHit[6] = {false};
    
    while (gameOver == false)
    {
        if (b1.getBullet() == true)
        {
            b1.fireBullet(s1,LCD);
            sfx.PlayNote(500, 0.05, 0.1);
            sfx.PlayNote(300, 0.05, 0.1);
        }
        
        while (b1.getBullet() == true)
        {
            s1.draw(LCD);
            b1.moveBullet(LCD);
            
            for (int i = 0; i < 6; i++)
            {
                if (alienNum < 1)
                {
                    gameOver = true;
                    break;
                }
                
                if (isHit[i] == true) continue;
                       
                isHit[i] = aliens[i]->hitTest(b1);
                
                if (isHit[i] == true)
                {
                    --alienNum;
                    aliens[i]->blackOut(LCD);
                    sfx.PlayNote(200, 0.1, 0.1);
                    sfx.PlayNote(350, 0.1, 0.1);
                    
                    LCD.filled_circle(b1.getxBullet(), b1.getyBullet(), 8, BLACK);
                    b1.setBullet(false);
                }
                else
                    aliens[i]->update(LCD);
            }
            wait(0.01);
        }
        
        while (b1.getBullet() == false)
        {
            if (alienNum < 1)
            {
                gameOver = true;
                break;
            }
            
            s1.draw(LCD);
            for (int i = 0; i < 6; i++)
            {
                if (isHit[i] == true) continue;
                aliens[i]->update(LCD);
            }
            wait(0.01);
        }
    }
    
    delete[] aliens;
    t.stop();
    float total = t.read();
    
    sfx.PlayNote(800, 0.1, 0.1);
    sfx.PlayNote(600, 0.1, 0.1);
    sfx.PlayNote(400, 0.1, 0.1);
    sfx.PlayNote(600, 0.1, 0.1);
    sfx.PlayNote(800, 0.1, 0.1);
    
    LCD.cls();
    LCD.printf("Congratulations!\nEarth is saved!\n");
    LCD.printf("Time: %f \nseconds",total);
    wait(4.0);
    
    exit(0);
}