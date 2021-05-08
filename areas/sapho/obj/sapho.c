#include <mudlib.h>
inherit OBJECT;

void reset(int arg)
{
  ::reset(arg);
  if(arg) return;
  set_weight (1);
  set_name ("sapho");
  set_alias(({"sapho", "sapho_smokable"}));
  set_short ("A chunk of Red Tar Sapho");
  
  set_long(
    "This is a big chunk of red tar sapho. The smell is quite fruity, and\n"
	  +"very strong. It feels like the texture of tar. If you have one, you\n"
	  +"can probably 'pack' this into a pipe.\n");
}

void init ()
{
  ::init ();
 
  add_action ("do_pack", "pack");
  add_action ("do_smoke", "smoke");
}

void do_pack (string str)
{
  if (present("pipe", this_player())){
  
  }
}

int dest_sapho()
{
  destruct(this_object());
  return 1;
}