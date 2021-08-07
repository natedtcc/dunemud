#include <mudlib.h>
inherit OBJECT;



void reset(int arg)
{
  ::reset(arg);

if(arg) return;

set_name ("sofa");
set_alias ( ({ "couch", "sofa" }) );

set_long (
  "This ashtray is full of gray ashes and little bits of\n"
  +"tree. There have been " + emptied_hits + " bowls emptied into it.");

set_short ("A massize L-shaped sofa");
set_no_get (1);
set_weight (999);
}

void init ()
{
  ::init ();

  add_action ("sit", "sit");
  add_action ("stand", "stand");
}

