/*   
  piano-pedal-engraver.cc --  implement Piano_pedal_engraver
  
  source file of the GNU LilyPond music typesetter
  
  (c) 2000--2001 Jan Nieuwenhuizen <janneke@gnu.org>
 */

#include "engraver.hh"
#include "musical-request.hh"
#include "grob.hh"
#include "item.hh"
#include "lily-guile.hh"
#include "rhythmic-head.hh"
#include "stem.hh"
#include "side-position-interface.hh"
#include "staff-symbol-referencer.hh"
#include "item.hh"

class Piano_pedal_engraver : public Engraver
{
public:
  TRANSLATOR_DECLARATIONS(Piano_pedal_engraver);
  ~Piano_pedal_engraver ();
protected:
  virtual void initialize ();
  virtual bool try_music (Music*);
  virtual void stop_translation_timestep ();
  virtual void start_translation_timestep ();
  virtual void acknowledge_grob (Grob_info);
  virtual void create_grobs ();

private:
  struct Pedal_info
  {
    char const * name_;
    Span_req* start_req_l_;
    Drul_array<Span_req*> req_l_drul_;
    Item* item_p_;
  };


  Pedal_info *info_list_;
};



Piano_pedal_engraver::Piano_pedal_engraver ()
{
  info_list_ = 0;
}
void
Piano_pedal_engraver::initialize ()
{
  info_list_ = new Pedal_info[4];
  Pedal_info *p = info_list_;


  char * names [] = { "Sostenuto", "Sustain", "UnaCorda", 0  };
  char **np = names ;
  do
    {
      p->name_ = *np;
      p->item_p_ = 0;
      p->req_l_drul_[START] = 0;
      p->req_l_drul_[STOP] = 0;
      p->start_req_l_ = 0;

      p++;
    }
  while (* (np ++));
}

Piano_pedal_engraver::~Piano_pedal_engraver ()
{
  delete[] info_list_;
}

/*
   Urg: Code dup
   I'm a script
  */
void
Piano_pedal_engraver::acknowledge_grob (Grob_info info)
{
  for (Pedal_info*p = info_list_; p && p->name_; p ++)
    {
      if (p->item_p_)
	{
	  if (Rhythmic_head::has_interface (info.grob_l_))
	    {
	      Side_position_interface::add_support (p->item_p_, info.grob_l_);

	      if (Side_position_interface::get_axis (p->item_p_) == X_AXIS
		  && !p->item_p_->get_parent (Y_AXIS))
		p->item_p_->set_parent (info.grob_l_, Y_AXIS);
	    }
	  if (Stem::has_interface (info.grob_l_))
	    {
	      Side_position_interface::add_support (p->item_p_,info.grob_l_);
	    }
	}
    }
}

bool
Piano_pedal_engraver::try_music (Music *m)
{
  if (Span_req * s = dynamic_cast<Span_req*> (m))
    {
      for (Pedal_info*p = info_list_; p->name_; p ++)
	{
	  if (scm_equal_p (s->get_mus_property ("span-type"),
			   ly_str02scm (p->name_))==SCM_BOOL_T)
	    {
	      p->req_l_drul_[s->get_span_dir ()] = s;
	      return true;
	    }
	}
    }
  return false;
}

void
Piano_pedal_engraver::create_grobs ()
{
  for (Pedal_info*p = info_list_; p && p->name_; p ++)
    {
      if (p->item_p_ || ! (p->req_l_drul_[STOP] || p->req_l_drul_[START]))
	continue;
      
      SCM s = SCM_EOL;
      SCM strings = get_property (("pedal" + String (p->name_) + "Strings").ch_C ());
      if (scm_ilength (strings) < 3)
	continue;
      
      if (p->req_l_drul_[STOP] && p->req_l_drul_[START])
	{
	  if (!p->start_req_l_)
	    {
	      p->req_l_drul_[STOP]->origin ()->warning (_f ("can't find start of piano pedal: `%s'",  p->name_));
	    }
	  else
	    {
	      s = ly_cadr (strings);
	    }
	  p->start_req_l_ = p->req_l_drul_[START];
	}
      else if (p->req_l_drul_[STOP])
	{
	  if (!p->start_req_l_)
	    {
	      p->req_l_drul_[STOP]->origin ()->warning (_f ("can't find start of piano pedal: `%s'", p->name_));
	    }
	  else
	    {
	      s = ly_caddr (strings);
	    }
	  p->start_req_l_ = 0;
	}
      else if (p->req_l_drul_[START])
	{
	  p->start_req_l_ = p->req_l_drul_[START];
	  s = ly_car (strings);
	}

      if (gh_string_p (s))
	{
	  String propname = String (p->name_) + "Pedal";
	  p->item_p_ = new Item (get_property (propname.ch_C ()));
	  p->item_p_->set_grob_property ("text", s);

	  announce_grob (p->item_p_,
			    p->req_l_drul_[START]
			    ? p->req_l_drul_[START]
			    : p->req_l_drul_[STOP]);
	}
      p->req_l_drul_[START] = 0;
      p->req_l_drul_[STOP] = 0;
    }
}

void
Piano_pedal_engraver::stop_translation_timestep ()
{
  Item * sustain = 0;
  for (Pedal_info*p = info_list_; p->name_; p ++)
    {
      if (p->name_ == String ("Sustain"))
	sustain = p->item_p_;
    }

  for (Pedal_info*p = info_list_; p->name_; p ++)
    {
      if (p->item_p_)
	{
	  Side_position_interface::add_staff_support (p->item_p_);
	  
	  /*
	    Hmm.
	  */
	  if (p->name_ != String ("Sustain"))
	    {
	      if (sustain)
		{
		  Side_position_interface::add_support (p->item_p_,sustain);
		}
	    }
	  typeset_grob (p->item_p_);
	}
      p->item_p_ = 0;
    }
}

void
Piano_pedal_engraver::start_translation_timestep ()
{
  for (Pedal_info*p = info_list_; p->name_; p ++)
    {
      p->req_l_drul_[STOP] = 0;
      p->req_l_drul_[START] = 0;
    }
}
ENTER_DESCRIPTION(Piano_pedal_engraver,
/* descr */       "Engrave piano pedal symbols.",
/* creats*/       "SostenutoPedal SustainPedal UnaCordaPedal",
/* acks  */       "rhythmic-head-interface stem-interface",
/* reads */       "pedalSostenutoStrings pedalSustainStrings pedalUnaCordaStrings",
/* write */       "");
