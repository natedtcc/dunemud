// sapho.c - Smokable Red-Tar Sapho - Bebop 2018

#include <mudlib.h>
inherit OBJECT;

void reset(int arg)
{
  ::reset(arg);
  if (!arg) {
    set_weight (0);
    set_name ("sapho");
    set_alias("sapho_smokable");
    set_short ("A chunk of Red Tar Sapho");
    
    set_long(
      "This is a big chunk of red tar sapho. The smell is quite fruity, and\n"
      +"very strong. It feels like the texture of tar. If you have one, you\n"
      +"can probably 'pack' this into a pipe.\n");
  }
}