//
//  source.hh -- part of LilyPond
//
//  copyright 1997 Jan Nieuwenhuizen <janneke@gnu.org>

#ifndef SOURCE_HH
#define SOURCE_HH
#include "cons.hh"
#include "proto.hh"

/**
   a set of sourcefiles.

   TODO: 
 */
class Sources 
{
  Sources (Sources const&) {}
public:
  Sources ();
  ~Sources();

  Source_file * get_file_l (String &filename );
  Source_file* sourcefile_l (char const* ch_C );
  void add (Source_file* sourcefile_p );
  void set_path (File_path*p_C);
  void set_binary (bool);

  const File_path * path_C_;
private:
  Cons<Source_file> *sourcefile_p_list_;
  bool binary_b_ ;
};



#endif // SOURCE_HH //
