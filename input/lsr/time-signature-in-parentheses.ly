%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.11.50"

\header {
  lsrtags = "staff-notation, tweaks-and-overrides"

  texidoc = "
The time signature can be enclosed within parentheses.

"
  doctitle = "Time signature in parentheses"
} % begin verbatim
tsMarkup = \markup {
  \override #'(baseline-skip . 0)
  \number {
    \bracket \column { 2 4 }
  }
}

\relative c'' {
  \override Staff.TimeSignature #'stencil = #ly:text-interface::print
  \override Staff.TimeSignature #'text = #tsMarkup
  \time 2/4
  a4 b8 c
}
