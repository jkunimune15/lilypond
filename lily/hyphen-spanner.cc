/*
  hyphen-spanner.cc -- implement Hyphen_spanner

  source file of the GNU LilyPond music typesetter

  (c) 1999 Glen Prideaux <glenprideaux@iname.com>

  (adapted from lyric-extender)
*/

#include <math.h>

#include "box.hh"
#include "lookup.hh"
#include "molecule.hh"
#include "paper-def.hh"
#include "hyphen-spanner.hh"
#include "spanner.hh"
#include "item.hh"

MAKE_SCHEME_SCORE_ELEMENT_CALLBACK(Hyphen_spanner,brew_molecule)

SCM 
Hyphen_spanner::brew_molecule (SCM smob)
{
  Spanner * sp = dynamic_cast<Spanner*> (unsmob_element (smob));

  Score_element * common = sp;
  Direction d = LEFT;
  do
    {
      common = common->common_refpoint( sp->get_bound (d), X_AXIS);
    }
  while (flip (&d) != LEFT);
  Interval bounds;
  
  do
    {
      Real  x = sp->get_bound (d)->relative_coordinate (common, X_AXIS);
      Interval ext =  sp->get_bound (d)->extent (X_AXIS);
      bounds[d] = (x + ext[-d]);
    }
  while (flip (&d) != LEFT);

  
  
  Real ss = sp->paper_l ()->get_var ("staffspace");
  Real lt = sp->paper_l ()->get_var ("stafflinethickness");
  Real th = gh_scm2double (sp->get_elt_property ("thickness")) * lt ;
  Real h = gh_scm2double (sp->get_elt_property ("height")) * ss;
  Real l = gh_scm2double (sp->get_elt_property ("minimum-length")) * ss;  
  Real w  = bounds.length ();
  /* First try: just make the hyphen take 1/3 of the available space  
   for length, use a geometric mean of the available space and some minimum
  */
  if(l < w)
    l = sqrt(l*w);

  Box b  (Interval (-l/2,l/2), Interval (h,h+th));
  Molecule mol (sp->lookup_l ()->filledbox (b));
  mol.translate_axis (bounds.center ()
		      -sp->relative_coordinate (common, X_AXIS),
		      X_AXIS);
  return mol.create_scheme ();
}
  
void
Hyphen_spanner::set_textitem (Direction d, Score_element* b)
{
  elt_l_->set_bound (d, b);
  elt_l_->add_dependency (b);
}

Hyphen_spanner::Hyphen_spanner (Spanner*s)
{
  elt_l_ = s;
}
