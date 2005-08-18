\version "2.6.0"

\header{
  texidoc="
Across linebreaks, the left edge of a first and second alternative
bracket should be equal.
"
}

\layout { raggedright= ##t }

\context Staff\relative c''  {
  \repeat "volta" 2 { c1 \break } \alternative { d e }
  c1
  \repeat "volta" 2 { c1 } \alternative { { d \break}  e }

}


