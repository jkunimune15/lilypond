 
vi1=\melodic{
  \meter 4/4;
  \octave c'';

  B8 G G G16 A B8 G G G16 A | 
	
}

vi2=\melodic{
  \meter 4/4;
  \octave c'';

% r16
  r1
}

%{
bc=\melodic{
  \clef "bass";
  \meter 4/4;
  \octave c;

  G1 |
}
%}

\score{
  < \multi 3;
    \vi1
    \vi2

  >

  \paper{
    geometric=1.2;
    unitspace=8.\mm;
    linewidth=18.\cm;
    gourlay_maxmeasures=4.;
    gourlay_energybound = 70000.;
	basicspace = 2.8\pt;
  }
}
