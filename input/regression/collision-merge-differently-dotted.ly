\version "2.6.0"
\header {
    
    texidoc = "If NoteCollision has merge-differently-dotted = ##t note
heads that have differing dot counts may be merged anyway.  Dots
should not disappear when merging similar note heads."
    
}

\layout { raggedright= ##t }
	
\context Staff \relative c'' <<
  {
    g8[ g8]
    \override Staff.NoteCollision  #'merge-differently-dotted = ##t
    g8[ g8]
    g4. r8 g8. g16
    g8 g4 r8 g4
  }
  \\
  {
    g8.[ f16]
    g8.[ f16]
    g8 g4 r8 g4
    g4. r8 g8. g16
  }
>>

