%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.11.49"

\header {
  lsrtags = "staff-notation, editorial-annotations, contexts-and-engravers, paper-and-layout"

  texidoc = "
To create blank staves, generate empty measures then remove the
@code{Bar_number_engraver} from the @code{Score} context, and the
@code{Time_signature_engraver}, @code{Clef_engraver} and
@code{Bar_engraver} from the @code{Staff} context. 

"
  doctitle = "Creating blank staves"
} % begin verbatim
#(set-global-staff-size 20)

\score {
  { 
    \repeat unfold 12 { s1 \break } 
  }
  \layout {
    indent = 0\in
    \context {
      \Staff
      \remove "Time_signature_engraver"
      \remove "Clef_engraver"
      \remove "Bar_engraver"
    }
    \context {
      \Score
      \remove "Bar_number_engraver"
    }
  }
}

\paper {
  #(set-paper-size "letter")
  ragged-last-bottom = ##f
  line-width = 7.5\in
  left-margin = 0.5\in
  bottom-margin = 0.25\in
  top-margin = 0.25\in
}
