\version "2.19.36"

\header {
  texidoc = "The @var{ly:one-line-auto-height-breaking} algorithm puts 
  everything on one line and crops the page-height to fit the music."
}

\paper { page-breaking = #ly:one-line-auto-height-breaking }

\include "typography-demo.ly"
