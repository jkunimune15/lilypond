%
% setup for Request->Element conversion. Guru-only
%

StaffContext=\translator {
	\type "Line_group_engraver_group";
	\name Staff ;
	barAuto = "1";
	voltaVisibility = "1";

%{
	The Hara_kiri_line_group_engraver is a Line_group_engraver 
	that will not typeset an empty line of staff, i.e., a line 
	of staff with only rests in it.  This is needed for orchestral
	scores.  Comment-out Line_group_engraver_group, and uncomment 
	Hara_kiri_line_group_engraver.
%}
%	\type "Hara_kiri_line_group_engraver";

	\consists "Multi_measure_rest_engraver";
	\consists "Repeat_engraver";
	\consists "Bar_engraver";
	\consists "Clef_engraver";
	\consists "Key_engraver";
	\consists "Local_key_engraver";
	\consists "Time_signature_engraver";
	\consists "Staff_symbol_engraver";
	\consists "Collision_engraver";
	\consists "Rest_collision_engraver";

%{
	Uncomment to get bar numbers on single staff systems:
	
	The Bar_number_engraver puts a number over a staff created
	at the same level of hierarchy.  This why you have to add it
	here separately if you want to have numbers on single staff
	systems: The Bar_number_engraver in Score_engraver will only
	put numbers on bars that are Score ("system") wide.  Such
	bars are only created when the toplevel system has multiple
	children-staffs.

%}
%{
	\consists "Bar_number_engraver";
%}
%{
	The Staff_margin_engraver puts the name of the instrument
	(\property Staff.instrument; Staff.instr for subsequent lines)
	to the left of a staff.
%}
	defaultClef = treble;

	marginBreakPriority = "-5";

%{
	\consists "Staff_margin_engraver";
%}
	\consists "Separating_line_group_engraver";
	  
	\accepts "Voice";
	dynamicStyle = "dynamic";
};

\translator{\StaffContext }
\translator {
	\type "Line_group_engraver_group";
	\name ChoirStaff;
	\consists "Vertical_align_engraver";
	\consists "Staff_group_bar_engraver";
	\accepts "Staff";
	\accepts "RhythmicStaff";
	\accepts "GrandStaff";
	\accepts "PianoStaff";
	
	\accepts "Lyrics";
	\accepts "ChordNames";
}


RhythmicStaffContext=\translator{
	\type "Line_group_engraver_group";
	numberOfStaffLines  = "1";
	\consists "Pitch_squash_engraver";
	\consists "Separating_line_group_engraver";	
	\name RhythmicStaff;

	\consists "Repeat_engraver";
	\consists "Bar_engraver";
	\consists "Time_signature_engraver";
	\consists "Staff_symbol_engraver";
	\accepts "Voice";
};
\translator{\RhythmicStaffContext}
VoiceContext = \translator {
	\type "Engraver_group_engraver";
	\consists "Dynamic_engraver";
	\name Voice ;
	
 	\consists "Rest_engraver";
	\consists "Dot_column_engraver";
	\consists "Stem_engraver";
	\consists "Beam_engraver";
	\consists "Abbreviation_beam_engraver";
%	\consists "Multi_measure_rest_engraver";

	% ugh.  Order matters here.
	\consists "Text_engraver";
	\consists "Script_engraver";
	\consists "Rhythmic_column_engraver";
	\consists "Font_size_engraver";
	\consists "Slur_engraver";
	\consists "Tie_engraver";
	\consists "Tuplet_engraver";	
	\consists "Note_heads_engraver" ;	
	\consists "Skip_req_swallow_translator";
};

\translator {\VoiceContext}

GrandStaffContext=\translator{
	\type "Line_group_engraver_group";
	\name GrandStaff;
	\consists "Span_bar_engraver";
	\consists "Vertical_align_engraver";
	\consists "Piano_bar_engraver";
	minVerticalAlign = 1.5*\staffheight;

	\accepts "Staff";
};
\translator{\GrandStaffContext}
\translator{\GrandStaffContext
	minVerticalAlign = 3.0*\staffheight;
	maxVerticalAlign = 3.0*\staffheight;
	\name "PianoStaff";
	
}
\translator {
	\type "Line_group_engraver_group";
%	\type "Hara_kiri_line_group_engraver";
	\consists "Span_bar_engraver";
	\consists "Vertical_align_engraver";
	\consists "Staff_group_bar_engraver";
	\name StaffGroup;
	\accepts "Staff";
	\accepts "RhythmicStaff";
	\accepts "GrandStaff";
	\accepts "PianoStaff";
	
	\accepts "Lyrics";
	\accepts "ChordNames";
}

\translator{
	\type "Line_group_engraver_group";

	\name LyricVoice ;
	\consists "Separating_line_group_engraver";
	\consists "Lyric_engraver";
	\consists "Extender_engraver";
	\consists "Beam_req_swallow_translator";
}

\translator {
	\type "Line_group_engraver_group";
	\name Lyrics;
	\consists "Vertical_align_engraver";
	\accepts "LyricVoice";
}

\translator{
	\type "Line_group_engraver_group";

	\name ChordNameVoice ;
	\consists "Separating_line_group_engraver";
	\consists "Chord_name_engraver";
}

\translator {
	\type "Line_group_engraver_group";
	\name ChordNames;
	\consists "Vertical_align_engraver";
	\accepts "ChordNameVoice";
}

ScoreContext = \translator {
	\type Score_engraver;
	\name Score;

	\consists "Timing_engraver";

	\consists "Span_score_bar_engraver";
	\consists "Score_priority_engraver";
	\consists "Priority_horizontal_align_engraver";
	\consists "Vertical_align_engraver";


	\accepts "StaffGroup";
	\accepts "Staff";
	\accepts "RhythmicStaff";	
	\accepts "Lyrics";
	\accepts "ChordNames";
	\accepts "GrandStaff";
	\accepts "ChoirStaff";
	\accepts "PianoStaff";
};
\translator { \ScoreContext }

ScoreWithNumbers = \translator {
 	\type "Score_engraver";

	% uncomment to bar numbers on a whole system.
	\consists "Bar_number_engraver";

};

StupidScore = \translator {
 	\type "Score_engraver";
	\name Score;
	\consists "Note_heads_engraver";
};



BarNumberingStaffContext = \translator {
	\StaffContext
	barColumnPriority = "0";
	marginBreakPriority = "-4";
	\consists "Mark_engraver";
	\consists "Bar_number_engraver";

};


HaraKiriStaffContext = \translator {
	\type "Hara_kiri_line_group_engraver";
	\name Staff;
	barColumnPriority = "0";
	marginBreakPriority = "-4";

	\consists "Repeat_engraver";
	\consists "Bar_engraver";
	\consists "Clef_engraver";
	\consists "Key_engraver";
	\consists "Time_signature_engraver";
	\consists "Local_key_engraver";
	\consists "Staff_symbol_engraver";
	\consists "Collision_engraver";
	\consists "Rest_collision_engraver";
	\consists "Staff_margin_engraver";
	\consists "Separating_line_group_engraver";
	  
	\accepts "Voice";
};

% huh, Score should do HaraKiri, not Part yes?
OrchestralPartStaffContext = \translator {
%	\HaraKiriStaffContext
	\StaffContext
	barColumnPriority = "0";
	marginBreakPriority = "-4";
	\consists "Mark_engraver";
	\consists "Bar_number_engraver";
};

% where to put HaraKiri here?
OrchestralScoreContext= \translator {
	\type Score_engraver;
	\name Score;
	barScriptPadding = "2.0";	% dimension \pt
	markScriptPadding = "4.0";
	barColumnPriority = "-4";
	markBreakPriority = "-4";
	defaultClef = treble;

	\consists "Timing_engraver";

	\consists "Bar_number_engraver";
	\consists "Mark_engraver";
	\consists "Span_score_bar_engraver";
	\consists "Score_priority_engraver";
	\consists "Priority_horizontal_align_engraver";
	\consists "Vertical_align_engraver";

	\accepts "ChoirStaff";
	\accepts "StaffGroup";
	\accepts "Staff";
	\accepts "RhythmicStaff";	
	\accepts "Lyrics";
	\accepts "ChordNames";
	\accepts "GrandStaff";
	\accepts "PianoStaff";
	
};
