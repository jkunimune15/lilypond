/*
  paper-book.cc -- implement Paper_book

  source file of the GNU LilyPond music typesetter

  (c) 2004 Jan Nieuwenhuizen <janneke@gnu.org>
*/

#include "ly-module.hh"
#include "main.hh"
#include "page.hh"
#include "paper-book.hh"
#include "output-def.hh"
#include "paper-outputter.hh"
#include "paper-line.hh"
#include "paper-score.hh"
#include "stencil.hh"
#include "warn.hh"


// JUNKME
SCM
stencil2line (Stencil stil, bool is_title = false)
{
  static SCM z;
  if (!z)
    z = scm_permanent_object (ly_offset2scm (Offset (0, 0)));
  Offset dim = Offset (stil.extent (X_AXIS).length (),
		       stil.extent (Y_AXIS).length ());
  Paper_line *pl = new Paper_line (dim, scm_cons (stil.smobbed_copy (),
						  SCM_EOL),
				   -10001 * is_title, is_title);

  return scm_gc_unprotect_object (pl->self_scm ());
}


Paper_book::Paper_book ()
{
  copyright_ = SCM_EOL;
  tagline_ = SCM_EOL;
  header_ = SCM_EOL;
  
  bookpaper_ = 0;
  smobify_self ();
}

Paper_book::~Paper_book ()
{
}

#include "ly-smobs.icc"

IMPLEMENT_DEFAULT_EQUAL_P (Paper_book);
IMPLEMENT_SMOBS (Paper_book)
IMPLEMENT_TYPE_P (Paper_book, "ly:paper-book?")

SCM
Paper_book::mark_smob (SCM smob)
{
  Paper_book *b = (Paper_book*) SCM_CELL_WORD_1 (smob);
  for (int i = 0; i < b->score_lines_.size (); i++)
    b->score_lines_[i].gc_mark ();

  scm_gc_mark (b->copyright_);
  if (b->bookpaper_)
    scm_gc_mark (b->bookpaper_->self_scm ());
  scm_gc_mark (b->header_);
  return b->tagline_;
}

int
Paper_book::print_smob (SCM smob, SCM port, scm_print_state*)
{
  Paper_book *b = (Paper_book*) ly_cdr (smob);
     
  scm_puts ("#<", port);
  scm_puts (classname (b), port);
  scm_puts (" ", port);
  //scm_puts (b->, port);
  scm_puts (">", port);
  return 1;
}


/*
  TODO: there is too much code dup, and the interface is not
  clear. FIXME.
 */
void
Paper_book::output (String outname)
{
  if (!score_lines_.size ())
    // FIXME: no end-output?
    return;
    
  /* Generate all stencils to trigger font loads.  */
  SCM pages = this->pages ();

  Paper_outputter *out = get_paper_outputter (outname);
  int page_count = scm_ilength (pages);

  SCM scopes = SCM_EOL;
  if (ly_c_module_p (header_))
    scopes = scm_cons (header_, scopes);
  
  out->output_header (bookpaper_, scopes, page_count, false);

  for (SCM s = pages; s != SCM_EOL; s = ly_cdr (s))
    {
      Page *p = unsmob_page (ly_car (s));
      progress_indication ("[" + to_string (p->number_));
      out->output_page (p, ly_cdr (s) == SCM_EOL);
      progress_indication ("]");
    }

  out->output_scheme (scm_list_1 (ly_symbol2scm ("end-output")));
  progress_indication ("\n");
}


Stencil
Paper_book::title (int i)
{
  SCM user_title = bookpaper_->lookup_variable (ly_symbol2scm ("user-title"));
  SCM book_title = bookpaper_->lookup_variable (ly_symbol2scm ("book-title"));
  SCM score_title = bookpaper_->lookup_variable (ly_symbol2scm ("score-title"));
  SCM field = (i == 0 ? ly_symbol2scm ("bookTitle")
	       : ly_symbol2scm ("scoreTitle"));

  Stencil title;

  // ugh code dup
  SCM scopes = SCM_EOL;
  if (ly_c_module_p (header_))
    scopes = scm_cons (header_, scopes);

  if (ly_c_module_p (score_lines_[i].header_))
    scopes = scm_cons (score_lines_[i].header_, scopes);
   //end ugh
  
  SCM s = ly_modules_lookup (scopes, field, SCM_BOOL_F);
  if (s != SCM_BOOL_F)
    title = *unsmob_stencil (scm_call_2 (user_title,
					 bookpaper_->self_scm (),
					 s));
  else
    title = *unsmob_stencil (scm_call_2 (i == 0 ? book_title : score_title,
					 bookpaper_->self_scm (),
					 scopes));
  if (!title.is_empty ())
    title.align_to (Y_AXIS, UP);
  
  return title;
}

void
Paper_book::classic_output (String outname)
{
  Paper_outputter *out = get_paper_outputter (outname);

  Output_def * p = bookpaper_;
  while (p && p->parent_)
    p = p->parent_;

  // ugh code dup
  SCM scopes = SCM_EOL;
  if (ly_c_module_p (header_))
    scopes = scm_cons (header_, scopes);

  if (ly_c_module_p (score_lines_[0].header_))
    scopes = scm_cons (score_lines_[0].header_, scopes);
  //end ugh
  
  out->output_header (p, scopes, 0, true);

  SCM top_lines = score_lines_.top ().lines_;
  Paper_line *first = unsmob_paper_line (scm_vector_ref (top_lines,
							 scm_int2num (0)));
  Offset o (0, -0.5 * first->dim ()[Y_AXIS]);
  int line_count = SCM_VECTOR_LENGTH (top_lines);
  for (int i = 0; i < line_count; i++)
    {
      /* In classic compatibility TeX tracks how large things are, and
	 advances the Y pos for us.  If we advance it too, we get too
	 much space.

	 FIXME: vague... why is TeX is different from other ouput
	        backends, why not fix the TeX backend? -- jcn */
      if (output_format_global == "tex")
	o = Offset (0, 0);

      out->output_line (scm_vector_ref (top_lines, scm_int2num (i)),
			&o, i == line_count - 1);
    }
  
  out->output_scheme (scm_list_1 (ly_symbol2scm ("end-output")));
  progress_indication ("\n");
}

/* calculate book height, #lines, stencils.  */
void
Paper_book::init ()
{
  int score_count = score_lines_.size ();

  /* Calculate the full book height.  Hmm, can't we cache system
     heights while making stencils?  */
  height_ = 0;
  for (int i = 0; i < score_count; i++)
    {
      Stencil title = this->title (i);
      if (!title.is_empty ())
	height_ += title.extent (Y_AXIS).length ();

      int line_count = SCM_VECTOR_LENGTH (score_lines_[i].lines_);
      for (int j = 0; j < line_count; j++)
	{
	  SCM s = scm_vector_ref ((SCM) score_lines_[i].lines_, scm_int2num (j));
	  height_ += unsmob_paper_line (s)->dim ()[Y_AXIS];
	}
    }

  Output_def *paper = bookpaper_;
  
  SCM scopes = SCM_EOL;
  if (ly_c_module_p (header_))
    scopes = scm_cons (header_, scopes);

  SCM make_tagline = paper->c_variable ("make-tagline");
  tagline_ = scm_call_2 (make_tagline, paper->self_scm (), scopes);
  Real tag_height = 0;
  if (Stencil *s = unsmob_stencil (tagline_))
    tag_height = s->extent (Y_AXIS).length ();
  height_ += tag_height;

  SCM make_copyright = paper->c_variable ("make-copyright");
  copyright_ = scm_call_2 (make_copyright, paper->self_scm (), scopes);
  Real copy_height = 0;
  if (Stencil *s = unsmob_stencil (copyright_))
    copy_height = s->extent (Y_AXIS).length ();
  height_ += copy_height;
}

SCM
Paper_book::lines ()
{
  int score_count = score_lines_.size ();
  SCM lines = SCM_EOL;
  for (int i = 0; i < score_count; i++)
    {
      Stencil title = this->title (i);      
      if (!title.is_empty ())
	lines = ly_snoc (stencil2line (title, true), lines);
      lines = scm_append (scm_list_2 (lines, scm_vector_to_list (score_lines_[i].lines_)));
    }
  
  //debug helper; ughr
  int i = 0;
  for (SCM s = lines; s != SCM_EOL; s = ly_cdr (s))
    unsmob_paper_line (ly_car (s))->number_ = ++i;
  return lines;
}

SCM
Paper_book::pages ()
{
  init ();
  Page::page_count_ = 0;

  Output_def *paper = bookpaper_;
  Page *page = new Page (paper, 1);

  Real text_height = page->text_height ();

  Real copy_height = 0;
  if (Stencil *s = unsmob_stencil (copyright_))
    copy_height = s->extent (Y_AXIS).length ();

  Real tag_height = 0;
  if (Stencil *s = unsmob_stencil (tagline_))
    tag_height = s->extent (Y_AXIS).length ();

  SCM all = lines ();
  SCM proc = paper->c_variable ("page-breaking");
  SCM breaks = scm_apply_0 (proc, scm_list_n (all, scm_make_real (height_),
					      scm_make_real (text_height),
					      scm_make_real (-copy_height),
					      scm_make_real (-tag_height),
					      SCM_UNDEFINED));

  /* Copyright on first page.  */
  if (unsmob_stencil (copyright_))
    page->copyright_ = copyright_;

  SCM pages = SCM_EOL;
  int page_count = SCM_VECTOR_LENGTH ((SCM) breaks);
  int line = 1;
  for (int i = 0; i < page_count; i++)
    {
      if (i)
	page = new Page (paper, i + 1);

      int next = i + 1 < page_count
	? ly_scm2int (scm_vector_ref (breaks, scm_int2num (i))) : 0;
      while ((!next && all != SCM_EOL) || line <= next)
	{
	  SCM s = ly_car (all);
	  page->lines_ = ly_snoc (s, page->lines_);
	  page->height_ += unsmob_paper_line (s)->dim ()[Y_AXIS];
	  page->line_count_++;
	  all = ly_cdr (all);
	  line++;
	}
      pages = scm_cons (page->self_scm (), pages);
    }

  /* Tagline on last page.  */
  if (unsmob_stencil (tagline_))
    page->tagline_ = tagline_;

  return scm_reverse (pages);
}

static SCM
c_ragged_page_breaks (SCM lines, Real book_height, Real text_height,
		      Real first, Real last)
{
  int page_number = 0;
  int page_count = int (book_height / text_height + 0.5);
  SCM breaks = SCM_EOL;
  Real page_height = text_height + first;
  Real h = 0;
  int number = 0;
  for (SCM s = lines; s != SCM_EOL; s = ly_cdr (s))
    {
      Paper_line *pl = unsmob_paper_line (ly_car (s));
      if (!pl->is_title () && h < page_height)
	number++;
      h += pl->dim ()[Y_AXIS];
      if (!pl->is_title () && h > page_height)
	{
	  breaks = ly_snoc (scm_int2num (number), breaks);
	  page_number++;
	  page_height = text_height + (page_number == page_count) * last;
	  h = 0;
	}
      if (ly_cdr (s) == SCM_EOL)
	breaks = ly_snoc (scm_int2num (pl->number_), breaks);
    }

  return scm_vector (breaks);
}

LY_DEFINE (ly_ragged_page_breaks, "ly:ragged-page-breaks",
	   5, 0, 0, (SCM lines, SCM book, SCM text, SCM first, SCM last),
	   "Return a vector with line numbers of page breaks.")
{
  SCM_ASSERT_TYPE (scm_pair_p (lines), lines, SCM_ARG1, __FUNCTION__, "list");
  SCM_ASSERT_TYPE (ly_c_number_p (book), book, SCM_ARG2, __FUNCTION__, "real");
  SCM_ASSERT_TYPE (ly_c_number_p (text), text, SCM_ARG2, __FUNCTION__, "real");
  SCM_ASSERT_TYPE (ly_c_number_p (first), first, SCM_ARG2, __FUNCTION__, "real");
  SCM_ASSERT_TYPE (ly_c_number_p (last), last, SCM_ARG2, __FUNCTION__, "real");

  return c_ragged_page_breaks (lines,
			       ly_scm2double (book), ly_scm2double (text),
			       ly_scm2double (first), ly_scm2double (last));
}

/****************************************************************/

Score_lines::Score_lines ()
{
  lines_ = SCM_EOL;
  header_ = SCM_EOL;
}

void
Score_lines::gc_mark ()
{
  scm_gc_mark (lines_);
  scm_gc_mark (header_);
}

