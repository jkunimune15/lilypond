/*
  bezier.cc -- implement Bezier and Bezier_bow

  source file of the GNU LilyPond music typesetter

  (c) 1998--2000 Jan Nieuwenhuizen <janneke@gnu.org>
*/

#include <math.h>
#include "bezier-bow.hh"
#include "misc.hh"
#include "bezier.hh"
#include "dimensions.hh"
#include "direction.hh"
#include "debug.hh"
#include "main.hh"
#include "lily-guile.hh"

void
flipy (Array<Offset>  &c)
{
  for (int i = c.size (); i--;)
    c[i][Y_AXIS] = - c[i][Y_AXIS];
}

void
rotate (Array<Offset> &c, Real phi)
{
  Offset rot (complex_exp (Offset (0, phi)));
  for (int i = 0; i < c.size (); i++)
    c[i] = complex_multiply (rot, c[i]);
}

void
translate (Array<Offset> &c, Offset o)
{
  for (int i = 0; i < c.size (); i++)
    c[i] += o;
}


Bezier_bow::Bezier_bow (Array<Offset> points, Direction dir)
{
  dir_ = dir;
  encompass_ = points;
  to_canonic_form ();

  rc_factor_ = 1.0;
  height_limit_ = 1.0;
  ratio_ = 1.0;
}

void
Bezier_bow::blow_fit ()
{
  Real len = curve_.control_[3][X_AXIS] ; 
  Real ind = curve_.control_[1][X_AXIS] / len;
  Real h = curve_.control_[1][Y_AXIS] * fit_factor () / len;

  // ugh. Unhardcode this
  if (h > 4 * ind)
    {
      h = 4* ind; 
    }

  if (h > 0.8 + -2 * ind)
    {
      h = 0.8 - 2  *ind; 
    }
  
  curve_.control_[1][Y_AXIS] = h * len;
  curve_.control_[2][Y_AXIS] = h * len;  

  curve_.check_sanity ();
}

void
Bezier_bow::calculate ()
{
  calc_default ();
  if (fit_factor () > 1.0)
    {
      //    calc_tangent_controls ();
      blow_fit ();
    }
}


  
Bezier
Bezier_bow::get_curve ()const
{
  Bezier rv = curve_;
  if (dir_ == DOWN)
    {
      rv.flip (Y_AXIS);
    }

  rv.rotate (alpha_);
  rv.translate (origin_);
  
  return rv;
}

static Real const FUDGE = 1e-8;

/*
  This function calculates 2 center control points,
  based on lines through c_0 --> left disturbing
  and c_3--> right disturbing encompass points.
  
  See Documentation/fonts.tex
 */
void
Bezier_bow::calc_tangent_controls ()
{
  Real b = curve_.control_[3][X_AXIS];
  Real h = curve_.control_[1][Y_AXIS];
  

  Drul_array<Offset> disturb;
  Drul_array<Real> maxtan;  
  maxtan[LEFT]  = maxtan[RIGHT] = h/(b/2);
  disturb[LEFT]  = disturb[RIGHT] =   Offset (b / 2, h);

  for (int i = 1; i < encompass_.size () -1; i++)
    {
      Real y= encompass_[i][Y_AXIS];
      if (y> 0)
        {
	  Real x = encompass_[i][X_AXIS];
	  
	  Direction d = LEFT;
	  do
	    {
	      // 1 if d == LEFT
	      int k = (1 - d)/2;
	      Real tan = y /  ((1-k)* b - d * x);

	      if (tan > maxtan[d])
		{
		  maxtan[d] = tan;
		  disturb[d] = Offset (x,y);
		}
	    }
	  while (flip (&d)!=LEFT);
	}
    }

  for (int i = 0; i < encompass_.size (); i++ )
    h = h >? encompass_[i][Y_AXIS];

  /*
    The curve will always be under line between curve_.control_0 -> curve_.control_1, so
    make it extra steep by slur_rc_factor
  */


  Drul_array<Real> angles;
  Direction d = LEFT;
  do
    {
      maxtan[d] *= -d * rc_factor_;
      angles[d] = atan (maxtan[d]);
    }
  while (flip(&d) != LEFT);

  Real rc3 = 0.0;

  /* 
    if we have two disturbing points, have line through those...
    in order to get a sane line, make sure points are reasonably far apart
    X distance must be reasonably(!) big (division)
   */
  if (abs (disturb[LEFT][X_AXIS] - disturb[RIGHT][X_AXIS]) > FUDGE)
    rc3 = (disturb[RIGHT][Y_AXIS] - disturb[LEFT][Y_AXIS]) / (disturb[RIGHT][X_AXIS] - disturb[LEFT][X_AXIS]);

  else
    rc3 = tan ((angles[LEFT] - angles[RIGHT]) / 2);


  // ugh: be less steep
  rc3 /= 2*rc_factor_;
  

  Real c2 = -maxtan[RIGHT] * curve_.control_[3][X_AXIS];

  // use highest because rc3 is damped.
  Real maxy = disturb[LEFT][Y_AXIS] >? disturb[RIGHT][Y_AXIS];
  Real c3 = disturb[LEFT][Y_AXIS] > disturb[RIGHT][Y_AXIS] ?
    maxy - rc3 * disturb[LEFT][X_AXIS] :
    maxy - rc3 * disturb[RIGHT][X_AXIS];

  curve_.control_[1][X_AXIS] = c3 / (maxtan[LEFT] - rc3);
  curve_.control_[1][Y_AXIS] = maxtan[LEFT] * curve_.control_[1][X_AXIS];
  
  curve_.control_[2][X_AXIS] = (c3 - c2) / (maxtan[RIGHT] - rc3);
  curve_.control_[2][Y_AXIS] = maxtan[RIGHT] * curve_.control_[2][X_AXIS] + c2;


  curve_.check_sanity();
}

/*
  The maximum amount that the encompass points stick out above the bezier curve.
 */
Real
Bezier_bow::fit_factor () const
{
  Real x1 = encompass_[0][X_AXIS];
  Real x2 = encompass_.top ()[X_AXIS];

  Real factor = 1.0;
  for (int i=1; i < encompass_.size ()-1; i++)
    {
      if (encompass_[i][X_AXIS] > x1 && encompass_[i][X_AXIS] < x2)
	{
	 Real y = curve_.get_other_coordinate (X_AXIS, encompass_[i][X_AXIS]);
	 if (y>0)
	   {
	     Real f = encompass_[i][Y_AXIS] / y;
	     factor = factor >? f;
	   }
	}
    }


  return factor;
}




void
Bezier_bow::to_canonic_form ()
{
  origin_ = encompass_[0];
  translate (encompass_,-origin_);

  Offset delta = encompass_.top () - encompass_[0];
  alpha_ = delta.arg ();

  rotate (encompass_, -alpha_);
  if (dir_ == DOWN)
    {
      flipy (encompass_);
    }

  while (encompass_.size () > 1 && encompass_[1][X_AXIS] <= 0.0)
    {
      programming_error ("Degenerate slur: infinite steepness reqd");
      encompass_.del (1);
    }

  Real l = encompass_.top ()[X_AXIS];
  while (encompass_.size () > 1 && encompass_.top (1)[X_AXIS] >= l)
    {
      programming_error ("Degenerate slur: infinite steepness reqd");
      encompass_.del (encompass_.size ()-2);
    }
}



/*
 See Documentation/fonts.tex
 */
void
Bezier_bow::calc_default ()
{
  Real pi = M_PI;

  Real alpha = height_limit_ * 2.0 / pi;
  Real beta = pi * ratio_ / (2.0 * height_limit_);

  Offset delta (encompass_.top ()[X_AXIS] 
    - encompass_[0][X_AXIS], 0);

  Real b = delta.length ();
  Real indent = alpha * atan (beta * b);
  Real height = indent;
 
  curve_.control_ [0] = Offset (0, 0);
  curve_.control_ [1] = Offset (indent, height);
  curve_.control_ [2] = Offset (b - indent, height);
  curve_.control_ [3] = Offset (b, 0);
}



