% pedal.ly
% 
% Johann Sebastian Bach
% preludium en fuga in D
% bwv 532
%
% fuga --- two bar excerpt of pedal part
%
% example for pedal studies

% ugh, front and back priority are the same...
% one must use (ugh):
%     left front:
%         -\front-\l{toe,heel}
%     left back:
%         -\l{toe,heel}-\back
%     rigth front:
%         -\r{toe,heel}-\front
%     right back:
%         -\back-\r{toe,heel}

% therefore: 
% \lheel \ltoe \rheel \rtoe
% \lbheel \lbheel \lfheel \lftoe
% \rbheel \rbtoe \rfheel \rftoe

\version "0.1.8";

\score{
		\melodic {
			\meter 4/4;
			\key fis cis;
			\clef bass;
			\octave c;
			% silly chord testing...
% these should be stacked...
%			< 'c-\ltoe 'e-\lheel > < c'-\rtoe e'-\rheel>
%			< 'c-\lheel 'e-\ltoe > < c'-\rheel e'-\rtoe> |
			< 'c-\ltoe-\lheel 'e > < c'-\rtoe-\rheel e' >
			< 'c-\lheel-\ltoe 'e > < c'-\rheel-\rtoe e' > |
			% 46
			\duration 16; 
			[d-\lfheel e-\rbtoe fis-\ltoe e-\rbtoe]
			[d-\lfheel e-\rbtoe fis-\lheel e] [d e fis e] [d e fis e] |
			d4 r4 r4 [b-\rtoe a-\ltoe b-\rtoe fis-\ltoe] |
	}
	\paper{}
}

