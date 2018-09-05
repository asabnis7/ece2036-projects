// Header file for attack.cpp -----------------------------


// class Spaceship ----------------------------------------
class Spaceship
{
private:
    int shipxPos, shipyPos;
public:
    Spaceship();
    void draw(uLCD_4DGL&);
    void moveRight();
    void moveLeft();
    int getxPos();
    int getyPos();
}; // end class Spaceship ---------------------------------


// class Bullet -------------------------------------------
class Bullet
{
private:
    int bulletxPos, bulletyPos;
    bool oneBullet;
public:
    Bullet();
    void fireBullet(Spaceship&, uLCD_4DGL&);
    void moveBullet(uLCD_4DGL&);
    void setBullet(bool);
    bool getBullet();
    int getxBullet();
    int getyBullet();
}; // end class Bullet ------------------------------------


// abstract base class ScreenAliens -----------------------
class ScreenAliens
{
private:
    int xPos, yPos;
    bool direc, hit;
public:
    virtual void draw(uLCD_4DGL&) = 0;
    virtual void update(uLCD_4DGL&) = 0;
    bool hitTest(Bullet&);
    void blackOut(uLCD_4DGL&);
    void changexPos(int);
    void setxPos(int);
    void setyPos(int);
    void setDirec(bool);
    int getxPos();
    int getyPos();
}; // end class ScreenAliens ------------------------------


// derived class AlienBob ---------------------------------
class AlienBob : public ScreenAliens
{
public:
    AlienBob(int);
    void draw(uLCD_4DGL&);
    void update(uLCD_4DGL&);
}; // end class AlienBob ----------------------------------


// derived class AlienAlice -------------------------------
class AlienAlice : public ScreenAliens
{
public:
    AlienAlice(int);
    void draw(uLCD_4DGL&);
    void update(uLCD_4DGL&);
}; // end class AlienAlice --------------------------------


// derived class AlienJim ---------------------------------
class AlienJim : public ScreenAliens
{
public:
    AlienJim(int);
    void draw(uLCD_4DGL&);
    void update(uLCD_4DGL&);
}; // end class AlienJim ----------------------------------


// derived class AlienDan ---------------------------------
class AlienDan : public ScreenAliens
{
public:
    AlienDan(int);
    void draw(uLCD_4DGL&);
    void update(uLCD_4DGL&);
}; // derived class AlienDan ------------------------------


// end of attack.h ----------------------------------------