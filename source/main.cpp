#include <nds.h>
#include <string.h> 
#include <stdlib.h>
#include <stdio.h>
#include <libdsmi.h>


u16 *main_vram = NULL;	// main display memory
touchPosition touch; 	// touchpad
int keysdown;			// key handle down

//class Button

class Button
{
	public:
		char *name;
		bool toggle;
		int id;		
		int curvalue;
		int x;
		int y;
		int width;
		int height;
		//char* name;
	Button(){
		toggle = false;
		curvalue = 0;
	}
	
	
	void button_up()
	{
	  u16 i;
	  
	    //clear dulu
	

		
		//border atas bawah
	
		for(i = 0; i < width; i++)
		{
			main_vram[((x+i)+y*256)] = RGB15(31,0,0) | BIT(15);
			main_vram[((x+i)+(y+height)*256)] = RGB15(31,0,0) | BIT(15);
		}
	
		// border kiri dan kanan
	
		for(i = 0; i < height; i++)
		{
			main_vram[((x)+(y+i)*256)] = RGB15(31,0,0) | BIT(15);
			main_vram[((x+width)+(y+i)*256)] = RGB15(31,0,0) | BIT(15);
		}
	
	}
	
	void button_down()
	{
	  u16 i;
	  u16 j;
	    //border atas dan bawah
	  
		for(i = 0; i < width; i++)
		{
		   for(j = 0; j < height; j++)
		   {
		    	main_vram[((x+i)+(y+j)*256)] = RGB15(31,0,0) | BIT(15);
	       }
		 }
	}
	
	void button_clear()
	{
	  u16 i;
	  u16 j;
		for(i = 0; i < width; i++)
		{
		   for(j = 0; j < height; j++)
		   {
		    	main_vram[((x+i)+(y+j)*256)] = RGB15(0,0,0) | BIT(15);
	       }
		 
		}
	}
	
	void draw()
	{
		button_up();
	}
	
	void update()
	{

		char* testOSC = "test";
		
		if (touch.px >= x &&  touch.px <= (x+width))
		{
			if (touch.py >= y && touch.py <= (y+height))
			{
				if (keysDown() & KEY_TOUCH)
				{
					curvalue = 1;
					testOSC = "";
					sprintf(testOSC, "%s/%d", name, id );
					dsmi_osc_new(testOSC);
					dsmi_osc_addintarg(curvalue);
					dsmi_osc_send();
					button_down();
				}

			}
		}
		
		if (keysUp() & KEY_TOUCH && curvalue == 1)
		{
			curvalue = 0;
			testOSC = "";
			sprintf(testOSC, "%s/%d", name, id );
			dsmi_osc_new(testOSC);
			dsmi_osc_addintarg(curvalue);
			dsmi_osc_send();
			button_clear();
			button_up();			
		}
		
		
	}
};


//class kaospad

class Kaosspad
{
	public:
		char *name;
		int id;		
		int curvalue;
		int x;
		int y;
		int width;
		int height;
		//char* name;
	void draw()
	{
	  u16 i;
	  
	    //border atas dan bawah
	  
		for(i = 0; i < width; i++)
		{
			main_vram[((x+i)+y*256)] = RGB15(31,0,0) | BIT(15);
			main_vram[((x+i)+(y+height)*256)] = RGB15(31,0,0) | BIT(15);
		}
	
		// border kiri dan kanan
	
		for(i = 0; i < height; i++)
		{
			main_vram[((x)+(y+i)*256)] = RGB15(31,0,0) | BIT(15);
			main_vram[((x+width)+(y+i)*256)] = RGB15(31,0,0) | BIT(15);
		}
	
	}
	
	void update()
	{
		int centerX;
		int centerY;
		
		centerX = width/2;
		centerY = height/2;
		
		if (touch.px >= x &&  touch.px <= (x+width))
		{
			if (touch.py >= y && touch.py <= (y+height))
			{
				char* testOSC = "test";
				testOSC = "";
				sprintf(testOSC, "%s/%d", name, id );
				iprintf("\x1b[10;0HTouch name = %s\n", testOSC);
				iprintf("\x1b[10;0HTouch x = %04i, y = %04i\n", touch.px-(centerX+x), (centerY+y)-touch.py);
				dsmi_osc_new(testOSC);
				dsmi_osc_addintarg(touch.px-(centerX+x));
				dsmi_osc_addintarg((centerY+y)-touch.py);
				dsmi_osc_send();
				
			}
		}
		
		
	}
};


//class gui vslider

class Vslider
{
	public:
		char *name;
		int id;
		int x;
		int y;
		int curvalue;
		int width;
		int height;
	void draw()
	{
	  u16 i;
	  
	    //border atas dan bawah
	  
		for(i = 0; i < width; i++)
		{
			main_vram[((x+i)+y*256)] = RGB15(31,0,0) | BIT(15);
			main_vram[((x+i)+(y+height)*256)] = RGB15(31,0,0) | BIT(15);
		}
	
		// border kiri dan kanan
	
		for(i = 0; i < height; i++)
		{
			main_vram[((x)+(y+i)*256)] = RGB15(31,0,0) | BIT(15);
			main_vram[((x+width)+(y+i)*256)] = RGB15(31,0,0) | BIT(15);
		}
	
	}
	
	void setOscName(char * OscName)
	{
		name = OscName;
	}
	
	void update()
	{
		u16 i;
		bool touchOver;
		
		if (touch.px >= x &&  touch.px <= (x+width))
		{
			if (touch.py >= y && touch.py <= (y+height))
			{
				touchOver = true;
			}else{
				touchOver = false;
			}
		}

		if (touchOver && keysHeld() & KEY_TOUCH)
		{
		
				for(i = 1; i < width-1; i++)
				{
					if (curvalue != y && curvalue != y+height)
					{
						main_vram[((x+i)+(curvalue)*256)] = RGB15(0,0,0) | BIT(15);
					}					
				}
		
		
		if (curvalue != touch.py){
			
			curvalue = touch.py;
			char* testOSC = "test";
			testOSC = "";
			sprintf(testOSC, "%s/%d", name, id );				
			dsmi_osc_new(testOSC);
			dsmi_osc_addintarg(height-(touch.py-y));
			dsmi_osc_send();
			iprintf("\x1b[10;0HTouch name = %s\n", testOSC);
			
			for(i = 1; i < width-1; i++)
			{
				main_vram[((x+i)+(touch.py)*256)] = RGB15(31,0,0) | BIT(15);
			}			
						
		}
		

				
				
		}else{
				for(i = 1; i < width-1; i++)
				{
					main_vram[((x+i)+(curvalue)*256)] = RGB15(31,0,0) | BIT(15);
				}
		
		}
		
	}
};

// class Hslider

class Hslider
{
	public:
		char *name;
		int id;		
		int curvalue;
		int x;
		int y;
		int width;
		int height;
		//char* name;

	
	void draw()
	{
	  u16 i;
	  
	    //border atas dan bawah
	  
		for(i = 0; i < width; i++)
		{
			main_vram[((x+i)+y*256)] = RGB15(31,0,0) | BIT(15);
			main_vram[((x+i)+(y+height)*256)] = RGB15(31,0,0) | BIT(15);
		}
	
		// border kiri dan kanan
	
		for(i = 0; i < height; i++)
		{
			main_vram[((x)+(y+i)*256)] = RGB15(31,0,0) | BIT(15);
			main_vram[((x+width)+(y+i)*256)] = RGB15(31,0,0) | BIT(15);
		}
	
	}
	
	void update()
	{
		u16 i;
		bool touchOver;
		
		if (touch.px >= x &&  touch.px <= (x+width))
		{
			if (touch.py >= y && touch.py <= (y+height))
			{
				touchOver = true;
			}else{
				touchOver = false;
			}
		}

		if (touchOver)
		{
		
				for(i = 1; i < height-1; i++)
				{
					if (curvalue != x && curvalue != x+width)
					{
						main_vram[((curvalue)+(y+i)*256)] = RGB15(0,0,0) | BIT(15);
					}					
				}
		
		
				if (curvalue != touch.px){
		
					curvalue = touch.px;
					char* testOSC = "test";
					testOSC = "";
					sprintf(testOSC, "%s/%d", name, id );
					dsmi_osc_new(testOSC);
			    	dsmi_osc_addintarg(width-(touch.px-x));
			    	dsmi_osc_send();
					iprintf("\x1b[10;0HTouch name = %s\n", testOSC);
		
					for(i = 1; i < height-1; i++)
					{
						main_vram[((touch.px)+(y+i)*256)] = RGB15(31,0,0) | BIT(15);
					}
									
				}
				
		}else{
				for(i = 1; i < height-1; i++)
				{
					main_vram[((curvalue)+(y+i)*256)] = RGB15(31,0,0) | BIT(15);
				}
		
		}
		
	}
};


//handle preset

int controlIndex = 0;
bool swap = FALSE;


void handlePad()
{


		scanKeys();
		keysdown = keysDown();
		
		if (keysdown & KEY_L)
		{
			controlIndex--;
			swap = true;
		}
		else if(keysdown & KEY_R)
		{
			controlIndex++;
			swap = true;
		}

		if (controlIndex<0) controlIndex=2;
		if (controlIndex>2) controlIndex=0;
		

		touchRead(&touch);
}

void clear_vram()
{
int i;
		for(i = 0; i < 256*256; i++)
		{
			main_vram[i] = RGB15(0,0,0) | BIT(15);
		}
}

// control declaration

//preset 1
const int slidercnt = 4;
const int buttoncnt = 20;
Vslider slider[slidercnt]; //control declaration
Hslider Hslider1;
Kaosspad Kaoss1;
Button smallButton[buttoncnt];

//preset 2
Vslider slider2[8];
Hslider Hslider2[2];


//preset 3
Kaosspad Kaoss3;


//function prototype

//preset 1
void initControl1();
void drawControl1();
void handleControl1();


//preset 2
void initControl2();
void drawControl2();
void handleControl2();

//preset 3
void initControl3();
void drawControl3();
void handleControl3();


//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	//set video mode to mode 5 with background 3 enabled
	videoSetMode(MODE_5_2D);	
	videoSetModeSub(MODE_0_2D);
	
	//map vram a to start of background graphics memory
	vramSetMainBanks(VRAM_A_MAIN_BG_0x06000000, VRAM_B_MAIN_BG_0x06020000,
		VRAM_C_SUB_BG_0x06200000 , VRAM_D_LCD);
		
	//console printing
	
	PrintConsole topScreen = *consoleInit(0, 3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, false);
	consoleSelect(&topScreen);
	
	
	//BG drawing

	int main_bg = bgInit(2, BgType_Bmp16, BgSize_B16_256x256, 2, 0);
	bgSetPriority(main_bg, 1);

	main_vram = (u16*)bgGetGfxPtr(main_bg);
	//u16 *sub_vram = (u16*)bgGetGfxPtr(0);


	//int sub_bg = bgInitSub(2, BgType_Bmp16, BgSize_B16_256x256, 2, 0);
	//bgSetPriority(sub_bg, 1);
	
	//main display is the touchscreen
	lcdMainOnBottom();
	
	//libdsmi
	
	iprintf("\x1b[15;0H\x1b[KConnecting\n");
	int res = dsmi_connect_wifi();
	
	if(res == 1) {
		iprintf("\x1b[15;0H\x1b[KOK\n");
	} else {
		iprintf("\x1b[15;0H\x1b[KOh no, could not connect!\n");
	}
	
	// control init
	
	initControl1();
	drawControl1();
	
	initControl2();
	initControl3();
	//drawControl2();
	
	while(1) {
		handlePad();
		
		
		switch(controlIndex){
			case 0:
				if (swap == true){clear_vram();drawControl1();swap=false;}
				handleControl1();
			break;
			case 1:
				if (swap == true){clear_vram();drawControl2();swap=false;}
				handleControl2();
			break;
			case 2:
				if (swap == true){clear_vram();drawControl3();swap=false;}
				handleControl3();
			break;	
		}
		swiWaitForVBlank();
	}
	
	
	return 0;
}

//PRESET 1

void initControl1()
{
	int i;
	int offset = 0;
	int row = 0;
	
	
	for(i = 0; i < slidercnt; i++)
	{
			slider[i].id = i;
			slider[i].name = "/preset1/Vslider";
			slider[i].x = 10+offset;
			slider[i].y = 10;
			slider[i].width = 20;
			slider[i].height = 120;
			slider[i].curvalue = slider[i].y + slider[i].height;
			offset = offset + 30;
	}
	
	
	Hslider1.id = 1;
	Hslider1.name = "/preset1/Hslider";
	Hslider1.x = 10;
	Hslider1.y = 140;
	Hslider1.width = 110;
	Hslider1.height = 20;
	Hslider1.curvalue = 10;
	//strcpy(Hslider1.name, "/ds/preset1/Hslider1");
	
	offset = 0;
	
	for (i = 0; i < buttoncnt ; i++)
	{
		if (i % 4 == 0 && i != 0) { row = row + 30; offset = 0;}
		smallButton[i].id = i;
		smallButton[i].name = "/preset1/Button";
		smallButton[i].x = 130 + offset;
		smallButton[i].y = 10 + row;
		smallButton[i].width =  20;
		smallButton[i].height = 20;
		offset = offset + 30;
		//strcpy(Kaoss1.name, "/ds/preset1/Kaoss1");		
	}

}

void drawControl1()
{
	int i;
	for(i = 0; i < slidercnt; i++){slider[i].draw();}	
	for(i = 0; i < buttoncnt; i++){smallButton[i].draw();}	
	Hslider1.draw();
	//Kaoss1.draw();
	//Button1.draw();
}

void handleControl1()
{
int i;

	for(i = 0; i < slidercnt; i++){slider[i].update();}
	for(i = 0; i < buttoncnt; i++){smallButton[i].update();}	
	Hslider1.update();
//	Kaoss1.update();
//	Button1.update();
}

// PRESET 2

void initControl2()
{
	int i;
	int offset = 0;
		
	for(i = 0; i < 8; i++)
	{
			slider2[i].id 		= i;
			slider2[i].name 	= "/preset2/Vslider";
			slider2[i].x  		= 10+offset;
			slider2[i].y  		= 10;
			slider2[i].width 	= 20;
			slider2[i].height	= 170;
			slider2[i].curvalue	= slider2[i].y + slider2[i].height;
			offset = offset + 30;
	}	
	  
}

void drawControl2()
{
	int i;
	for(i = 0; i < 8; i++){slider2[i].draw();}
}

void handleControl2()
{
	int i;
	for(i = 0; i < 8; i++){slider2[i].update();}
}


// PRESET 3

void initControl3()
{
	Kaoss3.id = 1;
	Kaoss3.name = "/preset3/Kaoss";
	Kaoss3.x = 5;
	Kaoss3.y = 5;
	Kaoss3.width =  240;
	Kaoss3.height = 180;	
}

void drawControl3()
{
	Kaoss3.draw();
}

void handleControl3()
{
	Kaoss3.update();
}