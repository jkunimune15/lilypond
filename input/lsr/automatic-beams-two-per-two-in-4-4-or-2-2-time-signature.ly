%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.11.50"

\header {
  lsrtags = "rhythms"

  texidoc = "
In a simple time signature of 2/2 or 4/4, 8th notes are beamed by
default as two sets of four.

Using a macro which overrides the autobeaming behavior, this snippet
changes the beaming to quarter note beats. 

"
  doctitle = "Automatic beams two per two in 4/4 or 2/2 time signature"
} % begin verbatim
% Automatic beams two per two in 4/4 or 2/2 time signature
%              _____
% Default     | | | |
%              _   _
% Required    | | | |

% macro for beamed two per two in 2/2 and 4/4 time signature
qBeam = {
  #(override-auto-beam-setting '(end 1 8 * *) 1 4 'Staff)
  #(override-auto-beam-setting '(end 1 8 * *) 2 4 'Staff)
  #(override-auto-beam-setting '(end 1 8 * *) 3 4 'Staff)
}

\score {
  <<
    \new Staff \relative c'' {
      \time 4/4
      g8^\markup { without the macro } g g g g g g g
      g8 g g g4 g8 g g
    }
    %Use the macro
    \new Staff \relative c'' {
      \time 4/4
      \qBeam
      g8^\markup { with the macro } g g g g g g g
      g8 g g g4 g8 g g
    }
  >>
  \layout {
    \context {
      \Staff
      \override TimeSignature #'style = #'()
    }
  }
}
