%{
 chord modifiers
%}

\chordmodifiers {
	m 	= \musicalpitch { 0 2 -1 }
	min 	= \musicalpitch { 0 2 -1 }
	aug 	= \musicalpitch { 0 4 1 }
	dim 	= \musicalpitch { 0 4 -1 }
	% urg, not actually a chord-modifier, but it works
	% c7 -> <c bes>, c 7+ -> c b
	maj 	= \musicalpitch { 0 6 1 }
	% sus4 should delete 2 too...
	sus 	= \musicalpitch { 0 3 0 }
}
