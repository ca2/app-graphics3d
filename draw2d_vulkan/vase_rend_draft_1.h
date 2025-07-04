// Original Name : vase_rend_draft_1.h

/* Drawing nearly perfect 2D line segments in Vulkan
 * You can use this code however you want.
 * I just hope you to cite my name and the page of this technique:
 * http://artgrammer.blogspot.com/2011/05/drawing-nearly-perfect-2d-line-segments.html
 * http://www.codeproject.com/KB/openGL/vklinedraw.aspx
 * 
 * Enjoy. Chris Tsang.*/
#ifndef VASE_REN_DRAFT_H
#define VASE_REN_DRAFT_H
#include <math.h>
static inline double GET_ABS(double x) {return x>0?x:-x;}
/*
 * this implementation uses only basic vk (vulkan 1.0)
 *   renders in immediate mode by vkBegin/ vkEnd
 *   choose only 1 from vase_rend_draft_1.h and vase_rend_draft_2.h
 *   to your need. if you have no preference, just use vase_rend_draft_2.h
 */
/* this is the master line() function which features:
 * -premium quality anti-aliased line drawing
 * -smaller CPU overhead than other CPU rasterizing algorithms
 * -line thickness control
 * -line color control
 * -can choose to use alpha blend or not
 * 
 * sample usage using alpha blending:
 * 
  vkEnable(VK_BLEND);
  vkBlendFunc(VK_SRC_ALPHA, VK_ONE_MINUS_SRC_ALPHA);
  vkMatrixMode(VK_PROJECTION);
  vkPushMatrix();
	vkLoadIdentity();
	vkOrtho( 0,context_width,context_height,0,0.0f,100.0f);

		line ( 10,100,100,300,		//coordinates
			1.2,			//thickness in px
			0.5, 0.0, 1.0, 1.0,	//line color RGBA
			0,0,			//not used
			true);			//enable alphablend

		//more line() calls and other drawing code...
  vkPopMatrix();
  vkDisable(VK_BLEND); //restore blending options
 *
 * and not using alpha blending (blend to background color):
 *
  vkMatrixMode(VK_PROJECTION);
  vkPushMatrix();
	vkLoadIdentity();
	vkOrtho( 0,context_width,context_height,0,0.0f,100.0f);

		line ( 20,100,110,300,		//coordinates
			1.2,			//thickness in px
			0.5, 0.0, 1.0,		//line color *rgb*
			1.0, 1.0, 1.0,		//background color
			false);			//not using alphablend

		//more line() calls and other drawing code...
  vkPopMatrix();
*/
void line( double x1, double y1, double x2, double y2, //coordinates of the line
	float w, //width/thickness of the line in pixel
	float Cr, float Cg, float Cb, //rgb color components
	float Br, float Bg, float Bb, //color of background when alphablend=false,
					//Br=alpha of color when alphablend=true
	bool alphablend) //use alpha blend or not
{
	double t; double R; double f=w-static_cast<int>(w);
	float A;
	
	if ( alphablend)
		A=Br;
	else
		A=1.0f;
	
	//determine parameters t,R
	/*   */if ( w>=0.0 && w<1.0) {
		t=0.05; R=0.48+0.32*f;
		if ( !alphablend) {
			Cr+=0.88*(1-f);
			Cg+=0.88*(1-f);
			Cb+=0.88*(1-f);
			if ( Cr>1.0) Cr=1.0;
			if ( Cg>1.0) Cg=1.0;
			if ( Cb>1.0) Cb=1.0;
		} else {
			A*=f;
		}
	} else if ( w>=1.0 && w<2.0) {
		t=0.05+f*0.33; R=0.768+0.312*f;
	} else if ( w>=2.0 && w<3.0){
		t=0.38+f*0.58; R=1.08;
	} else if ( w>=3.0 && w<4.0){
		t=0.96+f*0.48; R=1.08;
	} else if ( w>=4.0 && w<5.0){
		t=1.44+f*0.46; R=1.08;
	} else if ( w>=5.0 && w<6.0){
		t=1.9+f*0.6; R=1.08;
	} else if ( w>=6.0){
		double ff=w-6.0;
		t=2.5+ff*0.50; R=1.08;
	}
	//printf( "w=%f, f=%f, C=%.4f\n", w,f,C);
	
	//determine angle of the line to horizontal
	double tx=0,ty=0; //aura thinkness of a line
	double Rx=0,Ry=0; //fading edge of a line
	double cx=0,cy=0; //cap of a line
	double ALW=0.01;
	double Δx=x2-x1;
	double Δy=y2-y1;
	if ( GET_ABS(Δx) < ALW) {
		//vertical
		tx=t; ty=0;
		Rx=R; Ry=0;
		if ( w>0.0 && w<=1.0) { //m:2
			tx = 0.5; Rx=0.0;
		}
	} else if ( GET_ABS(Δy) < ALW) {
		//horizontal
		tx=0; ty=t;
		Rx=0; Ry=R;
		if ( w>0.0 && w<=1.0) {
			ty = 0.5; Ry=0.0;
		}
	} else {
		if ( w < 3) { //approximate to make things even faster
			double m=Δy/Δx;
			//and calculate tx,ty,Rx,Ry
			if ( m>-0.4142 && m<=0.4142) {
				// -22.5< angle <= 22.5, approximate to 0 (degree)
				tx=t*0.1; ty=t;
				Rx=R*0.6; Ry=R;
			} else if ( m>0.4142 && m<=2.4142) {
				// 22.5< angle <= 67.5, approximate to 45 (degree)
				tx=t*-0.7071; ty=t*0.7071;
				Rx=R*-0.7071; Ry=R*0.7071;
			} else if ( m>2.4142 || m<=-2.4142) {
				// 67.5 < angle <=112.5, approximate to 90 (degree)
				tx=t; ty=t*0.1;
				Rx=R; Ry=R*0.6;
			} else if ( m>-2.4142 && m<-0.4142) {
				// 112.5 < angle < 157.5, approximate to 135 (degree)
				tx=t*0.7071; ty=t*0.7071;
				Rx=R*0.7071; Ry=R*0.7071;
			} else {
				// error in determining angle
				//printf( "error in determining angle: m=%.4f\n",m);
			}
		} else { //calculate to exact
			Δx=y1-y2;
			Δy=x2-x1;
			double L=sqrt(Δx*Δx+Δy*Δy);
			Δx/=L;
			Δy/=L;
			cx=-Δy; cy=Δx; //m:3
			tx=t*Δx; ty=t*Δy;
			Rx=R*Δx; Ry=R*Δy;
		}
	}
	
	x1+=cx*0.5; y1+=cy*0.5; //m:4
	x2-=cx*0.5; y2-=cy*0.5;
	
	//draw the line by triangle strip m:1
vkBegin(VK_TRIANGLE_STRIP);
	if ( !alphablend) {vkColor3f( Br,Bg,Bb);} else {vkColor4f( Cr,Cg,Cb, 0);}
	vkVertex2f( x1-tx-Rx-cx, y1-ty-Ry-cy); //fading edge
	vkVertex2f( x2-tx-Rx+cx, y2-ty-Ry+cy);
	
	if ( !alphablend) {vkColor3f( Cr,Cg,Cb);} else {vkColor4f( Cr,Cg,Cb, A);}
	vkVertex2f( x1-tx-cx,y1-ty-cy); //aura
	vkVertex2f( x2-tx+cx,y2-ty+cy);
	vkVertex2f( x1+tx-cx,y1+ty-cy);
	vkVertex2f( x2+tx+cx,y2+ty+cy);
	
	if ( !alphablend) {vkColor3f( Br,Bg,Bb);} else {vkColor4f( Cr,Cg,Cb, 0);}
	vkVertex2f( x1+tx+Rx-cx, y1+ty+Ry-cy); //fading edge
	vkVertex2f( x2+tx+Rx+cx, y2+ty+Ry+cy);
vkEnd();

	//cap
	if ( w < 3) {
		//do not draw cap
	} else {
		//draw cap
	vkBegin(VK_TRIANGLE_STRIP);
		if ( !alphablend) {vkColor3f( Br,Bg,Bb);} else {vkColor4f( Cr,Cg,Cb, 0);}
		vkVertex2f( x1-tx-Rx-cx, y1-ty-Ry-cy);
		vkVertex2f( x1-tx-Rx, y1-ty-Ry);
		if ( !alphablend) {vkColor3f( Cr,Cg,Cb);} else {vkColor4f( Cr,Cg,Cb, A);}
		vkVertex2f( x1-tx-cx, y1-ty-cy);
		if ( !alphablend) {vkColor3f( Br,Bg,Bb);} else {vkColor4f( Cr,Cg,Cb, 0);}
		vkVertex2f( x1+tx+Rx, y1+ty+Ry);
		if ( !alphablend) {vkColor3f( Cr,Cg,Cb);} else {vkColor4f( Cr,Cg,Cb, A);}
		vkVertex2f( x1+tx-cx, y1+ty-cy);
		if ( !alphablend) {vkColor3f( Br,Bg,Bb);} else {vkColor4f( Cr,Cg,Cb, 0);}
		vkVertex2f( x1+tx+Rx-cx, y1+ty+Ry-cy);
	vkEnd();
	vkBegin(VK_TRIANGLE_STRIP);
		if ( !alphablend) {vkColor3f( Br,Bg,Bb);} else {vkColor4f( Cr,Cg,Cb, 0);}
		vkVertex2f( x2-tx-Rx+cx, y2-ty-Ry+cy);
		vkVertex2f( x2-tx-Rx, y2-ty-Ry);
		if ( !alphablend) {vkColor3f( Cr,Cg,Cb);} else {vkColor4f( Cr,Cg,Cb, A);}
		vkVertex2f( x2-tx+cx, y2-ty+cy);
		if ( !alphablend) {vkColor3f( Br,Bg,Bb);} else {vkColor4f( Cr,Cg,Cb, 0);}
		vkVertex2f( x2+tx+Rx, y2+ty+Ry);
		if ( !alphablend) {vkColor3f( Cr,Cg,Cb);} else {vkColor4f( Cr,Cg,Cb, A);}
		vkVertex2f( x2+tx+cx, y2+ty+cy);
		if ( !alphablend) {vkColor3f( Br,Bg,Bb);} else {vkColor4f( Cr,Cg,Cb, 0);}
		vkVertex2f( x2+tx+Rx+cx, y2+ty+Ry+cy);
	vkEnd();
	}
}


/*a skimmed version of line(); no color, no thickness control
 * draws near-perfectly a black "hair line" of thickness 1px
 * when alphablend is false, it assumes drawing on a white surface
 * when alphablend is true, it draws with alpha */
void hair_line( double x1, double y1, double x2, double y2, bool alphablend=0)
{
	double t=0.05; double R=0.768;
	double C=0.0;

	//determine angle of the line to horizontal
	double tx=0,ty=0, Rx=0,Ry=0;
	double ALW=0.01;
	double Δx=x2-x1;
	double Δy=y2-y1;
	if ( GET_ABS(Δx) < ALW) {
		//vertical
		tx = 0.5; ty = 0.0;
		Rx = 0.0; Ry = 0.0;
	} else if ( GET_ABS(Δy) < ALW) {
		//horizontal
		tx = 0.0; ty = 0.5;
		Rx = 0.0; Ry = 0.0;
	} else {
		double m=Δy/Δx;
		if ( m>-0.4142 && m<=0.4142) {
			// -22.5< angle <= 22.5, approximate to 0 (degree)
			tx=t*0.1; ty=t;
			Rx=R*0.6; Ry=R;
		} else if ( m>0.4142 && m<=2.4142) {
			// 22.5< angle <= 67.5, approximate to 45 (degree)
			tx=t*-0.7071; ty=t*0.7071;
			Rx=R*-0.7071; Ry=R*0.7071;
		} else if ( m>2.4142 || m<=-2.4142) {
			// 67.5 < angle <=112.5, approximate to 90 (degree)
			tx=t; ty=t*0.1;
			Rx=R; Ry=R*0.6;
		} else if ( m>-2.4142 && m<-0.4142) {
			// 112.5 < angle < 157.5, approximate to 135 (degree)
			tx=t*0.7071; ty=t*0.7071;
			Rx=R*0.7071; Ry=R*0.7071;
		}
	}
	
	//draw the line by triangle strip
vkBegin(VK_TRIANGLE_STRIP);
	if ( !alphablend) {vkColor3f( 1,1,1);} else {vkColor4f( C,C,C, 0);}
	vkVertex2f( x1-tx-Rx, y1-ty-Ry); //fading edge
	vkVertex2f( x2-tx-Rx, y2-ty-Ry);
	
	vkColor3f( C,C,C);	
	vkVertex2f( x1-tx,y1-ty); //aura
	vkVertex2f( x2-tx,y2-ty);
	vkVertex2f( x1+tx,y1+ty);
	vkVertex2f( x2+tx,y2+ty);
	
	if ( !alphablend) {vkColor3f( 1,1,1);} else {vkColor4f( C,C,C, 0);}
	vkVertex2f( x1+tx+Rx, y1+ty+Ry); //fading edge
	vkVertex2f( x2+tx+Rx, y2+ty+Ry);
vkEnd();
}

/*as a fall back of line()*/
void line_raw( double x1, double y1, double x2, double y2,
	double w,
	double Cr, double Cg, double Cb,
	double,double,double, bool)
{
	vkLineWidth(w);
vkBegin(VK_LINES);
	vkColor3f( Cr,Cg,Cb);
	vkVertex2f( x1,y1);
	vkVertex2f( x2,y2);
vkEnd();
}

#endif
