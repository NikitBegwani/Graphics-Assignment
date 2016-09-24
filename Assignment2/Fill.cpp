#include <iostream>
#include <bits/stdc++.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <windows.h>
#include <map>
#include "Fill.h"

using namespace std;

void fill();

void setupMenus(HWND hwnd)
{
	HMENU hmenu, hsubmenu;
	hmenu=GetMenu(hwnd);
	hsubmenu=GetSubMenu(hmenu, 0);
	
	switch (gDrawData.drawMode)
	{
	    case READY_MODE :
	      // enable 'Draw Circle', disable 'Filling' menu
		    EnableMenuItem(hsubmenu,ID_CIRCLE,
	                     MF_BYCOMMAND|MF_ENABLED);
	    	EnableMenuItem(hsubmenu,ID_FILL,
	                     MF_BYCOMMAND|MF_GRAYED);
	      	break;
	    case DRAW_MODE :
	    case FILL_MODE :
	    case FILLED_MODE :
	      // disable 'Draw Circle', 'Filling' menu 
	      	EnableMenuItem(hsubmenu,ID_CIRCLE,
	                     MF_BYCOMMAND|MF_GRAYED);
	      	EnableMenuItem(hsubmenu,ID_FILL,
	                     MF_BYCOMMAND|MF_GRAYED);
	      	break;
	    case DRAWN_MODE :
	      // enable 'Filling' menus, disable 'Draw Circle' menu
	      	EnableMenuItem(hsubmenu,ID_CIRCLE,
	                     MF_BYCOMMAND|MF_GRAYED);
	      	EnableMenuItem(hsubmenu,ID_FILL,
	                     MF_BYCOMMAND|MF_ENABLED);
	      	break;
	}	
}

void performFilling(HWND hwnd)
{
	setDrawMode(FILL_MODE, hwnd);
	SelectObject(gDrawData.hdcMem, gDrawData.hFillPen);
	fill();
	reDraw(hwnd);
	setDrawMode(FILLED_MODE, hwnd);
}

void processCommand(int cmd, HWND hwnd)
{
  	switch(cmd)
  	{
	    case ID_FILL:
	    	performFilling(hwnd);
	      	break;
	    default:
	      	processCommonCommand(cmd, hwnd);
	      	break;
  	}
}


bool boundaryCheck(int x, int y, int cx, int cy , int r)
{
  	if((cx-x)*(cx-x) + (cy -y)*(cy -y) - r*r <= 0)
    	return 1;
  	else 
	    return 0;
}


void fill()
{
	      
	int i, x1, x2, x3 ,y1, y2, y3;
	int r = gDrawData.radius, temp;
	
	x1 = gDrawData.cornerPts[0].x;
	y1 = gDrawData.cornerPts[0].y;

	x2 = gDrawData.cornerPts[1].x;
	y2 = gDrawData.cornerPts[1].y;
	
	x3 = gDrawData.cornerPts[2].x;
	y3 = gDrawData.cornerPts[2].y;

	vector <int> ycoordinates, xcoordinates, commonspace1, commonspace2, commonlist;
	
	ycoordinates.push_back(y1);
	ycoordinates.push_back(y2);
	ycoordinates.push_back(y3);
	
	sort(ycoordinates.begin(), ycoordinates.end());
	
	for(int i = ycoordinates[0] -r; i<= ycoordinates[2] +r; i++)
	{
		xcoordinates.clear();
	
	   	if( i>= (y1- r) &&  i <= (y1 + r) )
	    {
		    temp = sqrt(r*r - (y1-i)*(y1-i));
		    xcoordinates.push_back(x1 + temp);
		    xcoordinates.push_back(x1 - temp);
	    }
	
	    if( i>= (y2- r) &&  i <= (y2 + r) )
	    {
		    temp = sqrt(r*r - (y2-i)*(y2-i));
		    xcoordinates.push_back(x2 + temp);
		    xcoordinates.push_back(x2 - temp);
	    }
	
	    if( i>= (y3- r) &&  i <= (y3 + r) )
	    {
		    temp = sqrt(r*r - (y3-i)*(y3-i));
		    xcoordinates.push_back(x3 + temp);
		    xcoordinates.push_back(x3 - temp);
	    }
	
	  	sort(xcoordinates.begin(), xcoordinates.end());
	
	
	  	int n = xcoordinates.size();
	    
	    for(int j =0; j< n-1;j++)
	    {
		    commonspace1.clear();
		    commonspace2.clear();
	    
			if(boundaryCheck(xcoordinates[j],i,x1,y1,r))
		    {
		       	commonspace1.push_back(1);  
		    }
		    if(boundaryCheck(xcoordinates[j],i,x2,y2,r))
		    {
		    	commonspace1.push_back(2);  
		    }
		    if(boundaryCheck(xcoordinates[j],i,x3,y3,r))
		    {
		       	commonspace1.push_back(3);  
		    }
		    
	     	if(boundaryCheck(xcoordinates[j+1],i,x1,y1,r))
	      	{
	        	commonspace2.push_back(1);  
	      	}
			if(boundaryCheck(xcoordinates[j+1],i,x2,y2,r))
	      	{
	        	commonspace2.push_back(2);  
	      	}
	      	if(boundaryCheck(xcoordinates[j+1],i,x3,y3,r))
	      	{
		        commonspace2.push_back(3);  
		    }
		
			commonlist.clear(); 
		  	int len1 =0, len2 =0;
		  	while(len1 < commonspace1.size() && len2 < commonspace2.size())
		  	{
		  		if(commonspace1[len1] == commonspace2[len2])
		  		{
			  		commonlist.push_back(commonspace1[len1]);
			  		len1++;
			  		len2++;
				}
		  		else if (commonspace1[len1] < commonspace2[len2])
		  			len1++;
		  		else
				  	len2++;
		  	}		  
		 	
		 	COLORREF ref;
		 
			if(commonlist.size()==0)
		    	ref = RGB(255,255,255);
	    	
		   	else if(commonlist.size()==3)
			 	ref=RGB(255,0,0);
		 	
			else if(commonlist.size()==1)
				ref=RGB(0,255,255);	 
			
			else if(commonlist[0]==1 && commonlist[1]==2)
		    	ref=RGB(0,0,255);
	      
		  	else if(commonlist[0]==2)
	    		ref=RGB(0,255,0);
	    
	  		else
				ref=RGB(255,255,0);
	    
	  		for(int len1 = xcoordinates[j]; len1 <= xcoordinates[j+1] ; len1++)
		 		SetPixel(gDrawData.hdcMem, len1, i, ref);
		  		  
	  	}        
	
	}

}
