\version "1.5.54"

\header {

texidoc="Bar number settable and padding adjustable.  Bar numbers
start counting after the anacrusis."

}

\score {
  \notes \relative c'' {
      \partial 4 c4 
      c1 c c
      \property Score.currentBarNumber = #25
      \property Score.BarNumber \override #'padding = #3
      c1 c
  }
  \paper {
    linewidth = -1. \mm
    \translator {
	\ScoreContext
	BarNumber \override #'visibility-lambda = #all-visible
    }
  }
}
