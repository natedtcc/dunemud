// sapho_loot.c - Item found from searching boxes in Packaging Facility
// This can be sold for top dollar or given to the quest mob
// Bebop 2021

#include <mudlib.h>
inherit OBJECT;

void reset(int arg)
{
  ::reset(arg);
  if(arg) return;

  set_name ("sapho");
  set_alias(({"sapho", "sapho_loot"}));
  set_weight(2);

  set_short ("A large mass of Red Tar Sapho");
  
  set_long(
    "This is a massive rock of red tar sapho, pillaged from the packaging\n"
	  +"area of the processing plant You can almost hear it calling for you... \n"
	  +"This thing is probably worth a pretty good chunk of money at the shops,\n"
    +"something even more to the right person..\n");
}
