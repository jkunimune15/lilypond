%% Do not edit this file; it is auto-generated from input/new
%% This file is in the public domain.
\version "2.11.50"
\header {
  lsrtags = "expressive-marks,tweaks-and-overrides"
  texidoc = "
Caesura marks can be created by overriding the @code{'text}
property of the @code{BreathingSign} object.  A curved caesura
mark is also available.
"
  doctitle = "Inserting a caesura"
} % begin verbatim

\relative c'' {
  \override BreathingSign #'text =
    #(make-musicglyph-markup "scripts.caesura.straight")
  c8 e4. \breathe g8. e16 c4

  \override BreathingSign #'text =
    #(make-musicglyph-markup "scripts.caesura.curved")
  g8 e'4. \breathe g8. e16 c4
}
