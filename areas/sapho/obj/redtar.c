#include <mudlib.h>
inherit OBJECT;

void reset(int arg)
{
  ::reset(arg);
  if(arg) return;

  set_name ("sapho");
  set_weight (1);


  set_short ("A chunk of Red Tar Sapho");
  
  set_long(
    "This is a big chunk of red tar sapho. The smell is quite fruity, and\n"
	+"very strong. It feels like the texture of tar. If you have one, you\n"
	+"can probably 'pack' this into a pipe.\n"
}
