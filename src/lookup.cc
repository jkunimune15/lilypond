#include "lookup.hh"
#include "debug.hh"
#include "symtable.hh"
#include "dimen.hh"
#include "tex.hh"
#include "scalar.hh"

Real
Lookup::internote()
{
    return ball(4).dim.y.length()/2;
}

void
Lookup::parse(Text_db&t)
{
    symtables_->read(t) ;
}

Lookup::Lookup()
{
    symtables_ = new Symtables;
}

Lookup::~Lookup()
{
    delete symtables_;
}

Symbol
Lookup::ball(int j)
{
    if (j > 4)
	j = 4;

    Symtable * st = (*symtables_)("balls");
    return st->lookup(String(j));
}

Symbol
Lookup::rest(int j)
{
    return (*symtables_)("rests")->lookup(String(j));
}

Symbol
Lookup::accidental(int j)
{
    return (*symtables_)("accidentals")->lookup(String(j));
}


Symbol
Lookup::bar(String s)
{
    return (*symtables_)("bars")->lookup(s);
}

Symbol
Lookup::clef(String s)
{
    return (*symtables_)("clefs")->lookup(s);
}
 
Symbol
Lookup::dots(int j)
{
    if (j>3)
	error("max 3 dots");
    return (*symtables_)("dots")->lookup(j);
}

Symbol
Lookup::flag(int j)
{
    return (*symtables_)("flags")->lookup(j);
}

Symbol
Lookup::streepjes(int i)
{
    assert(i);
    
    int arg;
    String idx;
    
    if (i < 0) {
	idx = "botlines";
	arg = -i;
    } else {
	arg = i;
	idx = "toplines";
    }
    Symbol ret = (*symtables_)("streepjes")->lookup(idx);
    
    svec<String> a;
    a.add(arg);
    ret.tex = substitute_args(ret.tex, a);

    return ret;
}

/****************************************************************/
// bare bones.


Symbol
Lookup::linestaff(int lines, Real wid) 
{
    Symbol s;
    s.dim.x = Interval(0,wid);
    Real dy=(lines-1)*internote()*2;
    s.dim.y = Interval(0,dy);

    svec<String> a;
    a.add(lines);
    a.add(print_dimen(wid));

    s.tex = (*symtables_)("param")->lookup("linestaf").tex;
    s.tex = substitute_args(s.tex, a);
    
    return s;
}

/****************************************************************/



Symbol
Lookup::meter(svec<Scalar> a)
{
    Symbol s;
    s.dim.x = Interval( convert_dimen(0,"pt"),
			convert_dimen(10,"pt"));
    s.dim.y = Interval(0, convert_dimen(20,"pt") );	// todo
    String src = (*symtables_)("param")->lookup("meter").tex;
    s.tex = substitute_args(src,a);
    return s;    
}


Symbol
Lookup::stem(Real y1,Real y2)
{
    assert(y1 <= y2);
    Symbol s;
    
    s.dim.x = Interval(0,0);
    s.dim.y = Interval(y1,y2);
    
    svec<String> a;
    a.add(print_dimen(y1));
    a.add(print_dimen(y2));
	
    String src = (*symtables_)("param")->lookup("stem").tex;
    s.tex = substitute_args(src,a);
    return s;
}


