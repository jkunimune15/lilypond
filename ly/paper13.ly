% paper13.ly

\version "1.3.120";

paperThirteen = \paper {
	staffheight = 13.0\pt;

	\stylesheet #(make-style-sheet 'paper13)
	
	\include "params.ly";
}

\paper { \paperThirteen }
