/*
  cluster.hh

  source file of the GNU LilyPond music typesetter

  (c) 2002--2004 Juergen Reuter <reuter@ipd.uka.de>
*/

#ifndef CLUSTER_HH
#define CLUSTER_HH

#include "lily-guile.hh"
#include "molecule.hh"

class Cluster
{
public:
  DECLARE_SCHEME_CALLBACK (brew_molecule, (SCM ));
  //  DECLARE_SCHEME_CALLBACK (after_line_breaking, (SCM));
  static bool has_interface (Grob *);
  // DECLARE_SCHEME_CALLBACK (set_spacing_rods, (SCM ));
};

#endif // CLUSTER_HH

