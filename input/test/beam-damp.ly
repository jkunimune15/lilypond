\header {
texidoc = "Beams are less steep than the notes they encompass. "
}

\version "1.7.6"
\score{
	\notes\relative c''{
%		\stemUp
%		[a16 b b c]
%		[c b b a]
%		\stemDown
%		[c b b a]
%		[a b b c]
		\stemUp
		[g16 a b c]
		[c b a g]
		\stemDown
		[d' c b a]
		[a b c d]
	}
	\paper{
		linewidth=-1.
	}
}

%% new-chords-done %%
