%{

 Six Petits Preludes,
 Collection Johann Peter Kellner
 ca 1703 - 1707

 Kellner was a student of Bach's that copied some 90 works of his master.
 
%}

\header{
  copyright =	 "public domain";
  source = "Ed. Henry Lemoine Urtext";
  title =	 "Pr\\\"aludum";
  opus = "BWV 940";
  composer =	 "Johann Sebastian Bach (1685-1750)";
  enteredby =	 "jcn,hwn";

  % mutopia headers.
  mutopiatitle = "Prelude";
  mutopiacomposer = "J.S.Bach";
  mutopiaopus = "BWV940";
  mutopiainstrument = "Piano";
  style = "baroque";
  copyright = "Public Domain";
  maintainer = "Jan Nieuwenhuizen";
  maintainer_email = "janneke@gnu.org";
  tagline =    "\\\\This music is part of the Mutopia project, http://sca.uwaterloo.ca/Mutopia/\\\\It has been typeset and placed in the public domain by " + \maintainer + ".\\\\Unrestricted modification and redistribution is permitted and encouraged - copy this music and share it!";
  maintainer = "janneke@gnu.org";
  lastupdated = "1999/Nov/14";
  
}

\version "1.3.59";



one = \context Staff \notes\relative c{
	\context Voice=i
	\skip 1; |
	\stemup
	r4 d''2 cis4 |
	\stemboth
	d16 a' g f  e f cis d 
	e8.-\mordent f16  d8.-\prall cis16 |
	\stemup
	cis4 ~ cis16 a d8 ~ d4 a |
	%5
	b2 ~ b4 a ~ |
	a16 a \stemup g! f g4 ~ g f ~ |
	f16 a g f  e16 g8. ~  g16 g f e  d f8. ~ |
	f16 f e d b'4 a g |

	fis4 g r8 g16 bes e4 |
	%10
	d1
	\bar "|.";
}

two = \context Staff \notes\relative c{
	\context Voice=ii
	r16 d'' c bes  a bes f g 
	a8.-\mordent bes16  g8.-\prall f16 |
	\stemdown
	f2 e2 |
	\translator Staff=lower \stemup
	r4 a, bes b |
	\translator Staff=upper \stemdown
	r16 b' a g  f8. f16 e2 ~ |
	%5
	e2 ~ e4 ~ e16 e f! d |
	s4 e32 d e8.~ e4 d4 ~ |
	d4. cis16 d cis4 d ~ |
	d8 r r16 e f d r16 e f d r d e cis |
	r16 e d c!  bes! d8. s4 r16 bes' a g |
	%10
	fis1
}

three = \context Staff \notes\relative c{
	\context Voice=iii
	\stemup
	f2 e |
	\stemboth
	d16 d' c bes  a bes f g 
	a8.-\mordent bes16 
	g8.-\prall f16 |
	f2 g4 gis |
	a2 ~ a16 a g f  e f c d |
	%5
	e8.-\mordent f16  d8.-\prall c16 \stemboth c4. d8 |
	\stemdown
	e4 ~ e16 f e d  cis a b cis  d e f d |
	\property Voice . textStyle = "finger"
	bes!2 a ~ |
	a a |
	d, cis' |
	%10
	a'1
	\bar "|.";
}

four = \context Staff \notes\relative c{
	\context Voice=iv
	\stemdown 
	d2 cis |
	\skip 1*2; |
	\skip 4*3;
	\translator Staff=upper \stemup \property Voice.horizontalNoteShift = 1 
	c''4 |
	%5
	a gis ~ gis16 gis fis e 
	\skip 4*1;
	\translator Staff=lower \stemdown \property Voice.horizontalNoteShift = 0 
	\stemup
	b2 a |
	g a4. gis16 a |
	gis2 < g8 cis,> <f d> e4 |
	d4. fis16 g r16 bes8. ~ bes4 |
	%10
	\stemdown
	d,1
}

global = \notes{
	\time 4/4;
	\key f \major;
}

\score{
	% Allegretto
	\context PianoStaff <
		\context Staff = upper <
			\global
			\one
			\two
		>
		\context Staff = lower <
			\global
			\clef "bass";
			\three
			\four
		>
	>
	\paper{
		linewidth = 18.0 \cm;  

	}
	\midi{ \tempo 4 = 40; }
	\header{
		opus = "BWV 940";
	}
}

