\version "2.3.4"
\header {
    texidoc = "There are three different kinds of grace types: the base 
grace switches to smaller type, the appoggiatura inserts also a slur, and the
acciaccatura inserts a slur and slashes the stem." 
    }

\paper {
    raggedright = ##t
}

\relative c'' {
    c4 \grace { d8 }  c4
    \appoggiatura { d8 } c
    \acciaccatura { d } c
}
    
 
