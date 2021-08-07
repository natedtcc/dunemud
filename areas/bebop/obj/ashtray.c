#include <mudlib.h>
inherit OBJECT;

int emptied_hits;

void reset(int arg)
{
  ::reset(arg);

if(arg) return;

set_name ("ashtray");
set_alias ( ({ "ashtray", "tray" }) );
emptied_hits = 0;
set_long (
  "This ashtray is full of gray ashes and little bits of\n"
  +"tree. There have been " + emptied_hits + " bowls emptied into it.");

set_short ("A chrome ash tray");
set_no_get (1);

set_weight (999);
}

int get_hits(){
  return emptied_hits;  
}

void add_hit(){
  emptied_hits++;
  if (emptied_hits == 1){
  set_long (
    "This ashtray is full of gray ashes and little bits of\n"
    +"tree. It looks like it hasn't been emptied in a while...\n"
    +"There has been " + emptied_hits + " bowl emptied into it.");
  }
  else {
    set_long (
    "This ashtray is full of gray ashes and little bits of\n"
    +"tree. It looks like it hasn't been emptied in a while...\n"
    +"There have been " + emptied_hits + " bowls emptied into it.");
  }
}

void reset_hits(){
  emptied_hits = 0;
  set_long (
    "This ashtray is full of gray ashes and little bits of\n"
    +"tree. It looks like it hasn't been emptied in a while...\n"
    +"There have been " + emptied_hits + " bowls emptied into it.");
}