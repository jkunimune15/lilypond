/*   
'  separating-line-group-engraver.cc --  implement Separating_line_group_engraver
  
  source file of the GNU LilyPond music typesetter
  
  (c) 1998--2001 Han-Wen Nienhuys <hanwen@cs.uu.nl>
  
 */

#include "separating-group-spanner.hh"
#include "separation-item.hh"
#include "paper-column.hh"
#include "paper-def.hh"
#include "engraver.hh"
#include "axis-group-interface.hh"

class Separating_line_group_engraver : public Engraver
{
protected:
  Item * break_malt_p_;
  Item * musical_malt_p_;

  /*
    malt_p_ : we used to have a Single_malt_grouping_item
    
   */
  Item * last_step_musical_malt_p_;
  
  Spanner * sep_span_p_;
  
  virtual void acknowledge_grob (Grob_info);
  virtual void initialize ();
  virtual void finalize ();
  virtual void stop_translation_timestep ();
public:
  Separating_line_group_engraver ();
  VIRTUAL_COPY_CONS (Translator);
};

Separating_line_group_engraver::Separating_line_group_engraver ()
{
  last_step_musical_malt_p_ = 0;
  sep_span_p_ = 0;
  break_malt_p_ = 0;
  musical_malt_p_ =0;
}

void
Separating_line_group_engraver::initialize ()
{
  sep_span_p_ = new Spanner (get_property ("SeparatingGroupSpanner"));

  announce_grob (sep_span_p_, 0);
  sep_span_p_->set_bound (LEFT, unsmob_grob (get_property ("currentCommandColumn")));
}

void
Separating_line_group_engraver::finalize ()
{
  sep_span_p_->set_bound (RIGHT, unsmob_grob (get_property ("currentCommandColumn")));
  typeset_grob (sep_span_p_);
  sep_span_p_ =0;
}

void
Separating_line_group_engraver::acknowledge_grob (Grob_info i)
{
  Item * it = dynamic_cast <Item *> (i.elem_l_);
  if (!it)
    return;
  if (it->parent_l (X_AXIS)
      && it->parent_l (X_AXIS)->has_extent_callback_b
 (Axis_group_interface::group_extent_callback_proc, X_AXIS))
    return;

  
  bool ib =Item::breakable_b (it);
  Item *&p_ref_ (ib ? break_malt_p_
		 : musical_malt_p_);

  if (!p_ref_)
    {
      p_ref_ = new Item
 (get_property ("SeparationItem"));
	  
      if (ib)
	p_ref_->set_grob_property ("breakable", SCM_BOOL_T);
      announce_grob (p_ref_, 0);
    }
  Separation_item::add_item (p_ref_,it);
}

void
Separating_line_group_engraver::stop_translation_timestep ()
{
  if (break_malt_p_)
    {
      Separating_group_spanner::add_spacing_unit (sep_span_p_, break_malt_p_);
      
      typeset_grob (break_malt_p_);
      break_malt_p_ =0;
    }
  if (musical_malt_p_)
    {
      Separating_group_spanner::add_spacing_unit (sep_span_p_, musical_malt_p_);

      if (last_step_musical_malt_p_)
	{
	  Paper_column *col = 
	    last_step_musical_malt_p_->column_l();
	  SCM newtup = gh_cons (last_step_musical_malt_p_->self_scm (),
				musical_malt_p_->self_scm ());
	  col->set_grob_property ("spacing-sequence",
				  gh_cons (newtup,
					   col->get_grob_property ("spacing-sequence")));
	}
      
      typeset_grob (musical_malt_p_);
    }
  last_step_musical_malt_p_ = musical_malt_p_;
  musical_malt_p_ =0;

}



ADD_THIS_TRANSLATOR (Separating_line_group_engraver);

