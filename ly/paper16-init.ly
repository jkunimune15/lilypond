% paper16-init.ly


\version "1.7.18"

paperSixteen = \paper {
	staffheight = 16.0\pt
	#(define fonts (make-font-list 'paper16))

	\include "params-init.ly"
}

\paper { \paperSixteen }
