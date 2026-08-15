// Created by camilo <3ThomasBorregaardSørensen!!
#include "platform.h"



void copy(vkvg_matrix_t* pcairomatrix, const ::geometry2d::matrix* pmatrix)
{

   pcairomatrix->xx = pmatrix->a1;
   pcairomatrix->yx = pmatrix->a2;
   pcairomatrix->xy = pmatrix->b1;
   pcairomatrix->yy = pmatrix->b2;
   pcairomatrix->x0 = pmatrix->c1;
   pcairomatrix->y0 = pmatrix->c2;

}
