// recipe_quest.c - Recipe Quest Item - Bebop 2021
// This item is found in the boss room (room/saferoom.c)
// It is given to (/npc/questmob.c) located in (room/questroom.c)

#include <mudlib.h>
inherit OBJECT;

void reset(int arg)
{
  ::reset(arg);
  if (!arg) {

    set_name("recipe");
    set_alias("recipe_quest_item");
    set_short("A recipe for red tar sapho");
    set_long(
      "This is the recipe that Gabriel uses to refine and produce the addicting\n"
      +"red tar sapho. It's full of scientific formulae and a ton of chemicals\n"
      +"you've never heard of. Someone might appreciate it if you 'show recipe to'\n"
      +"them...");

    set_weight(0);
  }
}