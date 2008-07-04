%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.11.50"

\header {
  lsrtags = "expressive-marks"

  texidoc = "
Hairpins may be printed with a circled tip (al niente notation) by
setting the @code{circled-tip} property of the @code{Hairpin} object to
@code{#t}. 

"
  doctitle = "Printing hairpins using al niente notation"
} % begin verbatim
\relative c'' {
  \override Hairpin #'circled-tip = ##t
  c2\< c\!
  c4\> c\< c2\!
}
