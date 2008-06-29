%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.11.49"

\header {
  lsrtags = "expressive-marks"

  texidoc = "
Dynamics that occur at, begin on, or end on the same note will be
vertically aligned.  To ensure that dynamics are aligned when they do
not occur on the same note, increase the @code{staff-padding} property
of the @code{DynamicLineSpanner} object. 

"
  doctitle = "Vertically aligning dynamics across multiple notes"
} % begin verbatim
\relative c' {
  \override DynamicLineSpanner #'staff-padding = #4
  c2\p f\mf
  g2\< b4\> c\!
}
