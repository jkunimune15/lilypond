/*
  simple-file-storage.cc -- implement Simple_file_storage

  source file of the GNU LilyPond music typesetter

  (c)  1997--2000 Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/

#include <stdio.h>

#ifndef SEEK_END
#define SEEK_END 2
#endif

#include "simple-file-storage.hh"
#include "array.hh"
#include "string.hh"
#include "warn.hh"

void
Simple_file_storage::load_stdin ()
{
  len_i_ = 0;

  int c;
  Array<char> ch_arr;
  while ((c = fgetc (stdin)) != EOF)
    ch_arr.push (c);
  len_i_ = ch_arr.size ();
  data_p_ = ch_arr.remove_array_p ();
}

void
Simple_file_storage::load_file (String s)
{
  /*
    let's hope that "b" opens anything binary, and does not apply
    CR/LF translation
    */
  FILE * f =  fopen (s.ch_C (), "rb");

  if (!f)
    {
      warning (_f ("Can't open file: `%s'", s));
      return ;
    }

  int ret = fseek (f, 0, SEEK_END);
  len_i_ = ftell (f);
  rewind (f);
  data_p_ = new char[len_i_+1];
  data_p_[len_i_] = 0;
  ret = fread (data_p_, sizeof (char), len_i_, f);

  if  (ret!=len_i_)
    warning (_f ("Huh?  Got %d, expected %d characters", ret, len_i_));

  fclose (f);
}

/**
  Stupid but foolproof way of opening files.

  TODO
  Should check IO status

  This is of course a build it yourself version of mmap, so we should
  have been using that..., but this is simple & portable
  
*/

Simple_file_storage::Simple_file_storage (String s)
{
  data_p_ = 0;
  len_i_ = 0;

  if (!s.length_i () || (s == "-"))
    load_stdin ();
  else
    load_file (s);
}

char const*
Simple_file_storage::ch_C () const
{
  return data_p_;
}

int
Simple_file_storage::length_i () const
{
  return len_i_;
}


Simple_file_storage::~Simple_file_storage ()
{
  delete []data_p_;
}
