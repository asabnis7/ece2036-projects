// Arjun Sabnis
// LemurTest.cpp --------------------------------------------------------------

#include "mbed.h"
#include "uLCD_4DGL.h"
#include "PinDetect.h"
#include "SDFileSystem.h"
#include "Speaker.h"
#include <cstdlib>
#include <ctime>
#include <fstream>

// set up global objects ------------------------------------------------------

PinDetect pbLeft(p15);	// L select
PinDetect pbRight(p16); // R select
PinDetect pbReset(p17); // exit pushbutton

SDFileSystem sd(p11, p12, p13, p8, "sd"); // sd system

Speaker startTone(p21); // set up speaker object

uLCD_4DGL LCD(p28, p27, p29); // tx, rx, reset

Timer t; // timer for results


// draw bins ------------------------------------------------------------------
void binDraw()
{
	// set up initial conditions
	LCD.background_color(BLACK);
	LCD.cls();

	// draw first bin
	LCD.line(3, 4, 63, 4, WHITE);
	LCD.line(3, 124, 63, 124, WHITE);
	LCD.line(3, 4, 3, 124, WHITE);
	LCD.line(63, 4, 63, 125, WHITE);

	// draw second bin
	LCD.line(65, 4, 125, 4, WHITE);
	LCD.line(65, 124, 125, 124, WHITE);
	LCD.line(65, 4, 65, 124, WHITE);
	LCD.line(125, 4, 125, 125, WHITE);
}
// end function ---------------------------------------------------------------


// choose shapes --------------------------------------------------------------
void shapeChoice(int i, int j)
{
	// shape variables
    int shape, rad;
        
    // determine shape
    shape = (rand() % 50) % 3;
    rad = 2 + (rand() % 7);
    	
    switch(shape)
    {
    	case(0):
    		LCD.triangle(i, j + rad, i - rad, j - (rad/2), i + rad, j - (rad/2), (rand() % 0xFFFFFF));
    		break;
    		
    	case(1):
			LCD.filled_circle(i, j, rad, (rand() % 0xFFFFFF));
   			break;
    		
   		case(2):
   			LCD.filled_rectangle(i - rad, j - rad, i + rad, j + rad, (rand() % 0xFFFFFF));
   			break;
  	}
}
// end function ---------------------------------------------------------------


// shape draw function ----------------------------------------------------------
void shapeDraw(int nmax, int i_array[], int j_array[])
{
	int i, j, index;
	
	// draw in bin
	for (int n = 0; (n < nmax); n++)
	{
		// store index values for object center
		index = (rand() % 17);
		i = i_array[index];
		j = j_array[index];

		// make sure index not repeated
		while (i == 0 & j == 0)
		{
			index = (rand() % 17);
			i = i_array[index];
			j = j_array[index];
		}

		// clear array location
		i_array[index] = 0;
		j_array[index] = 0;

		// choose shape
		shapeChoice(i, j);
	}
}
// end function ---------------------------------------------------------------


// pushbutton exit function ---------------------------------------------------
void pbExit()
{
	LCD.cls();
	LCD.printf("Thank you lemurs, time for a break!");
	wait(2.0);
	LCD.cls();
	exit(0);
}
// end function ---------------------------------------------------------------


// declare class DrawObject ---------------------------------------------------
class DrawObject
{
	private:
		int n1max, n2max;
		static int total, right;
		bool correct;
		float dtime;
	
	public:
		// draw bins and objects
		DrawObject()
		{
			binDraw();
			
			// coordinate arrays to check for repetition
			int i1_array[] = { 13,13,13,13,13,13,33,33,33,33,33,33,53,53,53,53,53,53 };
			int i2_array[] = { 75,75,75,75,75,75,95,95,95,95,95,95,115,115,115,115,115,115 };
			int j1_array[] = { 14,34,54,74,94,114,14,34,54,74,94,114,14,34,54,74,94,114 };
			int j2_array[] = { 14,34,54,74,94,114,14,34,54,74,94,114,14,34,54,74,94,114 };
			
			// ensure num of shapes is different
			n1max = (1 + rand() % 15);
			n2max = (1 + rand() % 15);
			while (n1max == n2max)
			{
				n2max = (1 + rand() % 15);
			}

			// draw in first bin
			shapeDraw(n1max, i1_array, j1_array);

			// draw in second bin
			shapeDraw(n2max, i2_array, j2_array);
		}


		~DrawObject() {} // class destructor


		// print to screen
		void printResult()
		{
			total++;
			if (correct == true)
			{
				LCD.printf("Lemur was correct! \n");
				right++;
			}
			else
			{
				LCD.printf("Lemur was wrong! \n");
			}
			LCD.printf("Trial Number = %u \n", total);
			LCD.printf("Fraction Correct = %u/%u \n", right, total);
			LCD.printf("Time Taken = %f \n", dtime);
		}


		// print to file
		void pbFilePrint(FILE* fp_ptr)
		{
			fp_ptr = fopen("/sd/mydir/LemurResults.txt", "a");

			if (correct == true)
			{
				fprintf(fp_ptr, "Lemur was correct! \r\n");
			}
			else
			{
				fprintf(fp_ptr, "Lemur was wrong! \r\n");
			}
			fprintf(fp_ptr, "Trial Number = %u \r\n", total);
			fprintf(fp_ptr, "Fraction Correct = %u/%u \r\n", right, total);
			fprintf(fp_ptr, "Time Taken = %f \r\n", dtime);
			fprintf(fp_ptr, "\r\n \r\n");

			fclose(fp_ptr);
		}


		// getters	
		int get_n1() { return n1max; }
		int get_n2() { return n2max; }
		

		//setter
		void set_answer(bool b) { correct = b; }
		void set_time(float f) { dtime = f; }
}; 
// end class DrawObject -------------------------------------------------------

// define static ints
int DrawObject::total = 0;
int DrawObject::right = 0;

// print to screen/file -------------------------------------------------------
void print(DrawObject d, FILE *fp)
{
	LCD.cls();
	d.printResult();
	d.pbFilePrint(fp);
	wait(3.0);
}
// end function ---------------------------------------------------------------


// begin main function --------------------------------------------------------
int main()
{		
	// set pushbuttons to active low
	pbLeft.mode(PullUp);
    pbRight.mode(PullUp);
    pbReset.mode(PullUp);
    
    // quit application whenever needed
    pbReset.attach_deasserted(&pbExit);
	pbReset.setSampleFrequency();
    
    // Delay for initial pullup to take effect
    wait(.01);
    
    // create file to write to
    mkdir("/sd/mydir", 0777);
    FILE *fp = fopen("/sd/mydir/LemurResults.txt", "w");
    if(fp == NULL) 
    {
        LCD.printf("Error Open, please restart. \n");
        exit(0);
    }
	fclose(fp);
	
	// speaker plays tones
	int f = 1;
	for ((f = 1); f < 11; f++)
	{
		startTone.PlayNote((300.0 + 30*f), 0.25, 0.1);
	}
	
	// LCD setup
    LCD.display_control(PORTRAIT_R);
	LCD.cls();
	LCD.baudrate(BAUD_3000000); // jack up baud rate for fast screen
	LCD.printf("Welcome, lemur test subjects!"); // print intro message
	wait(2.0);
	
	int n1 , n2;
	bool answer = false;
	
	for (int trials = 0; (trials < 101); trials++)
	{
		DrawObject d1;
		t.start();
		
		// lemur selects answer
		while (!answer)
		{
			n1 = d1.get_n1();
			n2 = d1.get_n2();
		
			// left press
			if (!pbLeft)	
			{
				answer = true;
				// get time
				t.stop();
				d1.set_time(t.read());
				
				if (n1 < n2)
				{
					d1.set_answer(true);
					print(d1, fp);
				}
				else
				{
					d1.set_answer(false);		
					print(d1, fp);
				}	
			}
			
			// right press
			if (!pbRight)
			{
				answer = true;
				// get time
				t.stop();
				d1.set_time(t.read());
				
				if (n2 < n1)
				{
					d1.set_answer(true);
					print(d1, fp);
				}
				else
				{
					d1.set_answer(false);		
					print(d1, fp);
				}
			}
		} // end while loop
		
		t.reset();
		answer = false;
	}
	
	if (!pbReset)	
	{
		pbExit();
	}
} 
// end main program -----------------------------------------------------------


// end of file LemurTest.cpp --------------------------------------------------
