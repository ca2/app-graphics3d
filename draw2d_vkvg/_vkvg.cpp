// Created by camilo <3ThomasBorregaardSørensen!!
#include "platform.h"



void copy(vkvg_matrix_t* pcairomatrix, const ::geometry2d::matrix* pmatrix)
{

   pcairomatrix->xx = (::f32)pmatrix->a1;
   pcairomatrix->yx = (::f32)pmatrix->a2;
   pcairomatrix->xy = (::f32)pmatrix->b1;
   pcairomatrix->yy = (::f32)pmatrix->b2;
   pcairomatrix->x0 = (::f32)pmatrix->c1;
   pcairomatrix->y0 = (::f32)pmatrix->c2;

}
