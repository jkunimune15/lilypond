/*
  lyric-phrasing-engraver.hh -- declare Lyric_phrasing_engraver

  source file of the GNU LilyPond music typesetter

  (c)  2000 Glen Prideaux <glenprideaux@iname.com>
*/


#ifndef LYRIC_PHRASING_ENGRAVER_HH
#define LYRIC_PHRASING_ENGRAVER_HH

#include "lily-proto.hh"
#include "engraver.hh"
#include "item.hh"
#include "smobs.hh"
#include "protected-scm.hh"

class Syllable_group;


/**
   Align lyrics with noteheads, left aligning beginning of phrases,
   right aligning end of phrases, centering others under their notes.
 */

/* 
* Build an engraver that catches noteheads and lyrics.

 (It needs to be in a context above Staff and Lyrics, eg. in Score
context.)

* Determine which heads belong to which lyrics

 (eg. by looking at the names of their originating contexts, or maybe
some \properties)

* Attach the lyrics to the appropriate heads

 (by doing lyric->set_parent (head, X_AXIS), that will fix the current
noteheadwidth guessing kludge)

* Check if the lyric syllables end or start a phrase.

 (eg. check if the syllable ends with punctuation, and remember that
fact for the next one.)

* Adjust their alignment accordingly. 

 (eg. by doing lyric->add_offset_callback (centered_on_parent,X_AXIS)
and setting self-alignment-X)

* Add a property to switch on/off the engraver (for multi stanza
  vs. single stanza music)

Maybe this engraver could also take care of correct lyric alignment
for melismas as well.


 */


class Lyric_phrasing_engraver : public Engraver 
{
protected:
  virtual void acknowledge_grob (Grob_info);
  virtual void process_acknowledged_grobs ();
  virtual void stop_translation_timestep ();
  virtual void finalize ();  
private:
  void record_notehead (const String &context_id, Grob * notehead);
  void record_lyric (const String &context_id, Grob * lyric);
  void record_melisma (const String &context_id);
  void record_extender (const String &context_id, Grob * extender);
  Syllable_group * lookup_context_id (const String &context_id);

public:

  ~Lyric_phrasing_engraver ();
  TRANSLATOR_DECLARATIONS(  Lyric_phrasing_engraver);

private:
  /** association list of Syllable_group smobs

  fixme: should use property.
  
  */
  Protected_scm voice_alist_;
  Grob * any_notehead_l_;
};


class Syllable_group
{
  bool first_in_phrase_b_;
  Grob * notehead_l_;
  Link_array<Grob> lyric_list_;
  Grob * longest_lyric_l_;
  Grob * shortest_lyric_l_;
  int alignment_i_;
  bool melisma_b_;
  Real group_translation_f_;
public:
  static SCM make_entry ();
  void set_first_in_phrase (bool f);
  void set_notehead (Grob * notehead);
  void add_lyric (Grob * lyric);
  void add_extender (Grob * extender);
  void set_melisma () { melisma_b_ = true; }
  bool get_melisma () { return melisma_b_; }
  int lyric_count () { return lyric_list_.size (); }
  void clear ();
  bool is_empty ();
  bool set_lyric_align (const char *punc, Grob *default_notehead_l);
  void adjust_melisma_align ();
  int appropriate_alignment (const char *punc);
  Real amount_to_translate ();
  void next_lyric ();
  void copy (Syllable_group *);
private:
  Syllable_group ();
  DECLARE_SIMPLE_SMOBS (Syllable_group,);
} ;

DECLARE_UNSMOB(Syllable_group,voice_entry);


#endif // LYRIC_PHRASING_ENGRAVER_HH
