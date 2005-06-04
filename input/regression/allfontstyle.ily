\version "2.4.0"

\header{
    texidoc="
Different text styles are used for various purposes.
"
}

\paper {
    raggedright = ##t
}

\relative c'' \context Staff {
    \emptyText
    \repeat volta 2 { \time 4/4 c4^"cuivre"_\fermata }
    \alternative {
	{
	    d-4_\markup { \italic "cantabile"  } }
	{  e }  } \acciaccatura { c16 }

    f4\ff^""^\markup  { \large "Largo" } \mark "B" g 
}
