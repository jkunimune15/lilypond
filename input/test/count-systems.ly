
\version "2.1.26"
\header{
    
    texidoc="@cindex Count Systems
The function which controls the printing of the system number may be also
overridden to print, for example, the number of systems or the system
number of a grob.
" }

% Original comment has no sense? -HJJ:
% This can be most useful to assertain that a piece uses a specified number of lines.

%% What is the problem? -- jcn:
%% See, eg, input/mutopia/J.S.Bach/baerenreiter-sarabande.ly for
%% an application:

%% We want this to perfectly match the Baerenreiter spacing.
%% If we're not using 6 systems, there's definately a problem.
%% #(define (assert-system-count smob n) ...


#(define (display-systemno smob)
  (let* ((this-system (ly:grob-system smob))
	 (systems (ly:spanner-broken-into
		   (ly:grob-original this-system))))
    (display smob)
    (display (list-index systems this-system))
    (newline)))
  

#(define (display-system-count smob)
  (display (length
	    (ly:spanner-broken-into
	     (ly:grob-original
	      (ly:grob-system smob))))))

  
  
\score{
    \notes\relative c''{
	\override NoteHead  #'after-line-breaking-callback
	% = #display-system-count
	= #display-systemno
	c1
	d
    }
    \paper{ indent = 0.0\mm
	    linewidth = 10.0\mm
		raggedright = ##t
	}
}

