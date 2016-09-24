#include <windows.h>
#include "Line.h"
#include "ClipUtil.h"

int code(double x, double y)
{
  return ( ((x < gDrawData.clipMin.x) << 3) +
           ((x > gDrawData.clipMax.x) << 2) +
           ((y < gDrawData.clipMin.y) << 1) +
            (y > gDrawData.clipMax.y));
}

void scLineClip(POINT start, POINT end)
{
	POINT end2 = end;
	POINT start2 = start;
	int p[4],q[4],i,dx,dy,flag=1;
    double u1=0,u2=1,temp;
        dx=end.x-start.x;
        dy=end.y-start.y;
        p[0]=-dx;     q[0]=start.x-gDrawData.clipMin.x;
        p[1]=dx;      q[1]=gDrawData.clipMax.x-start.x;
        p[2]=-dy;     q[2]=start.y-gDrawData.clipMin.y;
        p[3]=dy;      q[3]=gDrawData.clipMax.y-start.y;
 
        if(p[0]==0 && p[3]==0)   //Point Clipping
        {
                if(start.x>=gDrawData.clipMin.x && start.x<=gDrawData.clipMax.x && start.y>=gDrawData.clipMin.y && start.y<=gDrawData.clipMax.y)
                        SetPixel(gDrawData.hdcMem, start.x,start.y, (0,255,0));
                else
                        return;
        }
 
        if (p[0]==0)
                if(q[0]*q[1]<=0)
                        return;             //for Parallel lines
        if(p[2]==0)
                if(q[2]*q[3]<=0)
                        return;
 
        for (i=0;i<4;i++)
        {
                if(p[i]<0 && flag)
                {
                        temp=(double)q[i]/(double)p[i];
                        if(temp>u2)
                                flag=0;
                        else
                        if(temp>u1)
                                u1=temp;
                }
                else
                if(p[i]>0  && flag)
                {
                        temp=(double)q[i]/(double)p[i];
                        if(temp<u1)
                                flag=0;
                        else
                        if(temp<u2)
                                u2=temp;
                }
        }
        if (u1>=u2 || flag==0)
                return;
        temp=start.x;
        i=start.y;
        start.x=temp+u1*dx;
        end.x=temp+u2*dx;
        start.y=i+u1*dy;
        end.y=i+u2*dy;
        
        //Three methods here
        //drawLineSegment(gDrawData.hdcMem, start2, end2, CLR_BG);
        //drawLineSegment(gDrawData.hdcMem, start, end, CLR_LINE);
        
        //drawLineSegment(gDrawData.hdcMem, start2, start, CLR_BG);
        //drawLineSegment(gDrawData.hdcMem, end2, end, CLR_BG);
        
  		performCorrectionAtClipPts(gDrawData.hdcMem, start,
                             CLR_LINE, CLR_BG);
		performCorrectionAtClipPts(gDrawData.hdcMem, end,
                             CLR_LINE, CLR_BG); 
        
}

void clip(HWND hwnd)
{
  scLineClip(gDrawData.lineEndPts[0], gDrawData.lineEndPts[1]);
  reDraw(hwnd);
  setDrawMode(CLIPPED_MODE, hwnd);
}
